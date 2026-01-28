#pragma once
#include <Arduino.h>

class DisplayManager {
public:
  void begin();
  void showInitializing();
  void showReady(bool btConnected);
};
