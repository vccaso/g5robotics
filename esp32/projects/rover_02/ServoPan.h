#pragma once
#include <ESP32Servo.h>
#include "Config.h"

void servoBegin();
void servoSetAngle(int angle);
int  servoGetAngle();
