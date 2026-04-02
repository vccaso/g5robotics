// XboxDrive.cpp (cleaned + updated)
//
// Features:
// - Bluepad32 Xbox controller support
// - Arcade drive mixing (left/right motor commands)
// - Deadzone + scaling to [-255..255]
// - Raw sticks for future servos
// - Menu button edge-detect (3-lines button): miscButtons mask 0x00000004
// - D-pad edge-detect (Up/Down/Left/Right) using cached state (_dpadNow/_dpadLast)
//
// Notes:
// - During development, BP32.forgetBluetoothKeys() makes pairing easier.
//   Remove it later if you want auto-reconnect after reboot.

#include "XboxDrive.h"
#include "Pins.h"

GamepadPtr XboxDrive::_gamepads[BP32_MAX_GAMEPADS] = { nullptr };

XboxDrive::XboxDrive() {}

void XboxDrive::begin() {
  Serial.println("Initializing Bluepad32...");
  BP32.setup(&XboxDrive::onConnected, &XboxDrive::onDisconnected);

  // Optional while testing pairing. Comment out later for persistent pairing.
  BP32.forgetBluetoothKeys();

  BP32.enableNewBluetoothConnections(true);
  Serial.println("Waiting for Bluetooth controller...");
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

int XboxDrive::applyDeadzoneAndScale(int v, int deadzonePct) {
  // Normalize from approx [-512..512] to [-255..255]
  int scaled = (v * 255) / 512;
  scaled = clamp255(scaled);

  int dz = (255 * deadzonePct) / 100;
  if (abs(scaled) <= dz) return 0;

  // Re-scale remaining range so you still get full power after deadzone
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

static GamepadPtr firstConnectedGamepad(GamepadPtr* pads) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (pads[i] && pads[i]->isConnected()) return pads[i];
  }
  return nullptr;
}

void XboxDrive::update() {
  BP32.update();

  if (!_btEnabled) {
    _leftCmd = 0;
    _rightCmd = 0;
    _hasNewCmd = true;
    _dpadLast = 0;
    _dpadNow = 0;
    _menuWasDown = false;
    return;
  }

  GamepadPtr gp = firstConnectedGamepad(_gamepads);

  if (!gp) {
    _leftCmd = 0;
    _rightCmd = 0;
    _hasNewCmd = true;

    // Reset D-pad state (prevents false edges when reconnecting)
    _dpadLast = 0;
    _dpadNow = 0;
    _aLast = false;
    _aNow = false;
    _xLast = false;
    _xNow = false;

    _menuWasDown = false;
    return;
  }

  // Cache D-pad state ONCE per update (so all directions work)
  _dpadLast = _dpadNow;
  _dpadNow  = gp->dpad();
  // Cache A button ONCE per update (edge-triggered helper)
  _aLast = _aNow;
  _aNow  = gp->a();

  // Cache X button ONCE per update (edge-triggered helper)
  _xLast = _xNow;
  _xNow  = gp->x();

  // Left stick: throttle (Y) and steering (X)
  // Many controllers report up as negative. Invert Y so up = forward.
  int x = applyDeadzoneAndScale(gp->axisX(), DEADZONE);
  int y = applyDeadzoneAndScale(-gp->axisY(), DEADZONE);

  // Gains
  float throttle = y * THROTTLE_GAIN;
  float steer    = x * TURN_GAIN;

  // Arcade mix: left = throttle + steer, right = throttle - steer
  _leftCmd  = clamp255((int)(throttle + steer));
  _rightCmd = clamp255((int)(throttle - steer));
  _hasNewCmd = true;
}

void XboxDrive::getRawSticks(int &lx, int &ly, int &rx, int &ry) {
  lx = ly = rx = ry = 0;

  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  if (!gp) return;

  lx = applyDeadzoneAndScale(gp->axisX(), DEADZONE);
  ly = applyDeadzoneAndScale(-gp->axisY(), DEADZONE);
  rx = applyDeadzoneAndScale(gp->axisRX(), DEADZONE);
  ry = applyDeadzoneAndScale(-gp->axisRY(), DEADZONE);

  lx = clamp255(lx);
  ly = clamp255(ly);
  rx = clamp255(rx);
  ry = clamp255(ry);
}


void XboxDrive::getRawSticks(int &lx, int &ly, int &rx) {
  int ryDummy = 0;
  getRawSticks(lx, ly, rx, ryDummy);
}


// ---- Menu button ("3 lines") edge detect ----
// You measured: miscButtons == 0x00000004 when pressed
bool XboxDrive::menuPressed() {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  if (!gp) {
    _menuWasDown = false;
    return false;
  }

  const uint32_t MENU_MASK_MISC = 0x00000004;

  uint32_t misc = gp->miscButtons();
  bool nowDown = (misc & MENU_MASK_MISC) != 0;
  bool pressedEdge = nowDown && !_menuWasDown;
  _menuWasDown = nowDown;

  return pressedEdge;
}

// ---- D-pad edge detect using cached state (_dpadNow/_dpadLast) ----
static bool edgePressed(uint8_t now, uint8_t last, uint8_t mask) {
  bool nowDown = (now & mask) != 0;
  bool wasDown = (last & mask) != 0;
  return nowDown && !wasDown;
}

bool XboxDrive::dpadUpPressed() {
  const uint8_t UP = 0x01;
  return edgePressed(_dpadNow, _dpadLast, UP);
}

bool XboxDrive::dpadRightPressed() {
  const uint8_t RIGHT = 0x04;
  return edgePressed(_dpadNow, _dpadLast, RIGHT);
}

bool XboxDrive::dpadDownPressed() {
  const uint8_t DOWN = 0x02;
  return edgePressed(_dpadNow, _dpadLast, DOWN);
}

bool XboxDrive::dpadLeftPressed() {
  const uint8_t LEFT = 0x08;
  return edgePressed(_dpadNow, _dpadLast, LEFT);
}

bool XboxDrive::aPressed() {
  // Edge: down now, not down last update
  return _aNow && !_aLast;
}

bool XboxDrive::xPressed() {
  // Edge: down now, not down last update
  return _xNow && !_xLast;
}


void XboxDrive::setBluetoothEnabled(bool enabled) {
  _btEnabled = enabled;
  BP32.enableNewBluetoothConnections(enabled);

  // When turning off, clear commands so the robot won't move
  if (!enabled) {
    _leftCmd = 0;
    _rightCmd = 0;
    _hasNewCmd = true;
    _dpadLast = 0;
    _dpadNow = 0;
    _menuWasDown = false;
  }
}

bool XboxDrive::bluetoothEnabled() const {
  return _btEnabled;
}

bool XboxDrive::dpadUpDown() const {
  const uint8_t UP = 0x01;
  return (_dpadNow & UP) != 0;
}

bool XboxDrive::dpadDownDown() const {
  const uint8_t DOWN = 0x02;
  return (_dpadNow & DOWN) != 0;
}

bool XboxDrive::yDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  return gp ? gp->y() : false;
}

bool XboxDrive::aDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  return gp ? gp->a() : false;
}

bool XboxDrive::lbDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  return gp ? gp->l1() : false;
}

bool XboxDrive::rbDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  return gp ? gp->r1() : false;
}

bool XboxDrive::ltDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  if (!gp) return false;

  // Left trigger is typically "brake" in Bluepad32
  // Treat as pressed if above a small threshold.
  return gp->brake() > 40;
}

bool XboxDrive::rtDown() const {
  GamepadPtr gp = firstConnectedGamepad(_gamepads);
  if (!gp) return false;

  // Right trigger is typically "throttle" in Bluepad32
  return gp->throttle() > 40;
}
