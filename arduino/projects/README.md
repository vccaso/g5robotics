# Arduino Projects Overview

This directory contains specialized robotics and embedded control projects built for Arduino boards (ATmega328P / Arduino Uno / Mega).

---

## Table of Projects

| Project | Description | Primary Components | Key Features |
| :--- | :--- | :--- | :--- |
| **[rover01_ir](file:///Users/developer/src/g5robotics/arduino/projects/rover01_ir/rover01_ir.ino)** | IR Remote Controlled 2WD/4WD Rover with Collision Avoidance | Arduino Uno, L298N Driver, IR Receiver, HC-SR04, Laser, Buzzer | Smart IR repeat filtering, 600ms remote signal failsafe, 60ms non-blocking ultrasonic sensing, noise filter |
| **[rover3](file:///Users/developer/src/g5robotics/arduino/projects/rover3/rover3.ino)** | 4-Wheel Drive / 4-Wheel Steer (4WD/4WS) Robotic Platform | Arduino Uno, PCA9685 I2C Servo Driver, Dual L298N Drivers | $+45^\circ / -45^\circ$ zero-radius on-axis spin, concurrent motor kickstart, closed-loop demo returning to origin |
| **[rpm_counter](file:///Users/developer/src/g5robotics/arduino/projects/rpm_counter/rpm_counter.ino)** | Digital Optical Tachometer & Pulse Counter | Arduino Uno, Speed Sensor, TM1637 4-Digit Display, Buttons | Interrupt-driven pulse counting (`FALLING` edge), RPM calculation (20 pulses/rev), mode/reset buttons |
| **[ultrasonicspeed](file:///Users/developer/src/g5robotics/arduino/projects/ultrasonicspeed/ultrasonicspeed.ino)** | Ultrasonic Distance & Speed Meter | Arduino Uno, HC-SR04 Sensor, MAX7219 LED Matrices, Buttons, LEDs | Distance & velocity measurement ($\Delta d / \Delta t$), Metric/Imperial conversion, LED matrix rendering |
| **[display8x64](file:///Users/developer/src/g5robotics/arduino/projects/display8x64)** | 8x64 LED Matrix Display Driver Project | Arduino Board, 8x64 MAX7219 LED Matrix Display Module | Expanded multi-matrix display controller setup |

---

## Detailed Project Breakdowns

### 1. [rover01_ir](file:///Users/developer/src/g5robotics/arduino/projects/rover01_ir/rover01_ir.ino) - IR Remote Controlled Rover

An obstacle-avoiding rover driven by a 32-bit Infrared (IR) NEC remote control.

#### Hardware & Pin Mapping
- **Microcontroller**: Arduino Uno
- **Motor Driver (L298N)**: `ENA = 9` (PWM), `IN1 = 2`, `IN2 = 3`, `ENB = 6` (PWM), `IN3 = 4`, `IN4 = 5`
- **IR Receiver**: Pin `11`
- **Ultrasonic Sensor (HC-SR04)**: `Trig = 12`, `Echo = A0` *(bypasses Pin 13 LED interference)*
- **Peripherals**: Horn Buzzer = Pin `7`, Laser Pointer = Pin `8`

#### Key Software Features
- **Smart IR Repeat Filtering**: Motion commands (`UP`, `DOWN`, `LEFT`, `RIGHT`) process `CMD_REPEAT` (`0xFFFFFFFF`), while one-shot buttons (Horn, Laser, Speed ±) ignore repeats to prevent accidental double-taps.
- **Out-of-Range Signal Loss Timeout**: Automatically stops motors if no IR signal or repeat code is received within 600ms.
- **Non-Blocking Ultrasonic Sampling**: Measures distance every 60ms using `millis()` with a 2-sample noise debouncer to eliminate false positive triggers.

---

### 2. [rover3](file:///Users/developer/src/g5robotics/arduino/projects/rover3/rover3.ino) - 4-Wheel Steer / 4-Wheel Drive Platform

A 4WD/4WS omnidirectional rover utilizing a PCA9685 I2C 16-channel PWM controller for individual wheel steering.

#### Hardware & Pin Mapping
- **Microcontroller**: Arduino Uno
- **I2C Servo Driver**: PCA9685 (Address `0x40`)
  - `CH 0`: Front Left Servo
  - `CH 1`: Front Right Servo
  - `CH 2`: Rear Left Servo
  - `CH 3`: Rear Right Servo
- **Dual L298N Motor Drivers**:
  - Motor 1 (Front Left): `EN = 3`, `IN1 = 4`, `IN2 = 7`
  - Motor 2 (Front Right): `EN = 5`, `IN1 = 8`, `IN2 = 12`
  - Motor 3 (Rear Left): `EN = 6`, `IN1 = A0`, `IN2 = A1`
  - Motor 4 (Rear Right): `EN = 9`, `IN1 = A2`, `IN2 = A3`
- **3D CAD & Chassis Models**: Stored in [3dmodels](file:///Users/developer/src/g5robotics/arduino/projects/rover3/3dmodels) (includes STL files and assembly photos).

#### Key Software Features
- **Zero-Radius On-Axis Tangential Spin**: Positions Front-Left & Rear-Right wheels to $+45^\circ$ and Front-Right & Rear-Left wheels to $-45^\circ$ ($135^\circ$), allowing all 4 motors driving forward to spin the robot $360^\circ$ on its central midpoint axis.
- **Concurrent Motor Kickstart**: Replaces sequential delays with a single 50ms pulse across all motor channels.
- **Calibrated Servo Bounds**: Restricts pulse widths to `205–410` counts ($1000\mu\text{s} - 2000\mu\text{s}$) to prevent mechanical servo binding.
- **Closed-Loop Demo Routine**: Executes a complete motion routine (linear pairs, crab translation, box square path, figure-8 dual-arc loop) that returns the rover to its exact starting point.

---

### 3. [rpm_counter](file:///Users/developer/src/g5robotics/arduino/projects/rpm_counter/rpm_counter.ino) - Digital Tachometer

An optical sensor-based pulse counter and RPM tachometer.

#### Hardware & Pin Mapping
- **Microcontroller**: Arduino Uno
- **Speed Sensor**: Pin `2` (External Interrupt 0)
- **TM1637 4-Digit 7-Segment Display**: `CLK = 5`, `DIO = 4`
- **Control Buttons**: `Reset Pin = 6`, `Mode Pin = 7` (both using `INPUT_PULLUP`)

#### Key Software Features
- **Interrupt-Driven Pulse Counting**: Tracks `FALLING` edge transitions on pin 2 with a 3ms micros noise filter.
- **RPM Calculation**: Computes RPM over a 1000ms window based on 20 pulses per revolution (`PULSES_PER_REV = 20`).
- **Mode Toggle**: Switches display output between total raw pulse count and calculated RPM.

---

### 4. [ultrasonicspeed](file:///Users/developer/src/g5robotics/arduino/projects/ultrasonicspeed/ultrasonicspeed.ino) - Distance & Velocity Meter

A dual-mode ultrasonic meter that displays distance and velocity on MAX7219 LED matrices.

#### Hardware & Pin Mapping
- **Microcontroller**: Arduino Uno
- **Ultrasonic Sensor**: HC-SR04 (`Trig = Pin X`, `Echo = Pin Y`)
- **MAX7219 LED Matrix Display**: `DIN = Pin Z`, `CLK = Pin W`, `CS = Pin V`
- **Control Buttons**: Mode Switch (Distance/Speed) = `Pin A`, Unit Switch (Metric/Imperial) = `Pin B`
- **Indicator LEDs**: Metric LED = `Pin C`, Imperial LED = `Pin D`

#### Key Software Features
- **Mode & Unit Selection**: Toggles between Distance Mode and Speed Calculation Mode ($\Delta d / \Delta t$).
- **Unit Conversion**: Switches dynamically between Metric (meters, m/s) and Imperial (feet, ft/s) units.

---

### 5. [display8x64](file:///Users/developer/src/g5robotics/arduino/projects/display8x64) - LED Matrix Workspace

Workspace repository for 8x64 pixel MAX7219 multi-matrix LED displays and custom font rendering routines.