#pragma once
#include <Arduino.h>

class ButtonManager {
public:
  ButtonManager(int pinA, int pinB);

  void begin();
  void update();

  bool pressedA();
  bool pressedB();

private:
  int _pinA, _pinB;
  bool _lastA = HIGH;
  bool _lastB = HIGH;
  bool _evtA = false;
  bool _evtB = false;
};
