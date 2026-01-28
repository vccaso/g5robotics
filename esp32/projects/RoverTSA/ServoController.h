#pragma once
#include <Arduino.h>

class ServoController {
public:
  void begin();
  void updateFromXbox(int lx, int ly, int rx);

private:
  void setServo(uint8_t channel, int angle);
};
