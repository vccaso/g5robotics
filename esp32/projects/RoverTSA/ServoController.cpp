// ServoController.cpp
//
// NOTE: This implementation assumes you update ServoController.h to include:
//   - enum class ArmMode { Free, Sync };
//   - public methods: setMode(), toggleMode(), mode(), resetToDefaults(),
//                     setDefaults(), setLimits(), setArmDeg(), setWristDeg(), setGripDeg()
//   - private state: current/default angles + limits + mode + wristTrim
//
// If you want, I can paste the matching ServoController.h right after this.

#include "ServoController.h"
#include "Pins.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ------------------------- PCA9685 config -------------------------
#ifndef PCA9685_ADDR
// If you later add PCA9685_ADDR to Pins.h, you can remove this fallback.
static const uint8_t PCA9685_ADDR_FALLBACK = 0x40;
#endif

// Channels (fallbacks; prefer defining SERVO_CH_* in Pins.h)
#ifndef SERVO_CH_ARM
static const uint8_t SERVO_CH_ARM_FALLBACK   = 0; // shoulder/elbow
static const uint8_t SERVO_CH_WRIST_FALLBACK = 1; // wrist
static const uint8_t SERVO_CH_GRIP_FALLBACK  = 2; // gripper
#endif

// MG996R typical pulse range (microseconds). You may tune these per-servo later.
static const uint16_t SERVO_US_MIN = 500;
static const uint16_t SERVO_US_MAX = 2500;


int ServoController::armDeg() const   { return _armDeg; }
int ServoController::wristDeg() const { return _wristDeg; }
int ServoController::gripDeg() const  { return _gripDeg; }

void ServoController::nudgeArmDeg(int delta) {
  setArmDeg(_armDeg + delta);
}

void ServoController::nudgeWristDeg(int delta) {
  setWristDeg(_wristDeg + delta);
}

void ServoController::nudgeGripDeg(int delta) {
  setGripDeg(_gripDeg + delta);
}


// PCA9685 has 12-bit (4096) ticks per PWM period.
// At 50 Hz, period is 20,000 us.
static inline uint16_t usToTicks(uint16_t us) {
  // ticks = us * 4096 / 20000
  // Use 32-bit math to avoid overflow.
  uint32_t ticks = (uint32_t)us * 4096UL;
  ticks = (ticks + 10000UL) / 20000UL; // rounded
  if (ticks > 4095) ticks = 4095;
  return (uint16_t)ticks;
}

static inline int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static inline int applyDeadzone(int v, int dz) {
  // v in [-255..255], dz in [0..255]
  if (v > -dz && v < dz) return 0;
  return v;
}

static inline int mapStickToDeg(int v /*-255..255*/, int minDeg, int maxDeg) {
  // Map stick to [minDeg..maxDeg]
  long out = map((long)v, -255L, 255L, (long)minDeg, (long)maxDeg);
  return (int)out;
}

// ------------------------- Driver instance -------------------------
static Adafruit_PWMServoDriver pwm(
#ifdef PCA9685_ADDR
  PCA9685_ADDR
#else
  PCA9685_ADDR_FALLBACK
#endif
);

// ------------------------- Implementation -------------------------

ServoController::ServoController()
: _mode(ArmMode::Free),
  _wristTrimDeg(0),
  _armCh(
#ifdef SERVO_CH_ARM
    SERVO_CH_ARM
#else
    SERVO_CH_ARM_FALLBACK
#endif
  ),
  _wristCh(
#ifdef SERVO_CH_WRIST
    SERVO_CH_WRIST
#else
    SERVO_CH_WRIST_FALLBACK
#endif
  ),
  _gripCh(
#ifdef SERVO_CH_GRIP
    SERVO_CH_GRIP
#else
    SERVO_CH_GRIP_FALLBACK
#endif
  )
{
  // Defaults (safe neutral-ish)
  _armDefaultDeg   = 110;  //<90 to the front
  _wristDefaultDeg = 90;  
  _gripDefaultDeg  = 60;

  // Current = defaults at boot
  _armDeg   = _armDefaultDeg;
  _wristDeg = _wristDefaultDeg;
  _gripDeg  = _gripDefaultDeg;

  // Conservative limits (change with setLimits())
  _armMinDeg   = 0;   _armMaxDeg   = 180;
  _wristMinDeg = 0;   _wristMaxDeg = 180;
  _gripMinDeg  = 0;   _gripMaxDeg  = 180;
}

