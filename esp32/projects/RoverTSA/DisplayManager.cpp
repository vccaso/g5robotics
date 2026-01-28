#include "DisplayManager.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplayManager::begin() {
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void DisplayManager::showInitializing() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BT-Rover");
  display.println();
  display.println("Initializing...");
  display.display();
}

void DisplayManager::showReady(bool btConnected) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BT-Rover");
  display.println();
  display.print("Bluetooth: ");
  display.println(btConnected ? "Connected" : "Waiting...");
  display.display();
}
