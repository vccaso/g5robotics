// XboxDrive.cpp (updated)
// - Keeps your existing drive mixing logic
// - Adds a robust getRawSticks() implementation (with safe scaling + deadzone)
// - Ensures axis values stay in [-255..255] even if controller reports different ranges
// NOTE: You MUST also declare getRawSticks() in XboxDrive.h as discussed.

#include "XboxDrive.h"
#include "Pins.h"

GamepadPtr XboxDrive::_gamepads[BP32_MAX_GAMEPADS] = { nullptr };

XboxDrive::XboxDrive() {}

void XboxDrive::begin() {
  BP32.setup(&XboxDrive::onConnected, &XboxDrive::onDisconnected);

  // Optional: make BT pairing easier
  BP32.forgetBluetoothKeys();

  // Allow new connections
  BP32.enableNewBluetoothConnections(true);
}

void XboxDrive::onConnected(GamepadPtr gp) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (_gamepads[i] == nullptr) {
      _gamepads[i] = gp;
      break;
    }
  }
}

void XboxDrive::onDisconnected(GamepadPtr gp) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (_gamepads[i] == gp) {
      _gamepads[i] = nullptr;
      break;
    }
  }
}

bool XboxDrive::isConnected() const {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (_gamepads[i] && _gamepads[i]->isConnected()) return true;
  }
  return false;
}

int XboxDrive::clamp255(int v) {
  if (v < -255) return -255;
  if (v > 255) return 255;
  return v;
}

// Bluepad32 axes are typically in range [-512..512] (varies by controller).
// We'll normalize to [-255..255] and apply a deadzone in percent.
int XboxDrive::applyDeadzoneAndScale(int v, int deadzonePct) {
  // Normalize from approx [-512..512] to [-255..255]
  int scaled = (v * 255) / 512;
  scaled = clamp255(scaled);

  int dz = (255 * deadzonePct) / 100;
  if (abs(scaled) <= dz) return 0;

  // Re-scale remaining range to keep full throw after deadzone
  int sign = (scaled >= 0) ? 1 : -1;
  int mag = abs(scaled) - dz;
  int out = (mag * 255) / (255 - dz);
  return clamp255(sign * out);
}

bool XboxDrive::getDriveCommand(int &outLeft, int &outRight) {
  if (!_hasNewCmd) return false;
  _hasNewCmd = false;
  outLeft = _leftCmd;
  outRight = _rightCmd;
  return true;
}

void XboxDrive::update() {
  BP32.update();

  GamepadPtr gp = nullptr;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (_gamepads[i] && _gamepads[i]->isConnected()) {
      gp = _gamepads[i];
      break;
    }
  }

  if (!gp) {
    _leftCmd = 0;
    _rightCmd = 0;
    _hasNewCmd = true;
    return;
  }

  // Left stick: throttle (Y) and steering (X)
  // Many controllers report up as negative. Invert Y so up = forward.
  int rawX = gp->axisX();
  int rawY = gp->axisY();

  int x = applyDeadzoneAndScale(rawX, DEADZONE);
  int y = applyDeadzoneAndScale(-rawY, DEADZONE);

  // Gains
  float throttle = y * THROTTLE_GAIN;
  float steer = x * TURN_GAIN;

  // Arcade mix: left = throttle + steer, right = throttle - steer
  int left = (int)(throttle + steer);
  int right = (int)(throttle - steer);

  _leftCmd = clamp255(left);
  _rightCmd = clamp255(right);
  _hasNewCmd = true;
}

// -------- Servo stick helpers --------
// Returns stick values normalized to [-255..255] with the same deadzone logic.
// This makes servo control feel consistent with drive control.
void XboxDrive::getRawSticks(int &lx, int &ly, int &rx) {
  lx = ly = rx = 0;

  GamepadPtr gp = nullptr;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (_gamepads[i] && _gamepads[i]->isConnected()) {
      gp = _gamepads[i];
      break;
    }
  }
  if (!gp) return;

  // Normalize & apply deadzone (same behavior as drive)
  // Invert LY so up = positive
  lx = applyDeadzoneAndScale(gp->axisX(), DEADZONE);
  ly = applyDeadzoneAndScale(-gp->axisY(), DEADZONE);

  // Right stick X (some controllers may report smaller ranges—scaler handles it)
  rx = applyDeadzoneAndScale(gp->axisRX(), DEADZONE);

  // Safety clamp (extra)
  lx = clamp255(lx);
  ly = clamp255(ly);
  rx = clamp255(rx);
}
