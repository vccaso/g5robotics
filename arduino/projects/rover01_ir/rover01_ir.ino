#include <IRremote.hpp>

// -------- IR receiver --------
const uint8_t IR_RECEIVE_PIN = 11;

// Replace these with YOUR remote's raw hex codes from the decoder sketch
const uint32_t CMD_UP = 0xE718FF00;         // 2
const uint32_t CMD_DOWN = 0xAD52FF00;       // 8
const uint32_t CMD_LEFT = 0xF708FF00;       // 4
const uint32_t CMD_RIGHT = 0xA55AFF00;      // 6
const uint32_t CMD_OK = 0xE31CFF00;         // STOP (5)
const uint32_t CMD_1 = 0xBB44FF00;          // beep
const uint32_t CMD_2 = 0xBF40FF00;          // Laser
const uint32_t CMD_0 = 0xBC43FF00;          // Emergency Stop
const uint32_t CMD_SPEED_UP = 0xEA15FF00;   // + increase speed
const uint32_t CMD_SPEED_DOWN = 0xF807FF00; // - reduce speed

// For remote repeat signal & failsafe timeout
const uint32_t CMD_REPEAT = 0xFFFFFFFF;
uint32_t lastCmd = 0;
uint32_t lastMotionCmd = 0;
uint32_t lastIRSignalTime = 0;
uint32_t lastPingTime = 0;
const uint32_t IR_TIMEOUT_MS = 200;

// -------- L298N motor pins --------
const int ENA = 9;
const int IN1 = 2;
const int IN2 = 3;

const int ENB = 6;
const int IN3 = 4;
const int IN4 = 5;

// Buzzer + Laser
const int BUZZER_PIN = 7;
const int LASER_PIN = 8;

// Motor speed control
int baseSpeed = 200;
const int MIN_SPEED = 80;
const int MAX_SPEED = 255;
const int SPEED_STEP = 20;
bool laserOn = false;

// -------- Ultrasonic HC-SR04 pins --------
const int trigPin = 12;
const int echoPin = A0;
const int SAFE_DISTANCE = 10; // cm

// ================= Distance Reader =================
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
  long distance = duration * 0.034 / 2;
  return distance;
}

// ================= Motor Helpers =================
void setLeftMotor(int speed) {
  if (speed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
  } else if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -speed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

void setRightMotor(int speed) {
  if (speed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
  } else if (speed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -speed);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}

void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

void moveForward() {
  setLeftMotor(baseSpeed);
  setRightMotor(baseSpeed);
}
void moveBackward() {
  setLeftMotor(-baseSpeed);
  setRightMotor(-baseSpeed);
}
void turnLeft() {
  setLeftMotor(-baseSpeed);
  setRightMotor(baseSpeed);
}
void turnRight() {
  setLeftMotor(baseSpeed);
  setRightMotor(-baseSpeed);
}

void applyLastMotion() {
  if (lastMotionCmd == CMD_UP)
    moveForward();
  else if (lastMotionCmd == CMD_DOWN)
    moveBackward();
  else if (lastMotionCmd == CMD_LEFT)
    turnLeft();
  else if (lastMotionCmd == CMD_RIGHT)
    turnRight();
}

// ================= Effects =================
void beepHorn(unsigned int d) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(d);
  digitalWrite(BUZZER_PIN, LOW);
}

void toggleLaser() {
  laserOn = !laserOn;
  digitalWrite(LASER_PIN, laserOn ? HIGH : LOW);
}

void increaseSpeed() {
  baseSpeed += SPEED_STEP;
  if (baseSpeed > MAX_SPEED)
    baseSpeed = MAX_SPEED;
  Serial.print("Speed UP: ");
  Serial.println(baseSpeed);
  applyLastMotion();
}

void decreaseSpeed() {
  baseSpeed -= SPEED_STEP;
  if (baseSpeed < MIN_SPEED)
    baseSpeed = MIN_SPEED;
  Serial.print("Speed DOWN: ");
  Serial.println(baseSpeed);
  applyLastMotion();
}

// ================= IR Command Handler =================
void handleCommand(uint32_t cmd) {

  if (cmd == CMD_UP) {
    lastMotionCmd = CMD_UP;
    moveForward();
    Serial.println("Forward");
  } else if (cmd == CMD_DOWN) {
    lastMotionCmd = CMD_DOWN;
    moveBackward();
    Serial.println("Backward");
  } else if (cmd == CMD_LEFT) {
    lastMotionCmd = CMD_LEFT;
    turnLeft();
    Serial.println("Left");
  } else if (cmd == CMD_RIGHT) {
    lastMotionCmd = CMD_RIGHT;
    turnRight();
    Serial.println("Right");
  } else if (cmd == CMD_OK) {
    stopMotors();
    lastMotionCmd = 0;
    Serial.println("Stop");
  } else if (cmd == CMD_1) {
    beepHorn(150);
    Serial.println("Horn");
  } else if (cmd == CMD_2) {
    toggleLaser();
    Serial.println(laserOn ? "Laser ON" : "Laser OFF");
  } else if (cmd == CMD_0) {
    stopMotors();
    laserOn = false;
    lastMotionCmd = 0;
    beepHorn(60);
    delay(60);
    beepHorn(60);
    Serial.println("!!! EMERGENCY STOP !!!");
  } else if (cmd == CMD_SPEED_UP) {
    increaseSpeed();
  } else if (cmd == CMD_SPEED_DOWN) {
    decreaseSpeed();
  }
}

// ================= Setup =================
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stopMotors();
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Rover Ready with Ultrasonic Avoidance!");
}

// ================= Main Loop =================
void loop() {
  // Handle IR
  if (IrReceiver.decode()) {
    uint32_t raw = IrReceiver.decodedIRData.decodedRawData;
    lastIRSignalTime = millis();

    if (raw == CMD_REPEAT) {
      // Repeat signal only applies to continuous motion commands
      if (lastMotionCmd != 0) {
        handleCommand(lastMotionCmd);
      }
    } else {
      lastCmd = raw;
      handleCommand(raw);
    }

    IrReceiver.resume();
  }

  // ---- Failsafe: Remote Signal Lost Timeout ----
  if (lastMotionCmd != 0 && (millis() - lastIRSignalTime > IR_TIMEOUT_MS)) {
    stopMotors();
    lastMotionCmd = 0;
    Serial.println("IR Signal Lost -> Failsafe Stop");
  }

  // ---- Automatic Obstacle Avoidance (forward only, sampled every 60ms) ----
  static uint8_t obstacleCount = 0;

  if (lastMotionCmd == CMD_UP && (millis() - lastPingTime > 60)) {
    lastPingTime = millis();
    long d = getDistance();

    // Filter out false ultra-short noise (d < 2cm) and check SAFE_DISTANCE
    if (d >= 2 && d < SAFE_DISTANCE) {
      obstacleCount++;
    } else {
      obstacleCount = 0;
    }

    // Require 2 consecutive valid obstacle readings to trigger avoidance
    if (obstacleCount >= 2) {
      obstacleCount = 0;

      Serial.print("Obstacle Detected: ");
      Serial.print(d);
      Serial.println(" cm -> Avoiding...");

      stopMotors();
      beepHorn(70);
      delay(70);
      beepHorn(70);

      moveBackward();
      delay(300);
      turnRight();
      delay(300);
      stopMotors();
      lastMotionCmd = 0;
    }
  }
}
