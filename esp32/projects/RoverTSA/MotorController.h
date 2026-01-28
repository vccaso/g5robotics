#pragma once
#include <Arduino.h>

class MotorController {
public:
  MotorController(int in1, int in2, int ena,
                  int in3, int in4, int enb,
                  int pwmChLeft, int pwmChRight,
                  int pwmFreqHz, int pwmResBits);

  void begin();
  void stop();

  // Set left/right speeds in range [-255..255]
  // sign = direction, magnitude = PWM duty
  void setLeftRight(int left, int right);

private:
  int _in1, _in2, _ena;
  int _in3, _in4, _enb;
  int _chL, _chR;
  int _freq, _res;

  void setChannel(int inA, int inB, int pwmPin, int pwmChannel, int value);
  int clamp(int v, int lo, int hi);
};
