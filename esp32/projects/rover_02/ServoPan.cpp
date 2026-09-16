#include "ServoPan.h"

static Servo panServo;
static int servoAngle = SERVO_CENTER;

void servoBegin() {
  panServo.setPeriodHertz(50);
  panServo.attach(SERVO_PIN, 500, 2400);
  panServo.write(SERVO_CENTER);
}

void servoSetAngle(int angle) {
  servoAngle = constrain(angle, SERVO_MIN, SERVO_MAX);
  panServo.write(servoAngle);
}

int servoGetAngle() { return servoAngle; }
