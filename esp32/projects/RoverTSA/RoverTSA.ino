// RoverTSA.ino
// ESP32 + L298N + Xbox (Bluepad32) + OLED + menu + 2 buttons + trajectory record/play

#include <Arduino.h>

#include "Pins.h"
#include "MotorController.h"
#include "XboxDrive.h"
#include "DisplayManager.h"
#include "ButtonManager.h"
#include "TrajectoryRecorder.h" 
#include "ServoController.h"




// ===== Instances =====
MotorController motors(
  PIN_IN1, PIN_IN2, PIN_ENA,
  PIN_IN3, PIN_IN4, PIN_ENB,
  PWM_CH_LEFT, PWM_CH_RIGHT,
  PWM_FREQ_HZ, PWM_RES_BITS
);

XboxDrive xbox;
DisplayManager display;
DisplayManager::Settings settings;
DisplayManager::MenuState menu;
ServoController arm;

ButtonManager buttons(PIN_BTN_A, PIN_BTN_B);
TrajectoryRecorder track;         // <-- NEW

// ===== Failsafe =====
static unsigned long lastCmdMs = 0;
static const unsigned long FAILSAFE_MS = 500;

static unsigned long lastUiMs = 0;
static const unsigned long UI_PERIOD_MS = 120; // faster UI feels nicer
static uint32_t lastArmControlMs = 0;

// ===== Speed helpers =====
static float speedScale(DisplayManager::SpeedMode s) {
  switch (s) {
    case DisplayManager::SpeedMode::Fast:   return 1.0f;
    case DisplayManager::SpeedMode::Slow:   return 0.75f;
    default: return 0.7f;
  }
}

static int applyMinPWM(int v, int minPwm) {
  if (v == 0) return 0;
  int sign = (v > 0) ? 1 : -1;
  int mag = abs(v);
  if (mag < minPwm) mag = minPwm;
  return sign * mag;
}

static int minPwmForSpeed(DisplayManager::SpeedMode s) {
  switch (s) {
    case DisplayManager::SpeedMode::Fast:   return 70;
    case DisplayManager::SpeedMode::Slow:   return 90;
    default: return 70;
  }
}

// ===== Physical button B short/long press =====
static bool btnBWasDown = false;
static unsigned long btnBDownMs = 0;
static const unsigned long BTN_B_LONG_MS = 2000;

static void updateBtnBHold(bool &shortPressEvent, bool &longPressEvent) {
  bool down = (digitalRead(PIN_BTN_B) == LOW);

  shortPressEvent = false;
  longPressEvent = false;

  if (down && !btnBWasDown) btnBDownMs = millis();

  if (!down && btnBWasDown) {
    unsigned long held = millis() - btnBDownMs;
    if (held < BTN_B_LONG_MS) shortPressEvent = true;
  }

  if (down && btnBWasDown) {
    unsigned long held = millis() - btnBDownMs;
    if (held >= BTN_B_LONG_MS) {
      longPressEvent = true;
      btnBDownMs = millis() + 999999; // prevent repeats
    }
  }

  btnBWasDown = down;
}

static void cycleSpeed(DisplayManager::SpeedMode &s) {
  if (s == DisplayManager::SpeedMode::Fast) s = DisplayManager::SpeedMode::Slow;
  else if (s == DisplayManager::SpeedMode::Slow) s = DisplayManager::SpeedMode::Fast;
  else s = DisplayManager::SpeedMode::Slow;
}

