#pragma once
#include <Arduino.h>
#include <Bluepad32.h>

class XboxDrive {
public:
  XboxDrive();

  void begin();
  void update(); // call in loop()

  bool isConnected() const;

  // Outputs: left/right in [-255..255], plus a "fresh" flag indicating updated values
  bool getDriveCommand(int &outLeft, int &outRight);

  void getRawSticks(int &lx, int &ly, int &rx);


private:
  static void onConnected(GamepadPtr gp);
  static void onDisconnected(GamepadPtr gp);

  static GamepadPtr _gamepads[BP32_MAX_GAMEPADS];

  int _leftCmd = 0;
  int _rightCmd = 0;
  bool _hasNewCmd = false;

  // Helpers
  int applyDeadzoneAndScale(int v, int deadzonePct);
  int clamp255(int v);
};

