#pragma once
#include <Arduino.h>

class DriveController {
public:
  void begin();

  void stopAll();

  void motor1Forward(uint8_t speed);
  void motor1Backward(uint8_t speed);
  void motor2Forward(uint8_t speed);
  void motor2Backward(uint8_t speed);
  void motor3Forward(uint8_t speed);
  void motor3Backward(uint8_t speed);
  void motor4Forward(uint8_t speed);
  void motor4Backward(uint8_t speed);

  void allForward(uint8_t speed);
  void allBackward(uint8_t speed);

  void leftSideForward(uint8_t speed);
  void leftSideBackward(uint8_t speed);
  void rightSideForward(uint8_t speed);
  void rightSideBackward(uint8_t speed);

private:
  void motorForward(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t speed);
  void motorBackward(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t speed);
  void motorStop(uint8_t enPin, uint8_t in1Pin, uint8_t in2Pin);
};