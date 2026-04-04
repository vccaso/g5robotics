#include "DisplayManager.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 16x16 Bluetooth icon - ON (clean + centered)
static const uint8_t bt_icon_on_16[] PROGMEM = {
  0x00,0x00,
  0x18,0x00,
  0x1C,0x00,
  0x1A,0x00,
  0x19,0x00,
  0x58,0x80,
  0xB8,0x40,
  0x58,0x80,
  0x19,0x00,
  0x1A,0x00,
  0x1C,0x00,
  0x18,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00
};

// 16x16 Bluetooth icon - OFF (same icon + slash)
static const uint8_t bt_icon_off_16[] PROGMEM = {
  0x00,0x00,
  0x18,0x00,
  0x1C,0x00,
  0x1A,0x00,
  0x19,0x00,
  0x58,0x80,
  0xB8,0x40,
  0x58,0x80,
  0x19,0x00,
  0x1A,0x00,
  0x1C,0x00,
  0x18,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00
};

// Optional: draw a "slash" overlay for OFF to make it unmistakable.
// We overlay this on top of bt_icon_on_16 instead of using bt_icon_off_16.
// 16x16 diagonal slash (top-left to bottom-right)
static const uint8_t slash_16[] PROGMEM = {
  0x01,0x00,
  0x02,0x00,
  0x04,0x00,
  0x08,0x00,
  0x10,0x00,
  0x20,0x00,
  0x40,0x00,
  0x80,0x00,
  0x40,0x00,
  0x20,0x00,
  0x10,0x00,
  0x08,0x00,
  0x04,0x00,
  0x02,0x00,
  0x01,0x00,
  0x00,0x00
};



void DisplayManager::begin(int sda, int scl, uint8_t i2cAddr) {
  _addr = i2cAddr;
  Wire.begin(sda, scl);

  _ok = display.begin(SSD1306_SWITCHCAPVCC, _addr);
  if (!_ok) {
    Serial.println("OLED init failed");
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void DisplayManager::setHint(const String& hint) {
  _hint = hint;
}

void DisplayManager::clearAndHeader(const __FlashStringHelper* title) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(title);
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);
}

const char* DisplayManager::menuItemLabel(MenuItem item) const {
  switch (item) {
    case MenuItem::Speed:     return "Drive Speed";
    case MenuItem::Bluetooth: return "Bluetooth";
    // case MenuItem::Record:    return "Record Track";
    // case MenuItem::Play:      return "Play Track";
    case MenuItem::DemoMode:  return "Demo";
    case MenuItem::ArmMode:   return "Arm Mode";
    default:                  return "?";
  }
}

const char* DisplayManager::armModeLabel(DisplayManager::ArmMode v) const {
  switch (v) {
    case DisplayManager::ArmMode::Free: return "Free";
    case DisplayManager::ArmMode::Sync: return "Sync";
    default: return "?";
  }
}

const char* DisplayManager::demoModeLabel(DisplayManager::DemoMode v) const {
  switch (v) {
    case DisplayManager::DemoMode::Demo: return "Demo";
    case DisplayManager::DemoMode::Innactive: return "Innactive";
    default: return "?";
  }
}

const char* DisplayManager::speedLabel(SpeedMode v) const {
  switch (v) {
    case SpeedMode::Fast:   return "Fast";
    case SpeedMode::Slow:   return "Slow";
    default:                return "?";
  }
}

void DisplayManager::drawRow(uint8_t rowIndex, bool selected, const char* left, const char* right) {

  const int y = 12 + (rowIndex * 10);


  display.setCursor(0, y);
  display.print(selected ? ">" : " ");
  display.print(left);

  // Right-aligned-ish value
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(right, 0, 0, &x1, &y1, &w, &h);
  int valueX = 128 - (int)w - 2;
  if (valueX < 70) valueX = 70;

  display.setCursor(valueX, y);
  display.print(right);
}

void DisplayManager::showInitializing() {
  if (!_ok) return;

  clearAndHeader(F("BT-Rover"));

  display.setCursor(0, 16);
  display.println(F("Initializing..."));

  if (_hint.length()) {
    display.setCursor(0, 52);
    display.println(_hint);
  }

  display.display();
}

