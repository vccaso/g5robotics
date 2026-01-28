#include "ServoController.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

static Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Typical SG90 range
static const uint16_t SERVO_MIN = 110;  // ~500us
static const uint16_t SERVO_MAX = 510;  // ~2500us

void ServoController::begin() {
  pwm.begin();
  pwm.setPWMFreq(50); // Servo frequency
  delay(10);
}

void ServoController::setServo(uint8_t channel, int angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(channel, 0, pulse);
}

void ServoController::updateFromXbox(int lx, int ly, int rx) {
  // lx, ly, rx expected range: -255 .. +255

  // Example mapping:
  // Servo 0: arm base (left stick X)
  // Servo 1: arm shoulder (left stick Y)
  // Servo 2: arm claw (right stick X)

  setServo(0, map(lx, -255, 255, 0, 180));
  setServo(1, map(ly, -255, 255, 20, 160));
  setServo(2, map(rx, -255, 255, 60, 120));
}
