#include "Ultrasonic.h"

static volatile uint32_t echoStart = 0;
static volatile float cachedDistance = -1.0f;
static uint32_t lastTriggerTime = 0;

void IRAM_ATTR echoISR() {
  uint32_t now = micros();
  if (digitalRead(ECHO_PIN) == HIGH) {
    echoStart = now;
  } else {
    if (echoStart > 0) {
      uint32_t duration = now - echoStart;
      cachedDistance = (duration * 0.0343f) / 2.0f;
      echoStart = 0;
    }
  }
}

void ultrasonicBegin() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
}

void ultrasonicUpdate() {
  uint32_t now = millis();

  // Timeout check for missed echo
  if (echoStart > 0 && (micros() - echoStart > 30000)) {
    echoStart = 0;
    cachedDistance = -1.0f;
  }

  // Periodic non-blocking trigger pulse every 60ms
  if (now - lastTriggerTime >= 60) {
    lastTriggerTime = now;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  }
}

float readDistanceCM() {
  ultrasonicUpdate();
  return cachedDistance;
}