void ServoController::begin() {
  // Ensure I2C pins are correct (OLED + PCA9685 share the bus)
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  pwm.begin();
  pwm.setPWMFreq(50);  // standard servo frequency
  delay(10);

  // Go to a known pose at startup
  resetToDefaults();
}

ServoController::ArmMode ServoController::mode() const {
  return _mode;
}

void ServoController::setMode(ArmMode m) {
  if (_mode == m) return;
  _mode = m;

  // When entering Sync, recompute wrist to match current arm position.
  if (_mode == ArmMode::Sync) {
    _wristTrimDeg = 0;
    _wristDeg = computeSyncWristDeg(_armDeg);
    writeServo(_wristCh, _wristDeg, _wristMinDeg, _wristMaxDeg);
  }
}

void ServoController::toggleMode() {
  setMode((_mode == ArmMode::Free) ? ArmMode::Sync : ArmMode::Free);
}

void ServoController::setDefaults(int armDeg, int wristDeg, int gripDeg) {
  _armDefaultDeg   = clampInt(armDeg,   0, 180);
  _wristDefaultDeg = clampInt(wristDeg, 0, 180);
  _gripDefaultDeg  = clampInt(gripDeg,  0, 180);
}

void ServoController::setLimits(
  int armMinDeg,   int armMaxDeg,
  int wristMinDeg, int wristMaxDeg,
  int gripMinDeg,  int gripMaxDeg
) {
  // Basic sanity (swap if user passed reversed)
  if (armMinDeg > armMaxDeg)   { int t = armMinDeg;   armMinDeg = armMaxDeg;   armMaxDeg = t; }
  if (wristMinDeg > wristMaxDeg){ int t = wristMinDeg; wristMinDeg = wristMaxDeg; wristMaxDeg = t; }
  if (gripMinDeg > gripMaxDeg) { int t = gripMinDeg;  gripMinDeg = gripMaxDeg;  gripMaxDeg = t; }

  _armMinDeg   = clampInt(armMinDeg,   0, 180);
  _armMaxDeg   = clampInt(armMaxDeg,   0, 180);
  _wristMinDeg = clampInt(wristMinDeg, 0, 180);
  _wristMaxDeg = clampInt(wristMaxDeg, 0, 180);
  _gripMinDeg  = clampInt(gripMinDeg,  0, 180);
  _gripMaxDeg  = clampInt(gripMaxDeg,  0, 180);

  // Re-apply clamps to current pose (and rewrite)
  setArmDeg(_armDeg);
  setWristDeg(_wristDeg);
  setGripDeg(_gripDeg);
}

void ServoController::resetToDefaults() {
  _wristTrimDeg = 0;

  _armDeg  = clampInt(_armDefaultDeg,   _armMinDeg,   _armMaxDeg);
  _gripDeg = clampInt(_gripDefaultDeg,  _gripMinDeg,  _gripMaxDeg);

  writeServo(_armCh,  _armDeg,  _armMinDeg,  _armMaxDeg);
  writeServo(_gripCh, _gripDeg, _gripMinDeg, _gripMaxDeg);

  if (_mode == ArmMode::Sync) {
    _wristDeg = computeSyncWristDeg(_armDeg);
  } else {
    _wristDeg = clampInt(_wristDefaultDeg, _wristMinDeg, _wristMaxDeg);
  }
  writeServo(_wristCh, _wristDeg, _wristMinDeg, _wristMaxDeg);
}

