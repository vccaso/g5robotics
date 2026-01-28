// RoverTSA.ino
// Main sketch for ESP32 + L298N (4 motors as 2 sides) + Xbox (Bluepad32) + OLED + PCA9685 servos + 2 buttons

#include <Arduino.h>

#include "Pins.h"
#include "MotorController.h"
#include "XboxDrive.h"
#include "DisplayManager.h"
#include "ButtonManager.h"
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
ButtonManager buttons(PIN_BTN_A, PIN_BTN_B);
ServoController servos;

// ===== Failsafe =====
static unsigned long lastCmdMs = 0;
static const unsigned long FAILSAFE_MS = 500;   // stop motors if no cmd in 0.5s
static unsigned long lastUiMs = 0;
static const unsigned long UI_PERIOD_MS = 250;  // update display 4x/sec

void setup() {
  Serial.begin(115200);
  delay(300);

  // Start subsystems
  motors.begin();
  display.begin();
  buttons.begin();
  servos.begin();
  xbox.begin();

  // Boot UI
  display.showInitializing();

  Serial.println("RoverTSA started.");
  Serial.println("Pair Xbox controller via Bluetooth (Bluepad32).");

  lastCmdMs = millis();
  lastUiMs = 0;
}

void loop() {
  // Update inputs
  xbox.update();
  buttons.update();

  // ----- Drive (motors) -----
  int left = 0, right = 0;
  if (xbox.getDriveCommand(left, right)) {
    lastCmdMs = millis();
    motors.setLeftRight(left, right);
  }

  // Failsafe stop if controller freezes/disconnects
  if (millis() - lastCmdMs > FAILSAFE_MS) {
    motors.stop();
  }

  // ----- Servos (from Xbox sticks) -----
  // We use raw sticks for the arm mapping (adjust later as you design arm)
  int lx = 0, ly = 0, rx = 0;
  xbox.getRawSticks(lx, ly, rx);
  servos.updateFromXbox(lx, ly, rx);

  // ----- Buttons (settings later) -----
  // Placeholder: just prints for now
  if (buttons.pressedA()) {
    Serial.println("Button A pressed (settings placeholder)");
  }
  if (buttons.pressedB()) {
    Serial.println("Button B pressed (settings placeholder)");
  }

  // ----- Display -----
  if (millis() - lastUiMs >= UI_PERIOD_MS) {
    lastUiMs = millis();
    display.showReady(xbox.isConnected());
  }

  delay(5);
}
