#pragma once
#include <Arduino.h>

class ServoController {
public:
  enum class ArmMode { Free, Sync };

  struct PoseDeg {
    int arm;
    int wrist;
    int grip;
  };

  struct LimitsDeg {
    int minDeg;
    int maxDeg;
  };

  ServoController();

  void begin();

  // Mode
  void setMode(ArmMode m);
  void toggleMode();
  ArmMode mode() const;

  // Defaults / Reset
  void setDefaults(int armDeg, int wristDeg, int gripDeg);
  void setDefaults(PoseDeg p);                 // <-- NEW overload
  void resetToDefaults();

  // Limits
  void setLimits(int armMinDeg, int armMaxDeg,
                 int wristMinDeg, int wristMaxDeg,
                 int gripMinDeg, int gripMaxDeg);

  void setLimits(LimitsDeg arm, LimitsDeg wrist, LimitsDeg grip);  // <-- NEW overload

  // Joint control
  void setArmDeg(int deg);
  void setWristDeg(int deg);
  void setGripDeg(int deg);
  void setPoseDeg(int armDeg, int wristDeg, int gripDeg);

  // Temporary Xbox hook
  void updateFromXbox(int lx, int ly, int rx);

  int armDeg() const;
  int wristDeg() const;
  int gripDeg() const;

  void nudgeArmDeg(int delta);
  void nudgeWristDeg(int delta);
  void nudgeGripDeg(int delta);


private:
  int computeSyncWristDeg(int armDeg, bool includeTrim = true) const;
  void writeServo(uint8_t channel, int deg, int minDeg, int maxDeg);

  ArmMode _mode;

  int _armDeg, _wristDeg, _gripDeg;
  int _armDefaultDeg, _wristDefaultDeg, _gripDefaultDeg;

  int _armMinDeg, _armMaxDeg;
  int _wristMinDeg, _wristMaxDeg;
  int _gripMinDeg, _gripMaxDeg;

  int _wristTrimDeg;

  uint8_t _armCh, _wristCh, _gripCh;

  static const int DEADZONE = 15;
};
