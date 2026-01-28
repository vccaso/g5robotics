#include "ButtonManager.h"

ButtonManager::ButtonManager(int pinA, int pinB)
: _pinA(pinA), _pinB(pinB) {}

void ButtonManager::begin() {
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);
}

void ButtonManager::update() {
  bool nowA = digitalRead(_pinA);
  bool nowB = digitalRead(_pinB);

  _evtA = (_lastA == HIGH && nowA == LOW);
  _evtB = (_lastB == HIGH && nowB == LOW);

  _lastA = nowA;
  _lastB = nowB;
}

bool ButtonManager::pressedA() {
  bool r = _evtA;
  _evtA = false;
  return r;
}

bool ButtonManager::pressedB() {
  bool r = _evtB;
  _evtB = false;
  return r;
}
