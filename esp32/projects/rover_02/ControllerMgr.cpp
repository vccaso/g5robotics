#include "ControllerMgr.h"
#include <Bluepad32.h>
#include "Config.h"     // uses SERVO_CENTER, SERVO_RANGE, SERVO_MIN/MAX if you have them

// ---------- DEBUG ----------
#ifndef CTRL_DEBUG
#define CTRL_DEBUG 1
#endif

#if CTRL_DEBUG
  #define CDBG(...) Serial.printf(__VA_ARGS__)
#else
  #define CDBG(...)
#endif

// ---------- Bluepad32 ----------
static ControllerPtr sCtl = nullptr;

// Helpers
static float deadband(float x, float d) { return (fabsf(x) < d) ? 0.0f : x; }
static float normAxis(int v) {
  v = constrain(v, -512, 512);
  return (float)v / 512.0f; // -1..+1
}
static float normTrigger(int v) {
  if (v <= 0) return 0.0f;
  if (v > 255) return constrain(v / 1023.0f, 0.0f, 1.0f);
  return constrain(v / 255.0f, 0.0f, 1.0f);
}

static void onConnectedController(ControllerPtr ctl) {
  sCtl = ctl;
  Serial.println("✅ Controller connected");
}

static void onDisconnectedController(ControllerPtr ctl) {
  Serial.println("❌ Controller disconnected");
  if (sCtl == ctl) sCtl = nullptr;
}

void controllerBegin() {
  // Bluepad32 setup requires callbacks
  BP32.setup(onConnectedController, onDisconnectedController);
  Serial.println("ControllerMgr: ready (Bluepad32)");
}

void controllerForgetKeys() {
  Serial.println("⚠️ Forgetting Bluetooth keys (pairings)...");
  BP32.forgetBluetoothKeys();
}

bool controllerIsConnected() {
  return (sCtl && sCtl->isConnected() && sCtl->isGamepad());
}

ControllerState controllerUpdate() {
  ControllerState st;
  BP32.update();

  if (!controllerIsConnected()) {
    // Not connected: return safe defaults (all zeros)
    st.connected = false;
    st.servoTarget = SERVO_CENTER;
    return st;
  }

  st.connected = true;

  GamepadPtr gp = (GamepadPtr)sCtl;

  // ----- Left stick: mecanum translation -----
  float lx = deadband(normAxis(gp->axisX()), 0.08f);
  float ly = deadband(normAxis(gp->axisY()), 0.08f);

  st.vx = -ly;  // forward positive
  st.vy =  lx;  // left positive

  // ----- Rotation: bumpers + triggers -----
  float w = 0;
  if (gp->l1()) w += 0.6f;
  if (gp->r1()) w -= 0.6f;

  float lt = normTrigger(gp->brake());
  float rt = normTrigger(gp->throttle());
  w += lt * 0.8f;
  w -= rt * 0.8f;
  st.w = constrain(w, -1.0f, 1.0f);

  // ----- Right stick X: servo pan -----
  float rx = deadband(normAxis(gp->axisRX()), 0.08f);
  int target = SERVO_CENTER + (int)(rx * SERVO_RANGE);
  target = constrain(target, SERVO_MIN, SERVO_MAX);
  st.servoTarget = target;

  // ----- Buttons -----
  st.estop = gp->x();
  st.turbo = gp->a();

  st.maxPWM = st.turbo ? 230 : 190;

  // Optional quick actions:
  if (gp->b()) st.servoTarget = SERVO_MIN;      // hard-left
  if (gp->y()) st.servoTarget = SERVO_CENTER;   // center

  // ----- Debug line every ~200ms -----
  static uint32_t lastLog = 0;
  if (millis() - lastLog > 200) {
    lastLog = millis();
    CDBG("CTL lx:%d ly:%d rx:%d | LT:%d RT:%d | vx:%.2f vy:%.2f w:%.2f | pwm:%d | servo:%d | X(estop):%d\n",
         gp->axisX(), gp->axisY(), gp->axisRX(),
         gp->brake(), gp->throttle(),
         st.vx, st.vy, st.w, st.maxPWM, st.servoTarget, (int)st.estop);
  }

  return st;
}
