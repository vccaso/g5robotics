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

  // D-pad navigation events (true only once per press)
  bool dpadUpPressed();
  bool dpadDownPressed();
  bool dpadLeftPressed();
  bool dpadRightPressed();

  // Face buttons (edge-triggered)
  bool aPressed();      // Xbox "A" button (green)
  bool xPressed();

  bool menuPressed();   // Xbox "3-lines" button, edge-triggered
  void setBluetoothEnabled(bool enabled);
  bool bluetoothEnabled() const;

  // Held (level) states - true while the button is held down
  bool dpadUpDown() const;
  bool dpadDownDown() const;
  bool yDown() const;
  bool aDown() const;
  // --- Front buttons (held states) ---
  bool lbDown() const;        // left bumper
  bool rbDown() const;        // right bumper
  bool ltDown() const;        // left trigger (analog)
  bool rtDown() const;        // right trigger (analog)

  // Raw sticks: add right Y for wrist control in Free mode
  void getRawSticks(int &lx, int &ly, int &rx);        // 3-arg 
  void getRawSticks(int &lx, int &ly, int &rx, int &ry); // 4-arg 


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

  uint8_t _dpadNow = 0;
  uint8_t _dpadLast = 0;

  bool _aNow = false;
  bool _aLast = false;

  bool _xNow = false;
  bool _xLast = false;

  bool _btEnabled = true;
  bool _menuWasDown = false;

};
