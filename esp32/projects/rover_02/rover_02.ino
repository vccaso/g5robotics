#include <Arduino.h>

#include "Config.h"
#include "Motors.h"
#include "Ultrasonic.h"
#include "ServoPan.h"
#include "ControllerMgr.h"

void setup() {
  Serial.begin(115200);
  delay(300);

  ultrasonicBegin();
  servoBegin();
  motorsBegin();

  controllerBegin();

  Serial.println("=== Rover ===");
}

void loop() {
  // 1) Non-blocking distance update
  float dist = readDistanceCM();

  // 2) Read controller
  ControllerState st = controllerUpdate();

  // 3) Safety when disconnected
  if (!st.connected) {
    stopAllMotors();
    servoSetAngle(SERVO_CENTER);
  } else {
    // 4) Apply servo
    servoSetAngle(st.servoTarget);

    // 5) Obstacle safety override (prevent driving forward into nearby obstacles)
    if (dist > 0.0f && dist < MIN_SAFE_DISTANCE_CM && st.vx > 0.0f) {
      st.vx = 0.0f;
    }

    // 6) Apply drive (unless estop)
    if (st.estop) {
      stopAllMotors();
    } else {
      driveMecanum(st.vx, st.vy, st.w, st.maxPWM);
    }
  }

  // 7) Periodic telemetry print
  static uint32_t lastUI = 0;
  if (millis() - lastUI > 200) {
    lastUI = millis();
    Serial.printf("Dist: %.1f cm | Servo:%d | Connected:%d\n", dist, servoGetAngle(), (int)st.connected);
  }

  delay(5);
}
