#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class ServoController {
public:
  ServoController(uint8_t i2cAddress = 0x40);

  void begin();

  void setAllTo90();
  void centerAll();

  void setFrontLeft(uint8_t angle);
  void setFrontRight(uint8_t angle);
  void setRearLeft(uint8_t angle);
  void setRearRight(uint8_t angle);

  void setAll(uint8_t angle);
  void setAngles(uint8_t fl, uint8_t fr, uint8_t rl, uint8_t rr);

  void crabLeft45();
  void crabRight45();
  void turnLeft90();
  void turnRight90();
  void spinTurnCW();
  void spinTurnCCW();
  void spin();
  void steerCurveLeft();
  void steerCurveRight();

private:
  Adafruit_PWMServoDriver _pca;

  static const uint16_t SERVO_MIN_PULSE = 205; // ~1000us
  static const uint16_t SERVO_MAX_PULSE = 410; // ~2000us

  uint16_t angleToPulse(uint8_t angle);
  void setAngle(uint8_t channel, uint8_t angle);
};