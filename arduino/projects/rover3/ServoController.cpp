#include "ServoController.h"
#include "Pins.h"

ServoController::ServoController(uint8_t i2cAddress)
  : _pca(i2cAddress) {
}

void ServoController::begin() {
  Wire.begin();
  _pca.begin();
  _pca.setPWMFreq(50);   // Standard servo frequency
  delay(200);
}

uint16_t ServoController::angleToPulse(uint8_t angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
}

void ServoController::setAngle(uint8_t channel, uint8_t angle) {
  uint16_t pulse = angleToPulse(angle);
  _pca.setPWM(channel, 0, pulse);
}

void ServoController::setFrontLeft(uint8_t angle) {
  setAngle(SERVO_CH_FL, angle);
}

void ServoController::setFrontRight(uint8_t angle) {
  setAngle(SERVO_CH_FR, angle);
}

void ServoController::setRearLeft(uint8_t angle) {
  setAngle(SERVO_CH_RL, angle);
}

void ServoController::setRearRight(uint8_t angle) {
  setAngle(SERVO_CH_RR, angle);
}

void ServoController::setAll(uint8_t angle) {
  setAngles(angle, angle, angle, angle);
}

void ServoController::setAllTo90() {
  setAll(90);
}

void ServoController::centerAll() {
  setAllTo90();
}

void ServoController::setAngles(uint8_t fl, uint8_t fr, uint8_t rl, uint8_t rr) {
  setFrontLeft(fl);
  setFrontRight(fr);
  setRearLeft(rl);
  setRearRight(rr);
}

void ServoController::crabLeft45() {
  setAll(45);
}

void ServoController::crabRight45() {
  setAll(135);
}

void ServoController::turnLeft90() {
  setAll(0);
}

void ServoController::turnRight90() {
  setAll(180);
}

void ServoController::spin() {
  setAngles(45, 135, 45, 135);
}

void ServoController::spinTurnCW() {
  setAngles(45, 135, 135, 45);
}

void ServoController::spinTurnCCW() {
  setAngles(135, 45, 45, 135);
}

void ServoController::steerCurveLeft() {
  setAngles(60, 120, 120, 60);
}

void ServoController::steerCurveRight() {
  setAngles(120, 60, 60, 120);
}