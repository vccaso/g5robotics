#pragma once
#include <Arduino.h>

// This struct is the single "output" of the controller layer.
struct ControllerState {
  bool connected = false;

  // -1..+1 (your mecanum mixer expects this)
  float vx = 0;   // forward (+) / backward (-)
  float vy = 0;   // left (+) / right (-)
  float w  = 0;   // rotate left (+) / right (-)

  int maxPWM = 190;      // default speed
  int servoTarget = 90;  // degrees (you'll clamp in ServoPan)

  bool estop = false;    // X button
  bool turbo = false;    // A button
};

// Call once in setup()
void controllerBegin();

// Call in loop(); returns latest state snapshot
ControllerState controllerUpdate();

// Optional utility
bool controllerIsConnected();

// Optional: wipe pairings
void controllerForgetKeys();
