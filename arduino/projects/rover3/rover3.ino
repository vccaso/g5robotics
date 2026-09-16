#include "Pins.h"
#include "ServoController.h"
#include "DriveController.h"

ServoController steering(PCA9685_ADDR);
DriveController drive;

int speed1 = 100;
int movement_delay1 = 1000;
int movement_delay2 = 500;
int movement_delay3 = 2000;

void demo();

void setup() {
  Serial.begin(115200);
  Serial.println("Rover-3 starting...");

  drive.begin();
  steering.begin();

  // Initialize wheel positions and stop motors
  steering.setAllTo90();
  drive.stopAll();
  delay(1000);

  Serial.println("Initialization complete.");
}

void runFullClosedLoopDemo();

void loop() {
  runFullClosedLoopDemo();
  delay(10000); // Wait 10 seconds before running next cycle
}

void spin() {
  steering.spin(); // Set FL & RR to 135° (-45°), FR & RL to +45°
  delay(500);
  drive.motor1Forward(120);
  drive.motor2Forward(120);
  drive.motor3Backward(120); 
  drive.motor4Backward(120);
  delay(2000);
  drive.stopAll();
  delay(800);
  drive.motor1Backward(120); 
  drive.motor2Backward(120); 
  drive.motor3Forward(120);
  drive.motor4Forward(120);
  delay(2000);
  drive.stopAll();
  delay(500);
}

void runFullClosedLoopDemo() {
  int driveSpeed = 120;

  Serial.println("\n==========================================");
  Serial.println("  STARTING CLOSED-LOOP DEMO ROUTINE");
  Serial.println("==========================================");

  // ----------------------------------------------------
  // Phase 1: Fixed-Servo Linear & Crab Motion Pairs
  // ----------------------------------------------------
  Serial.println("[Phase 1.1] Straight Forward -> Backward Pair");
  steering.setAllTo90();
  delay(500);
  drive.allForward(driveSpeed);
  delay(1000);
  drive.stopAll();
  delay(500);
  drive.allBackward(driveSpeed);
  delay(1000);
  drive.stopAll();
  delay(800);

  Serial.println("[Phase 1.2] Crab Left (45°) -> Crab Right (45°) Pair");
  steering.crabLeft45();
  delay(500);
  drive.allForward(driveSpeed);
  delay(1200);
  drive.stopAll();
  delay(500);
  steering.crabRight45();
  delay(500);
  drive.allBackward(driveSpeed); // Return along exact crab line
  delay(1200);
  drive.stopAll();
  delay(800);

  spin();
  // ----------------------------------------------------
  // Phase 2: Zero-Radius On-Axis Spin (FL/RR @ +45°, FR/RL @ -45° with All-4 Forward)
  // ----------------------------------------------------
  Serial.println("[Phase 2] On-Axis Spin (FL/RR @ +45°, FR/RL @ -45° + All-4 Forward)");
  steering.spinTurnCW(); // Set FL & RR to +45°, FR & RL to 135° (-45°)
  delay(500);
  drive.allForward(driveSpeed); // All 4 wheels forward spin robot CW on central axis
  delay(2000);
  drive.stopAll();
  delay(500);

  steering.spinTurnCCW(); // Set FL & RR to 135° (-45°), FR & RL to +45°
  delay(500);
  drive.allForward(driveSpeed); // All 4 wheels forward spin robot CCW back to original heading
  delay(2000);
  drive.stopAll();
  delay(800);

  // ----------------------------------------------------
  // Phase 3: Closed Geometric Shape (4-Leg Crab Square Box)
  // ----------------------------------------------------
  Serial.println("[Phase 3] Closed Geometric Shape (Square Box Pattern)");
  // Leg 1: North (Straight Forward)
  steering.setAllTo90();
  delay(500);
  drive.allForward(driveSpeed);
  delay(1200);
  drive.stopAll();
  delay(400);

  // Leg 2: East (Crab Right 90°)
  steering.turnRight90();
  delay(500);
  drive.allForward(driveSpeed);
  delay(1200);
  drive.stopAll();
  delay(400);

  // Leg 3: South (Straight Backward)
  steering.setAllTo90();
  delay(500);
  drive.allBackward(driveSpeed);
  delay(1200);
  drive.stopAll();
  delay(400);

  // Leg 4: West (Crab Left 90° - Back to Start)
  steering.turnLeft90();
  delay(500);
  drive.allForward(driveSpeed);
  delay(1200);
  drive.stopAll();
  delay(800);

  spin();
  // ----------------------------------------------------
  // Phase 4: Symmetrical Dual-Arc Curve (Figure-8 Path)
  // ----------------------------------------------------
  Serial.println("[Phase 4] Symmetrical Dual-Arc Curve (Figure-8 Loop)");
  // Left Arc Outbound
  steering.steerCurveLeft();
  delay(500);
  drive.allForward(driveSpeed);
  delay(2000);
  drive.stopAll();
  delay(400);

  // Right Arc Outbound
  steering.steerCurveRight();
  delay(500);
  drive.allForward(driveSpeed);
  delay(2000);
  drive.stopAll();
  delay(400);

  // Right Arc Inbound (Reverse)
  steering.steerCurveRight();
  delay(500);
  drive.allBackward(driveSpeed);
  delay(2000);
  drive.stopAll();
  delay(400);

  // Left Arc Inbound (Reverse Back to Start)
  steering.steerCurveLeft();
  delay(500);
  drive.allBackward(driveSpeed);
  delay(2000);
  drive.stopAll();
  delay(800);

  // ----------------------------------------------------
  // Phase 5: Reset to Neutral Center Standby
  // ----------------------------------------------------
  Serial.println("[Phase 5] Routine Complete. Resetting to center origin.");
  steering.setAllTo90();
  drive.stopAll();
  delay(1000);
  Serial.println("==========================================");
  Serial.println("  CLOSED-LOOP DEMO FINISHED - AT ORIGIN");
  Serial.println("==========================================");
}
