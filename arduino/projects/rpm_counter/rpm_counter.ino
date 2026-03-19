#include <TM1637Display.h>

#define SENSOR_PIN 2
#define DISPLAY_CLK 5
#define DISPLAY_DIO 4

#define RESET_BTN_PIN 6
#define MODE_BTN_PIN  7

#define PULSES_PER_REV 20  

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

volatile unsigned long pulseCount = 0;
volatile unsigned long lastPulseMicros = 0;

unsigned long rpm = 0;

// 0 = count mode, 1 = rpm mode
bool showRPM = false;

// button debounce
unsigned long lastResetPress = 0;
unsigned long lastModePress = 0;
const unsigned long buttonDebounceMs = 200;

// rpm update timing
unsigned long lastRpmCalc = 0;
unsigned long lastRpmPulseCount = 0;
const unsigned long rpmIntervalMs = 1000;

void countPulse() {
  unsigned long now = micros();

  // simple noise filter / debounce for sensor
  if (now - lastPulseMicros > 3000) {
    pulseCount++;
    lastPulseMicros = now;
  }
}

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);
  pinMode(MODE_BTN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), countPulse, FALLING);

  display.setBrightness(7, true);
  display.showNumberDec(0, false);

  Serial.begin(9600);
  Serial.println("System started");
}

void loop() {
  handleButtons();
  updateRPM();
  updateDisplay();
}

void handleButtons() {
  unsigned long now = millis();

  // RESET button
  if (digitalRead(RESET_BTN_PIN) == LOW) {
    if (now - lastResetPress > buttonDebounceMs) {
      noInterrupts();
      pulseCount = 0;
      lastRpmPulseCount = 0;
      interrupts();

      rpm = 0;

      Serial.println("Reset pressed");
      lastResetPress = now;
    }
  }

  // MODE button
  if (digitalRead(MODE_BTN_PIN) == LOW) {
    if (now - lastModePress > buttonDebounceMs) {
      showRPM = !showRPM;

      Serial.print("Mode changed to: ");
      Serial.println(showRPM ? "RPM" : "COUNT");

      lastModePress = now;
    }
  }
}

void updateRPM() {
  unsigned long now = millis();

  if (now - lastRpmCalc >= rpmIntervalMs) {
    unsigned long currentCount;

    noInterrupts();
    currentCount = pulseCount;
    interrupts();

    unsigned long pulsesInWindow = currentCount - lastRpmPulseCount;
    lastRpmPulseCount = currentCount;

    rpm = (pulsesInWindow * 60UL) / PULSES_PER_REV;
    lastRpmCalc = now;

    Serial.print("Count: ");
    Serial.print(currentCount);
    Serial.print("  RPM: ");
    Serial.println(rpm);
  }
}

void updateDisplay() {
  unsigned long valueToShow;

  if (showRPM) {
    valueToShow = rpm;
  } else {
    noInterrupts();
    valueToShow = pulseCount;
    interrupts();
  }

  if (valueToShow > 9999) {
    valueToShow = 9999; // limit for 4-digit display
  }

  display.showNumberDec((int)valueToShow, false);
}