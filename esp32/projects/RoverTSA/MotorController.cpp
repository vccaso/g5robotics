#include "MotorController.h"

MotorController::MotorController(int in1, int in2, int ena,
                                 int in3, int in4, int enb,
                                 int pwmChLeft, int pwmChRight,
                                 int pwmFreqHz, int pwmResBits)
: _in1(in1), _in2(in2), _ena(ena),
  _in3(in3), _in4(in4), _enb(enb),
  _chL(pwmChLeft), _chR(pwmChRight),
  _freq(pwmFreqHz), _res(pwmResBits) {}

void MotorController::begin() {
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);

  // ESP32 LEDC PWM setup
  ledcSetup(_chL, _freq, _res);
  ledcSetup(_chR, _freq, _res);
  ledcAttachPin(_ena, _chL);
  ledcAttachPin(_enb, _chR);

  stop();
}

void MotorController::stop() {
  // Brake/coast choice:
  // Coast: INx LOW, PWM 0
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, LOW);
  ledcWrite(_chL, 0);
  ledcWrite(_chR, 0);
}

int MotorController::clamp(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void MotorController::setChannel(int inA, int inB, int /*pwmPin*/, int pwmChannel, int value) {
  value = clamp(value, -255, 255);

  int pwm = abs(value);

  if (value > 0) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
  } else if (value < 0) {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
  } else {
    // Coast
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }

  ledcWrite(pwmChannel, pwm);
}

void MotorController::setLeftRight(int left, int right) {
  left = clamp(left, -255, 255);
  right = clamp(right, -255, 255);

  setChannel(_in1, _in2, _ena, _chL, left);
  setChannel(_in3, _in4, _enb, _chR, right);
}
