# ESP32 Projects Overview

This directory contains advanced robotics and embedded control applications targeting the dual-core ESP32 microcontroller architecture.

---

## Table of ESP32 Projects

| Project | Description | Hardware Subsystems | Key Features |
| :--- | :--- | :--- | :--- |
| **[rover_02](file:///Users/developer/src/g5robotics/esp32/projects/rover_02/rover_02.ino)** | Bluetooth Mecanum Drive Rover with Pan-Tilt Ultrasonic Sensor | ESP32 WROOM, Dual L298N Drivers, HC-SR04, Servo Pan, Gamepad | 4-Wheel Mecanum kinematics, interrupt-driven ultrasonic reader, motor PWM speed ramping, obstacle auto-brake, Bluepad32 gamepad |
| **[RoverTSA](file:///Users/developer/src/g5robotics/esp32/projects/RoverTSA/RoverTSA.ino)** | Autonomous & Xbox Controlled Robotic Arm Rover with OLED & Trajectory Recorder | ESP32 WROOM, OLED Display (SSD1306), PCA9685 I2C Servo Arm, L298N Driver, Buttons | Xbox Gamepad (Bluepad32), interactive OLED menu, autonomous trajectory record & playback, robotic arm (shoulder, wrist, gripper), failsafe timeout |

---

## Detailed Project Breakdowns

### 1. [rover_02](file:///Users/developer/src/g5robotics/esp32/projects/rover_02/rover_02.ino) - Bluetooth Mecanum Rover

A 4-wheel omnidirectional Mecanum drive rover controlled via Bluetooth gamepad with non-blocking ultrasonic distance measurement and safety features.

#### Hardware & Pin Mapping
- **Microcontroller**: ESP32 WROOM-32
- **L298N Driver #1 (Rear Wheels)**:
  - Rear Right (RR): `ENA = 25`, `IN1 = 26`, `IN2 = 27`, PWM Channel `7`
  - Rear Left (RL): `ENB = 14`, `IN3 = 13`, `IN4 = 16`, PWM Channel `6`
- **L298N Driver #2 (Front Wheels)**:
  - Front Right (FR): `ENA = 33`, `IN1 = 32`, `IN2 = 21`, PWM Channel `5`
  - Front Left (FL): `ENB = 18`, `IN3 = 19`, `IN4 = 22`, PWM Channel `4`
- **Pan Servo**: GPIO `23` (50Hz PWM, range $50^\circ - 130^\circ$)
- **Ultrasonic Sensor (HC-SR04)**: `Trig = GPIO 5`, `Echo = GPIO 4`
- **Gamepad Controller**: Bluetooth Gamepad using Bluepad32 library

#### Key Software Features
- **Mecanum Drive Mixing Engine**: Full 3-DOF translation ($V_x$ forward/backward, $V_y$ strafe left/right, $\omega$ yaw rotation) with speed normalization.
- **Interrupt-Driven Non-Blocking Ultrasonic Reader**: Employs `IRAM_ATTR` edge interrupts on `ECHO_PIN` with 60ms periodic ping cycles (0ms blocking time in `loop()`).
- **Motor PWM Speed Ramping**: Smoothly interpolates motor target speeds (`RAMP_MAX_STEP = 15`) to eliminate high inrush current spikes and prevent ESP32 MCU power brownouts.
- **Obstacle Auto-Brake**: Automatically halts forward drive (`st.vx = 0.0f`) when an obstacle is detected within $15\text{ cm}$ while preserving reverse and steering mobility.
- **Dual ESP32 Core Compatibility**: Preprocessor-guarded PWM API calls (`#if ESP_ARDUINO_VERSION >= ...`) supporting both Arduino ESP32 core v2.x and v3.x.

---

### 2. [RoverTSA](file:///Users/developer/src/g5robotics/esp32/projects/RoverTSA/RoverTSA.ino) - Arm Rover & Trajectory Recorder

A full-featured mobile robot platform with an integrated 3-DOF robotic arm, OLED graphical user interface, dual button menu navigation, Xbox wireless controller integration, and trajectory recording/playback.

#### Hardware & Pin Mapping
- **Microcontroller**: ESP32 WROOM-32
- **Motor Driver (L298N Dual Channel)**:
  - Right Side Motors: `ENA = 32`, `IN1 = 33`, `IN2 = 25` (PWM Channel 1)
  - Left Side Motors: `ENB = 14`, `IN3 = 27`, `IN4 = 26` (PWM Channel 0)
- **I2C Bus (`SDA = 21`, `SCL = 22`)**:
  - SSD1306 OLED Display ($128 \times 64$ pixels)
  - PCA9685 16-Channel Servo Driver (Address `0x40`)
    - `CH 0`: Arm Shoulder/Elbow
    - `CH 1`: Wrist Pitch
    - `CH 2`: Gripper Open/Close
- **Physical Input Buttons**: `Button A = GPIO 18` (Next/Scroll), `Button B = GPIO 19` (Select/Enter & Long Press)

#### Key Software Features
- **Xbox Wireless Controller Integration**: Real-time driving and robotic arm manipulation using Bluepad32 with deadzone filtering and gain control.
- **Interactive OLED Menu System**: Multi-page graphical UI for selecting Drive Speed Modes (Fast, Slow, Normal), viewing live motor PWM/telemetry, and triggering trajectory playback.
- **Trajectory Recording & Playback**: Autonomous record/replay engine that captures real-time driver motor commands and arm poses to memory and executes recorded trajectories autonomously.
- **Dual Button Manager**: Hardware button handler supporting short press, long press (2000ms), and debounced event triggering.
- **Failsafe System**: Automatically halts all motor outputs if gamepad communication drops for more than $500\text{ms}$.