void ServoController::setArmDeg(int deg) {
  _armDeg = clampInt(deg, _armMinDeg, _armMaxDeg);
  writeServo(_armCh, _armDeg, _armMinDeg, _armMaxDeg);

  if (_mode == ArmMode::Sync) {
    _wristDeg = computeSyncWristDeg(_armDeg);
    writeServo(_wristCh, _wristDeg, _wristMinDeg, _wristMaxDeg);
  }
}

void ServoController::setWristDeg(int deg) {
  if (_mode == ArmMode::Free) {
    _wristDeg = clampInt(deg, _wristMinDeg, _wristMaxDeg);
    writeServo(_wristCh, _wristDeg, _wristMinDeg, _wristMaxDeg);
    return;
  }

  // Sync mode: wrist command becomes a "trim" around the computed sync wrist.
  int base = computeSyncWristDeg(_armDeg, /*includeTrim=*/false);
  int desired = clampInt(deg, _wristMinDeg, _wristMaxDeg);
  _wristTrimDeg = desired - base;

  // Apply trim immediately
  _wristDeg = computeSyncWristDeg(_armDeg, /*includeTrim=*/true);
  writeServo(_wristCh, _wristDeg, _wristMinDeg, _wristMaxDeg);
}

void ServoController::setGripDeg(int deg) {
  _gripDeg = clampInt(deg, _gripMinDeg, _gripMaxDeg);
  writeServo(_gripCh, _gripDeg, _gripMinDeg, _gripMaxDeg);
}

void ServoController::setPoseDeg(int armDeg, int wristDeg, int gripDeg) {
  setArmDeg(armDeg);
  setWristDeg(wristDeg);
  setGripDeg(gripDeg);
}

int ServoController::computeSyncWristDeg(int armDeg, bool includeTrim) const {
  // Sync logic:
  // wrist = wristDefault - (arm - armDefault) + trim
  int delta = armDeg - _armDefaultDeg;
  int wrist = _wristDefaultDeg + delta;
  if (includeTrim) wrist += _wristTrimDeg;
  wrist = clampInt(wrist, _wristMinDeg, _wristMaxDeg);
  return wrist;
}

// This is just a temporary mapping so you can test today.
// We'll change button/stick mapping later when you decide controls.
void ServoController::updateFromXbox(int lx, int ly, int rx) {
  lx = applyDeadzone(lx, DEADZONE);
  ly = applyDeadzone(ly, DEADZONE);
  rx = applyDeadzone(rx, DEADZONE);

  // Suggested temporary behavior:
  //  - ly controls ARM (big joint)
  //  - rx controls GRIP
  //  - lx controls WRIST in Free mode, or WRIST TRIM in Sync mode
  int arm  = mapStickToDeg(ly, _armMinDeg, _armMaxDeg);
  int grip = mapStickToDeg(rx, _gripMinDeg, _gripMaxDeg);

  setArmDeg(arm);
  setGripDeg(grip);

  int wrist = mapStickToDeg(lx, _wristMinDeg, _wristMaxDeg);
  setWristDeg(wrist);
}

void ServoController::writeServo(uint8_t channel, int deg, int minDeg, int maxDeg) {
  deg = clampInt(deg, minDeg, maxDeg);

  // Convert deg -> microseconds -> PCA ticks
  uint16_t us = (uint16_t)map(deg, 0, 180, SERVO_US_MIN, SERVO_US_MAX);
  uint16_t ticks = usToTicks(us);

  pwm.setPWM(channel, 0, ticks);
}

void ServoController::setDefaults(PoseDeg p) {
  setDefaults(p.arm, p.wrist, p.grip);
}

void ServoController::setLimits(LimitsDeg arm, LimitsDeg wrist, LimitsDeg grip) {
  setLimits(arm.minDeg, arm.maxDeg,
            wrist.minDeg, wrist.maxDeg,
            grip.minDeg, grip.maxDeg);
}
