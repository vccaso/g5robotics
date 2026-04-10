#pragma once
#include <Arduino.h>

// ===============================
// L298N Motor Driver (2 channels)
// ===============================
// Left side motors = Channel A (OUT1/OUT2)
// Right side motors = Channel B (OUT3/OUT4)

// LEFT side (two motors in parallel)
static const int PIN_IN3 = 27;   // Direction pin 1
static const int PIN_IN4 = 26;   // Direction pin 2
static const int PIN_ENB = 14;   // Speed (PWM)

// RIGHT side (two motors in parallel)
static const int PIN_IN1 = 33;   // Direction pin 1
static const int PIN_IN2 = 25;   // Direction pin 2
static const int PIN_ENA = 32;   // Speed (PWM)

// ===============================
// PWM settings for ESP32 motors
// ===============================
static const int PWM_FREQ_HZ   = 20000; // 20 kHz (quiet)
static const int PWM_RES_BITS  = 8;     // 0..255
static const int PWM_CH_LEFT   = 0;
static const int PWM_CH_RIGHT  = 1;

// ===============================
// I2C bus pins (OLED + PCA9685)
// ===============================
static const int PIN_I2C_SDA = 21;  //white. -  orange
static const int PIN_I2C_SCL = 22;  //black - yellow

// ===============================
// Buttons (manual fallback later)
// ===============================
static const int PIN_BTN_A = 18; // NEXT
static const int PIN_BTN_B = 19; // SELECT

// ===============================
// Controller tuning
// ===============================
static const int DEADZONE = 10;          // ignore small stick movement (%)
static const float TURN_GAIN = 1.0f;     // steering sensitivity
static const float THROTTLE_GAIN = 1.0f; // throttle sensitivity

// ===============================
// Arm
// ===============================

// PCA9685 (servo driver)
static const uint8_t PCA9685_ADDR = 0x40;

// Servo channels on PCA9685
static const uint8_t SERVO_CH_ARM   = 0;  // shoulder OR elbow
static const uint8_t SERVO_CH_WRIST = 1;  // wrist pitch
static const uint8_t SERVO_CH_GRIP  = 2;  // fingers open/close
