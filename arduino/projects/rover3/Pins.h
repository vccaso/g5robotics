#pragma once
#include <Arduino.h>

// ======================================================
// PCA9685 I2C Address
// ======================================================
static const uint8_t PCA9685_ADDR = 0x40;

// ======================================================
// Steering servo channels on PCA9685
// ======================================================
static const uint8_t SERVO_CH_FL = 0;   // Front Left
static const uint8_t SERVO_CH_FR = 1;   // Front Right
static const uint8_t SERVO_CH_RL = 2;   // Rear Left
static const uint8_t SERVO_CH_RR = 3;   // Rear Right

// ======================================================
// L298N #1 -> Motor 1 and Motor 2
// ======================================================
static const uint8_t M1_EN  = 3;   // PWM
static const uint8_t M1_IN1 = 4;
static const uint8_t M1_IN2 = 7;

static const uint8_t M2_EN  = 5;   // PWM
static const uint8_t M2_IN1 = 8;
static const uint8_t M2_IN2 = 12;

// ======================================================
// L298N #2 -> Motor 3 and Motor 4
// ======================================================
static const uint8_t M3_EN  = 6;   // PWM
static const uint8_t M3_IN1 = A0;
static const uint8_t M3_IN2 = A1;

static const uint8_t M4_EN  = 9;   // PWM
static const uint8_t M4_IN1 = A2;
static const uint8_t M4_IN2 = A3;