#pragma once
#include <Arduino.h>

class DisplayManager {
public:
  enum class UiScreen : uint8_t {
    Boot,
    Status,
    MenuMain
  };

  // menu  has 5 items
  enum class MenuItem : uint8_t {
    Speed = 0,
    Bluetooth = 1,
    ArmMode = 2,
    Record = 3,
    Play = 4,
    COUNT
  };

  enum class SpeedMode : uint8_t { Fast, Slow };
  enum class ArmMode : uint8_t { Free, Sync };

  struct Settings {
    SpeedMode speed = SpeedMode::Fast;
    bool bluetoothOn = true;
    ArmMode armMode = ArmMode::Sync;
  };

  struct MenuState {
    UiScreen screen = UiScreen::Status;
    MenuItem cursor = MenuItem::Speed;
  };



public:
  void begin(int sda = 21, int scl = 22, uint8_t i2cAddr = 0x3C);

  void showInitializing();

  // UPDATED: status now shows speed + BT + REC/PLAY
  void showStatus(bool btEnabled, bool btConnected, SpeedMode speed,
                  bool isRecording, bool isPlaying, ArmMode armMode);

  // UPDATED: menu now renders record/play states + hasTrack
  void renderMenuMain(const Settings& s, const MenuState& m,
                      bool isRecording, bool isPlaying, bool hasTrack);

  void setHint(const String& hint);

private:
  bool _ok = false;
  uint8_t _addr = 0x3C;
  String _hint;

  void clearAndHeader(const __FlashStringHelper* title);

  const char* menuItemLabel(MenuItem item) const;
  const char* speedLabel(SpeedMode v) const;

  const char* armModeLabel(ArmMode v) const;


  void drawRow(uint8_t rowIndex, bool selected, const char* left, const char* right);
};
