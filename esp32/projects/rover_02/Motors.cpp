#include "Motors.h"


// Set to true to invert that motor direction
static const bool INV_FL = false;
static const bool INV_FR = false;
static const bool INV_RL = false;
static const bool INV_RR = false;


Motor mRR = { L1_ENA, L1_IN1, L1_IN2, CH_RR };
Motor mRL = { L1_ENB, L1_IN3, L1_IN4, CH_RL };
Motor mFR = { L2_ENA, L2_IN1, L2_IN2, CH_FR };
Motor mFL = { L2_ENB, L2_IN3, L2_IN4, CH_FL };

static int curFL = 0;
static int curFR = 0;
static int curRL = 0;
static int curRR = 0;

static int applyInvert(int speed, bool inv) {
  return inv ? -speed : speed;
}

static int rampSpeed(int current, int target) {
  int diff = target - current;
  if (diff > RAMP_MAX_STEP) diff = RAMP_MAX_STEP;
  else if (diff < -RAMP_MAX_STEP) diff = -RAMP_MAX_STEP;
  return current + diff;
}

static void setPWM(const Motor& m, int pwmVal) {
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcWrite(m.en, pwmVal);
#else
  ledcWrite(m.ch, pwmVal);
#endif
}

static void motorWrite(const Motor& m, int speed) {
  speed = constrain(speed, -PWM_MAX, PWM_MAX);

  if (speed == 0) {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, LOW);
    setPWM(m, 0);
    return;
  }
  if (speed > 0) {
    digitalWrite(m.in1, HIGH);
    digitalWrite(m.in2, LOW);
    setPWM(m, speed);
  } else {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, HIGH);
    setPWM(m, -speed);
  }
}

static void motorSetup(const Motor& m) {
  pinMode(m.in1, OUTPUT);
  pinMode(m.in2, OUTPUT);
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttach(m.en, PWM_FREQ, PWM_RES);
#else
  ledcSetup(m.ch, PWM_FREQ, PWM_RES);
  ledcAttachPin(m.en, m.ch);
#endif
  motorWrite(m, 0);
}

void motorsBegin() {
  motorSetup(mFL);
  motorSetup(mFR);
  motorSetup(mRL);
  motorSetup(mRR);
}

void stopAllMotors() {
  curFL = 0;
  curFR = 0;
  curRL = 0;
  curRR = 0;
  motorWrite(mFL, 0);
  motorWrite(mFR, 0);
  motorWrite(mRL, 0);
  motorWrite(mRR, 0);
}

void driveMecanum(float vx, float vy, float w, int maxPWM) {
  float fl = vx + vy + w;
  float fr = vx - vy - w;
  float rl = vx - vy + w;
  float rr = vx + vy - w;

  float maxMag = max(max(fabsf(fl), fabsf(fr)), max(fabsf(rl), fabsf(rr)));
  if (maxMag > 1.0f) { fl/=maxMag; fr/=maxMag; rl/=maxMag; rr/=maxMag; }

  int targetFL = applyInvert((int)(fl * maxPWM), INV_FL);
  int targetFR = applyInvert((int)(fr * maxPWM), INV_FR);
  int targetRL = applyInvert((int)(rl * maxPWM), INV_RL);
  int targetRR = applyInvert((int)(rr * maxPWM), INV_RR);

  curFL = rampSpeed(curFL, targetFL);
  curFR = rampSpeed(curFR, targetFR);
  curRL = rampSpeed(curRL, targetRL);
  curRR = rampSpeed(curRR, targetRR);

  motorWrite(mFL, curFL);
  motorWrite(mFR, curFR);
  motorWrite(mRL, curRL);
  motorWrite(mRR, curRR);
}