void DisplayManager::showStatus(bool btEnabled, bool btConnected, SpeedMode speed,
                                bool isRecording, bool isPlaying, ArmMode armMode, DemoMode demoMode) {
  if (!_ok) return;

  clearAndHeader(F("BT-Rover"));

  // Speed letter
  char sp = 'F';
  if (speed == SpeedMode::Fast) sp = 'F';
  else if (speed == SpeedMode::Slow) sp = 'S';

  display.setTextSize(2);
  display.setCursor(0, 14);
  display.write(sp);
  display.setTextSize(1);

  // Mode label
  display.setCursor(20, 18);
  display.setTextSize(3);
  // if (isRecording) display.print(F("REC"));
  // else if (isPlaying) display.print(F("PLAY"));
  // else display.print(F("READY"));
  if (demoMode == DemoMode::Demo) {
    
    display.write("Demo");
  }
  else if (demoMode == DemoMode::Innactive) {
    
      // arm mode (Sync,Free)
    display.setCursor(65,35);
    display.setTextSize(2);
    if (armMode == ArmMode::Free) display.write("Free");
    else if (armMode == ArmMode::Sync) display.write("Sync");
    else display.print(F("??"));

  }



  display.setTextSize(1);

  // BT icon (blink if enabled but not connected)
  const int iconX = 128 - 18;
  const int iconY = 12;

  bool blink = (millis() / 350) % 2;

  if (btEnabled) {
    if (btConnected) {
      display.drawBitmap(iconX, iconY, bt_icon_on_16, 16, 16, SSD1306_WHITE);
    } else {
      if (blink) display.drawBitmap(iconX, iconY, bt_icon_on_16, 16, 16, SSD1306_WHITE);
    }
  } else {
    display.drawBitmap(iconX, iconY, bt_icon_on_16, 16, 16, SSD1306_WHITE);
    display.drawBitmap(iconX, iconY, slash_16, 16, 16, SSD1306_WHITE);
  }

  display.display();
}





void DisplayManager::renderMenuMain(const Settings& s, const MenuState& m,
                                   bool isRecording, bool isPlaying, bool hasTrack) {

  if (!_ok) return;

  clearAndHeader(F("Menu"));

  // Row 0: Speed
  drawRow(0, m.cursor == MenuItem::Speed,
          menuItemLabel(MenuItem::Speed),
          speedLabel(s.speed));

  // Row 1: Bluetooth
  bool selected = (m.cursor == MenuItem::Bluetooth);
  // Draw row label
  drawRow(1, selected,
          menuItemLabel(MenuItem::Bluetooth),
          "");  // no text on right

  // Row 2: Arm Mode
  drawRow(2, m.cursor == MenuItem::ArmMode,
          menuItemLabel(MenuItem::ArmMode),
          this->armModeLabel(s.armMode));

  // Row 3: Record
  // drawRow(3, m.cursor == MenuItem::Record,
  //         menuItemLabel(MenuItem::Record),
  //         isRecording ? "REC" : "Ready");

  drawRow(3, m.cursor == MenuItem::DemoMode,
          menuItemLabel(MenuItem::DemoMode),
          this->demoModeLabel(s.demoMode));

  // Row 4: Play
  // const char* playVal = isPlaying ? "PLAY" : (hasTrack ? "Ready" : "No Track");
  // drawRow(4, m.cursor == MenuItem::Play,
  //         menuItemLabel(MenuItem::Play),
  //         playVal);


  // Draw icon at right side
  int iconX = 128 - 18;
  int iconY = (12 + (1 * 10)) - 2;

  if (s.bluetoothOn) {
    // Draw icon at right side
    int iconX = 128 - 18;
    int iconY = (12 + (1 * 10)) - 2;

    // Always draw the base Bluetooth icon
    display.drawBitmap(iconX, iconY, bt_icon_on_16, 16, 16, SSD1306_WHITE);

    // If OFF, draw a slash overlay to show disabled
    if (!s.bluetoothOn) {
      display.drawBitmap(iconX, iconY, slash_16, 16, 16, SSD1306_WHITE);
    }
  }


  // display.setCursor(0, 60);
  // display.print(F("Up/Down + Left/Right"));

  display.display();
}
