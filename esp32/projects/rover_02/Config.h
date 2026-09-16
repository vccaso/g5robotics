#pragma once
#include <Arduino.h>

// Ultrasonic
static const int TRIG_PIN = 5;
static const int ECHO_PIN = 4;

// Servo
static const int SERVO_PIN = 23;

// L298N #1 (A=RR, B=RL)
static const int L1_ENA = 25, L1_IN1 = 26, L1_IN2 = 27;
static const int L1_ENB = 14, L1_IN3 = 13, L1_IN4 = 16;

// L298N #2 (A=FR, B=FL)
static const int L2_ENA = 33, L2_IN1 = 32, L2_IN2 = 21;
static const int L2_ENB = 18, L2_IN3 = 19, L2_IN4 = 22;

// Servo limits: +/- 40
static const int SERVO_CENTER = 90;
static const int SERVO_RANGE  = 40;
static const int SERVO_MIN    = SERVO_CENTER - SERVO_RANGE; // 50
static const int SERVO_MAX    = SERVO_CENTER + SERVO_RANGE; // 130;

// PWM
static const int PWM_FREQ = 20000;
static const int PWM_RES  = 8;
static const int PWM_MAX  = 255;

// Safety & Ramping
static const float MIN_SAFE_DISTANCE_CM = 15.0f;
static const int RAMP_MAX_STEP          = 15;

// IMPORTANT: keep 0..3 free for ESP32Servo
static const int CH_FL = 4;
static const int CH_FR = 5;
static const int CH_RL = 6;
static const int CH_RR = 7;
