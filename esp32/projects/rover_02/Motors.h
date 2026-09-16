#pragma once
#include <Arduino.h>
#include "Config.h"

struct Motor { int en, in1, in2, ch; };

extern Motor mRR, mRL, mFR, mFL;

void motorsBegin();
void stopAllMotors();
void driveMecanum(float vx, float vy, float w, int maxPWM);
