#include "DriveController.h"
#include "Pins.h"

void DriveController::begin() {
  pinMode(M1_EN, OUTPUT);
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);

  pinMode(M2_EN, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);

  pinMode(M3_EN, OUTPUT);
  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);

  pinMode(M4_EN, OUTPUT);
  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);

  stopAll();
}

void DriveController::motorForward(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t speed) {
  speed = constrain(speed, 0, 255);
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enPin, speed);
}

void DriveController::motorBackward(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t speed) {
  speed = constrain(speed, 0, 255);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enPin, speed);
}

void DriveController::motorStop(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enPin, 0);
}

void DriveController::stopAll() {
  motorStop(M1_EN, M1_IN1, M1_IN2);
  motorStop(M2_EN, M2_IN1, M2_IN2);
  motorStop(M3_EN, M3_IN1, M3_IN2);
  motorStop(M4_EN, M4_IN1, M4_IN2);
}

void DriveController::motor1Forward(uint8_t speed) { motorForward(M1_EN, M1_IN1, M1_IN2, speed); }
void DriveController::motor1Backward(uint8_t speed) { motorBackward(M1_EN, M1_IN1, M1_IN2, speed); }

void DriveController::motor2Forward(uint8_t speed) { motorForward(M2_EN, M2_IN1, M2_IN2, speed); }
void DriveController::motor2Backward(uint8_t speed) { motorBackward(M2_EN, M2_IN1, M2_IN2, speed); }

void DriveController::motor3Forward(uint8_t speed) { motorForward(M3_EN, M3_IN1, M3_IN2, speed); }
void DriveController::motor3Backward(uint8_t speed) { motorBackward(M3_EN, M3_IN1, M3_IN2, speed); }

void DriveController::motor4Forward(uint8_t speed) { motorForward(M4_EN, M4_IN1, M4_IN2, speed); }
void DriveController::motor4Backward(uint8_t speed) { motorBackward(M4_EN, M4_IN1, M4_IN2, speed); }

void DriveController::allForward(uint8_t speed) {
  if (speed > 0 && speed < 100) {
    motorForward(M1_EN, M1_IN1, M1_IN2, 100);
    motorForward(M2_EN, M2_IN1, M2_IN2, 100);
    motorForward(M3_EN, M3_IN1, M3_IN2, 100);
    motorForward(M4_EN, M4_IN1, M4_IN2, 100);
    delay(50);
  }
  motor1Forward(speed);
  motor2Forward(speed);
  motor3Forward(speed);
  motor4Forward(speed);
}

void DriveController::allBackward(uint8_t speed) {
  if (speed > 0 && speed < 100) {
    motorBackward(M1_EN, M1_IN1, M1_IN2, 100);
    motorBackward(M2_EN, M2_IN1, M2_IN2, 100);
    motorBackward(M3_EN, M3_IN1, M3_IN2, 100);
    motorBackward(M4_EN, M4_IN1, M4_IN2, 100);
    delay(50);
  }
  motor1Backward(speed);
  motor2Backward(speed);
  motor3Backward(speed);
  motor4Backward(speed);
}

void DriveController::leftSideForward(uint8_t speed) {
  motor1Forward(speed);
  motor3Forward(speed);
}

void DriveController::leftSideBackward(uint8_t speed) {
  motor1Backward(speed);
  motor3Backward(speed);
}

void DriveController::rightSideForward(uint8_t speed) {
  motor2Forward(speed);
  motor4Forward(speed);
}

void DriveController::rightSideBackward(uint8_t speed) {
  motor2Backward(speed);
  motor4Backward(speed);
}