void setup() {
  Serial.begin(115200);
  delay(300);

  motors.begin();
  display.begin();
  buttons.begin();
  xbox.begin();
  track.begin(); // <-- NEW

  menu.screen = DisplayManager::UiScreen::Status;
  menu.cursor = DisplayManager::MenuItem::Speed;



  arm.setMode(ServoController::ArmMode::Free);
  arm.begin();
  // keep arm mode synced with menu setting
  arm.setMode(settings.armMode == DisplayManager::ArmMode::Free
              ? ServoController::ArmMode::Free
              : ServoController::ArmMode::Sync);

  // arm {front, back}, wrist{,}, grip {}
  // arm:
  // wrist:
  // grip limits: 
  //    lower -> open grip, upper -> close grip
  arm.setLimits(
    {0, 160}, {0, 160}, {40, 130}
  ); 
  arm.setDefaults({ 130, 130, 60 });
  arm.resetToDefaults(); 



  display.showInitializing();

  lastCmdMs = millis();
  lastUiMs = 0;

  xbox.setBluetoothEnabled(settings.bluetoothOn);
}

void loop() {
  xbox.update();
  buttons.update();

  bool menuBtn = xbox.menuPressed();   // read ONCE per loop

  // IMPORTANT:
  // We must capture whether we were recording/playing *before* we process stop events.
  // Otherwise, a stop press (B short / Menu) can accidentally also open the menu
  // in the same loop iteration.
  const bool busyBefore = track.isRecording() || track.isPlaying();


  // detect X Pressed
  bool xPressed = xbox.xPressed();
  if (xPressed==true) {
      settings.armMode = (settings.armMode == DisplayManager::ArmMode::Free)
                    ? DisplayManager::ArmMode::Sync
                    : DisplayManager::ArmMode::Free;

      arm.setMode(settings.armMode == DisplayManager::ArmMode::Free
              ? ServoController::ArmMode::Free
              : ServoController::ArmMode::Sync);
  }

  bool bShort = false, bLong = false;
  updateBtnBHold(bShort, bLong);

  // ===== GLOBAL STOP (record/play safety) =====
  // While recording or playing: any of these stops immediately.
  const bool stopNow = bLong || menuBtn || bShort;

  // if (track.isRecording() && stopNow) {
  //   track.stopRecording();   // saves to flash
  //   motors.stop();
  //   menu.screen = DisplayManager::UiScreen::Status;
  // }
  // if (track.isPlaying() && stopNow) {
  //   track.stopPlayback();
  //   motors.stop();
  //   menu.screen = DisplayManager::UiScreen::Status;
  // }

  // If we just stopped recording/playback, suppress menu toggling this loop.
  const bool suppressMenuToggle = busyBefore && stopNow;
  const bool wasBusy = track.isRecording() || track.isPlaying();

  // ===== MENU OPEN/CLOSE (only when NOT recording/playing) =====
  if (!wasBusy && !suppressMenuToggle) {
    if (menu.screen == DisplayManager::UiScreen::Status) {
      if (bShort || menuBtn) {
        menu.screen = DisplayManager::UiScreen::MenuMain;
        motors.stop();
      }
    } else {
      if (bLong || menuBtn) {
        menu.screen = DisplayManager::UiScreen::Status;
        motors.stop();
      }
    }
  }

  // ===== MENU NAV + ACTIONS =====
  if (menu.screen == DisplayManager::UiScreen::MenuMain && !wasBusy) {
    // "Select" inside the menu:
    // - Physical Button B short press
    // - Xbox controller A button (edge-triggered)
    const bool selectPressed = bShort || xbox.aPressed();

    // A = NEXT (cursor down)
    if (buttons.pressedA()) {
      int idx = (int)menu.cursor;
      idx = (idx + 1) % (int)DisplayManager::MenuItem::COUNT;
      menu.cursor = (DisplayManager::MenuItem)idx;
    }

    // D-pad up/down = cursor move
    if (xbox.dpadUpPressed()) {
      int idx = (int)menu.cursor - 1;
      if (idx < 0) idx = (int)DisplayManager::MenuItem::COUNT - 1;
      menu.cursor = (DisplayManager::MenuItem)idx;
    }
    if (xbox.dpadDownPressed()) {
      int idx = (int)menu.cursor;
      idx = (idx + 1) % (int)DisplayManager::MenuItem::COUNT;
      menu.cursor = (DisplayManager::MenuItem)idx;
    }

    // ---- Speed edit ----
    if (menu.cursor == DisplayManager::MenuItem::Speed ) {
      if (xbox.dpadLeftPressed() || xbox.dpadRightPressed()) {
        // Fast  <- Slow <- Fast
        // if (settings.speed == DisplayManager::SpeedMode::Fast) settings.speed = DisplayManager::SpeedMode::Slow;
        // if (settings.speed == DisplayManager::SpeedMode::Slow) settings.speed = DisplayManager::SpeedMode::Fast;
        cycleSpeed(settings.speed);
      }
      if (selectPressed) { // physical select cycles speed too
        cycleSpeed(settings.speed);
      }
    }

    // ---- Bluetooth toggle ----
    if (menu.cursor == DisplayManager::MenuItem::Bluetooth) {
      bool changed = false;
      if (xbox.dpadLeftPressed() || xbox.dpadRightPressed() || selectPressed) {
        settings.bluetoothOn = !settings.bluetoothOn;
        changed = true;
      }
      if (changed) {
        xbox.setBluetoothEnabled(settings.bluetoothOn);
        motors.stop();
      }
    }

    // ---- Record Track: B short starts recording, exits menu when starting ----
    if (menu.cursor == DisplayManager::MenuItem::Record) {
      if (selectPressed) {
        track.startRecording(settings.speed);
        menu.screen = DisplayManager::UiScreen::Status;
        motors.stop();
        lastCmdMs = millis();
      }
    }

    // ---- Arm Mode: Free <-> Sync ----
    if (menu.cursor == DisplayManager::MenuItem::ArmMode) {
      if (xbox.dpadLeftPressed() || xbox.dpadRightPressed() || selectPressed) {

        settings.armMode = (settings.armMode == DisplayManager::ArmMode::Free)
                            ? DisplayManager::ArmMode::Sync
                            : DisplayManager::ArmMode::Free;

        arm.setMode(settings.armMode == DisplayManager::ArmMode::Free
                      ? ServoController::ArmMode::Free
                      : ServoController::ArmMode::Sync);
      }
    }

    // ---- Demo Mode: Demo <-> Innactive ----
    if (menu.cursor == DisplayManager::MenuItem::DemoMode) {
      if (xbox.dpadLeftPressed() || xbox.dpadRightPressed() || selectPressed) {

        settings.demoMode = (settings.demoMode == DisplayManager::DemoMode::Demo)
                            ? DisplayManager::DemoMode::Innactive
                            : DisplayManager::DemoMode::Demo;

        
      }
    }



    // ---- Play Track: B short starts playback (if exists), exits menu ----
    if (menu.cursor == DisplayManager::MenuItem::Play) {
      if (selectPressed) {
        bool ok = track.startPlayback();
        if (ok) {
          menu.screen = DisplayManager::UiScreen::Status;
          motors.stop();
          lastCmdMs = millis();
        } else {
          // No track saved. Leave menu open; display will show "No Track".
          motors.stop();
        }
      }
    }

    motors.stop(); // always stop while menu open
  }


  // Only control arm in Status screen, and only when not recording/playing
  if (menu.screen == DisplayManager::UiScreen::Status && xbox.isConnected() && !wasBusy && settings.demoMode == DisplayManager::DemoMode::Innactive) {

    const uint32_t now = millis();
    const uint32_t PERIOD_MS = 60;
    if (now - lastArmControlMs >= PERIOD_MS) {
      lastArmControlMs = now;

      const int ARM_STEP   = 2;
      const int GRIP_STEP  = 2;
      const int WRIST_STEP = 2;

      // LEFT FRONT buttons: LB / LT -> arm
      if (xbox.lbDown()) arm.nudgeArmDeg(+ARM_STEP);   // increase angle (arm backward)
      if (xbox.ltDown()) arm.nudgeArmDeg(-ARM_STEP);   // decrease angle (arm forward)

      // RIGHT FRONT buttons: RB / RT -> gripper
      if (xbox.rbDown()) arm.nudgeGripDeg(-GRIP_STEP); // open (may need sign flip)
      if (xbox.rtDown()) arm.nudgeGripDeg(+GRIP_STEP); // close (may need sign flip)

      // Wrist:
      // Sync: auto-compensate (do nothing here)
      // Free: right stick Y controls wrist
      if (arm.mode() == ServoController::ArmMode::Free) {
        int lx, ly, rx, ry;
        xbox.getRawSticks(lx, ly, rx, ry);

        if (ry > 40)       arm.nudgeWristDeg(+WRIST_STEP);
        else if (ry < -40) arm.nudgeWristDeg(-WRIST_STEP);
      }
    }
  }


  // ===== DRIVE / RECORD / PLAY (Status only) =====
  if (menu.screen == DisplayManager::UiScreen::Status) {

    // If BT disabled, never drive
    if (!settings.bluetoothOn) {
      motors.stop();
      lastCmdMs = millis();
    }
    else if (track.isPlaying()) {
      // PLAYBACK drives the motors
      int left = 0, right = 0;
      if (track.playbackTick(left, right)) {
        motors.setLeftRight(left, right);
        lastCmdMs = millis();
      } else {
        motors.stop();
      }
    }
    else if((settings.demoMode == DisplayManager::DemoMode::Innactive)) {
      // Normal Xbox drive 4 wheels
      int left = 0, right = 0;
      if (xbox.getDriveCommand(left, right)) {
        lastCmdMs = millis();

        float k = speedScale(settings.speed);
        left  = (int)(left  * k);
        right = (int)(right * k);

        int minPwm = minPwmForSpeed(settings.speed);
        left  = applyMinPWM(left,  minPwm);
        right = applyMinPWM(right, minPwm);

        motors.setLeftRight(left, right);

        // If recording, record the ACTUAL commands sent to motors
        if (track.isRecording()) {
          track.recordTick(left, right, settings.speed);
        }
      }

      if (millis() - lastCmdMs > FAILSAFE_MS) {
        motors.stop();
      }
    }
  } else {
    motors.stop();
  }


  // DEMO seaction
  if(settings.demoMode == DisplayManager::DemoMode::Demo)
  {
        if (xPressed==true) {
          //run demo 1
          motors.setLeftRight(0, 200);
          delay(1000);
          motors.setLeftRight(200, 0);
          delay(1000);
          motors.setLeftRight(200, 200);
          delay(1000);
          motors.setLeftRight(-200, 200);
          delay(1000);
          motors.setLeftRight(0, -200);
          delay(1000);
          motors.setLeftRight(-200, 0);
          delay(1000);
          motors.setLeftRight(-200, -200);
          delay(1000);
          motors.setLeftRight(200, -200);
          delay(1000);
          motors.stop();
        }

        if (xbox.dpadUpPressed()){
          moveArm(10,10);
          delay(1000);
          moveArm(-50,30);
          //arm.nudgeGripDeg(15);
          // arm.nudgeWristDeg(15);

        }
  }

  
  // ===== DISPLAY =====
  if (millis() - lastUiMs >= UI_PERIOD_MS) {
    lastUiMs = millis();

    if (menu.screen == DisplayManager::UiScreen::Status) {
      display.showStatus(settings.bluetoothOn, xbox.isConnected(),
                         settings.speed, track.isRecording(), track.isPlaying(), settings.armMode, settings.demoMode);
    } else {
      display.renderMenuMain(settings, menu,
                             track.isRecording(), track.isPlaying(),
                             track.hasSavedTrack());
    }
  }

  delay(5);
}



void moveArm(int degree, int slowness) {
    int increment = (degree > 0) ? 1 : -1;
    for(int i=0; i< abs(degree); i++)
    {
      arm.nudgeArmDeg(increment);
      delay(slowness);
    }
}
