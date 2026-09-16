#pragma once
#include <Arduino.h>
#include "Config.h"

void ultrasonicBegin();
void ultrasonicUpdate();
float readDistanceCM();
