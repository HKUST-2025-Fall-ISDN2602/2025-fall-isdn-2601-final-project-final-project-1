/*
 * ISDN 2601 Final Project: 5-Servo Mechanical Arm Control
 * Controls a 5-servo mechanical arm to grab and move objects
 * Using ESP8266 (LOLIN/WEMOS D1 R2 & mini)
 */

#include <Servo.h>

// Define servo objects for 5 servos
Servo baseServo;      // Servo 1: Base rotation
Servo shoulderServo;  // Servo 2: Shoulder joint
Servo elbowServo;     // Servo 3: Elbow joint
Servo wristServo;     // Servo 4: Wrist joint
Servo gripperServo;   // Servo 5: Gripper

// Define ESP8266 pins connected to servo signal wires
// Using pins that support PWM on ESP8266
const int BASE_PIN = D1;      // GPIO5
const int SHOULDER_PIN = D2;  // GPIO4
const int ELBOW_PIN = D3;     // GPIO0
const int WRIST_PIN = D4;     // GPIO2
const int GRIPPER_PIN = D5;   // GPIO14

// Servo angle limits (adjust these based on your arm's mechanical limits)
const int BASE_MIN = 0;
const int BASE_MAX = 180;

const int SHOULDER_MIN = 30;
const int SHOULDER_MAX = 150;

const int ELBOW_MIN = 20;
const int ELBOW_MAX = 160;

const int WRIST_MIN = 40;
const int WRIST_MAX = 140;

const int GRIPPER_OPEN = 70;   // Open gripper position
const int GRIPPER_CLOSE = 120; // Closed gripper position

// Default positions
int basePos = 90;
int shoulderPos = 90;
int elbowPos = 90;
int wristPos = 90;
int gripperPos = GRIPPER_OPEN;

void setup() {
  Serial.begin(115200);
  Serial.println("Mechanical Arm Control System Initializing...");
  
  // Attach servos to their respective pins
  baseServo.attach(BASE_PIN);
  shoulderServo.attach(SHOULDER_PIN);
  elbowServo.attach(ELBOW_PIN);
  wristServo.attach(WRIST_PIN);
  gripperServo.attach(GRIPPER_PIN);
  
  // Move to initial position
  moveToPosition(90, 90, 90, 90, GRIPPER_OPEN);
  delay(2000);
  
  Serial.println("Arm initialized. Ready for commands.");
  Serial.println("Commands: 1=Cube, 2=Cylinder, 3=Hat, 4=Boat, 5=Reset, 6=Test");
  Serial.println("Manual control: B[angle], S[angle], E[angle], W[angle], G[angle]");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.length() > 0) {
      processCommand(command);
    }
  }
}

void processCommand(String command) {
  char cmd = command.charAt(0);
  int value = command.substring(1).toInt();
  
  switch(cmd) {
    case '1':
      pickAndPlaceCube();
      break;
    case '2':
      pickAndPlaceCylinder();
      break;
    case '3':
      pickAndPlaceHat();
      break;
    case '4':
      pickAndPlaceBoat();
      break;
    case '5':
      resetArm();
      break;
    case '6':
      testSequence();
      break;
    case 'B': // Base control
      basePos = constrain(value, BASE_MIN, BASE_MAX);
      baseServo.write(basePos);
      Serial.println("Base: " + String(basePos));
      break;
    case 'S': // Shoulder control
      shoulderPos = constrain(value, SHOULDER_MIN, SHOULDER_MAX);
      shoulderServo.write(shoulderPos);
      Serial.println("Shoulder: " + String(shoulderPos));
      break;
    case 'E': // Elbow control
      elbowPos = constrain(value, ELBOW_MIN, ELBOW_MAX);
      elbowServo.write(elbowPos);
      Serial.println("Elbow: " + String(elbowPos));
      break;
    case 'W': // Wrist control
      wristPos = constrain(value, WRIST_MIN, WRIST_MAX);
      wristServo.write(wristPos);
      Serial.println("Wrist: " + String(wristPos));
      break;
    case 'G': // Gripper control
      gripperPos = constrain(value, GRIPPER_OPEN, GRIPPER_CLOSE);
      gripperServo.write(gripperPos);
      Serial.println("Gripper: " + String(gripperPos));
      break;
    default:
      Serial.println("Unknown command: " + command);
      break;
  }
}

void moveToPosition(int base, int shoulder, int elbow, int wrist, int gripper) {
  // Constrain angles to safe limits
  base = constrain(base, BASE_MIN, BASE_MAX);
  shoulder = constrain(shoulder, SHOULDER_MIN, SHOULDER_MAX);
  elbow = constrain(elbow, ELBOW_MIN, ELBOW_MAX);
  wrist = constrain(wrist, WRIST_MIN, WRIST_MAX);
  gripper = constrain(gripper, GRIPPER_OPEN, GRIPPER_CLOSE);
  
  // Move servos smoothly
  baseServo.write(base);
  delay(300);
  shoulderServo.write(shoulder);
  delay(300);
  elbowServo.write(elbow);
  delay(300);
  wristServo.write(wrist);
  delay(300);
  gripperServo.write(gripper);
  delay(300);
  
  // Update current positions
  basePos = base;
  shoulderPos = shoulder;
  elbowPos = elbow;
  wristPos = wrist;
  gripperPos = gripper;
}

void openGripper() {
  gripperServo.write(GRIPPER_OPEN);
  gripperPos = GRIPPER_OPEN;
  delay(500);
}

void closeGripper() {
  gripperServo.write(GRIPPER_CLOSE);
  gripperPos = GRIPPER_CLOSE;
  delay(500);
}

// Pre-defined positions for different objects
void pickAndPlaceCube() {
  Serial.println("Starting Cube pick and place sequence...");
  
  // Move to approach position above cube
  moveToPosition(90, 60, 120, 80, GRIPPER_OPEN);
  delay(1000);
  
  // Lower to pick position
  moveToPosition(90, 70, 130, 85, GRIPPER_OPEN);
  delay(1000);
  
  // Close gripper
  closeGripper();
  delay(1000);
  
  // Lift object
  moveToPosition(90, 50, 110, 75, GRIPPER_CLOSE);
  delay(1000);
  
  // Move to drop position
  moveToPosition(120, 60, 115, 80, GRIPPER_CLOSE);
  delay(1000);
  
  // Lower to place position
  moveToPosition(120, 70, 125, 85, GRIPPER_CLOSE);
  delay(1000);
  
  // Open gripper to release
  openGripper();
  delay(1000);
  
  // Return to safe position
  moveToPosition(120, 50, 110, 75, GRIPPER_OPEN);
  delay(1000);
  
  resetArm();
  Serial.println("Cube sequence completed.");
}

void pickAndPlaceCylinder() {
  Serial.println("Starting Cylinder pick and place sequence...");
  // Similar pattern as cube but with adjusted angles
  // You'll need to fine-tune these positions
  
  moveToPosition(60, 65, 125, 90, GRIPPER_OPEN);
  delay(1000);
  
  moveToPosition(60, 75, 135, 95, GRIPPER_OPEN);
  delay(1000);
  
  closeGripper();
  delay(1000);
  
  moveToPosition(60, 55, 115, 85, GRIPPER_CLOSE);
  delay(1000);
  
  moveToPosition(150, 65, 120, 90, GRIPPER_CLOSE);
  delay(1000);
  
  moveToPosition(150, 75, 130, 95, GRIPPER_CLOSE);
  delay(1000);
  
  openGripper();
  delay(1000);
  
  moveToPosition(150, 55, 115, 85, GRIPPER_OPEN);
  delay(1000);
  
  resetArm();
  Serial.println("Cylinder sequence completed.");
}

void pickAndPlaceHat() {
  Serial.println("Starting Hat pick and place sequence...");
  // Add specific positions for hat
  
  resetArm();
  Serial.println("Hat sequence completed.");
}

void pickAndPlaceBoat() {
  Serial.println("Starting Boat pick and place sequence...");
  // Add specific positions for boat
  
  resetArm();
  Serial.println("Boat sequence completed.");
}

void resetArm() {
  Serial.println("Returning to home position...");
  moveToPosition(90, 90, 90, 90, GRIPPER_OPEN);
  delay(1000);
}

void testSequence() {
  Serial.println("Starting test sequence...");
  
  // Test each joint individually
  for (int pos = BASE_MIN; pos <= BASE_MAX; pos += 30) {
    baseServo.write(pos);
    Serial.println("Base: " + String(pos));
    delay(1000);
  }
  baseServo.write(90);
  
  for (int pos = SHOULDER_MIN; pos <= SHOULDER_MAX; pos += 30) {
    shoulderServo.write(pos);
    Serial.println("Shoulder: " + String(pos));
    delay(1000);
  }
  shoulderServo.write(90);
  
  for (int pos = ELBOW_MIN; pos <= ELBOW_MAX; pos += 30) {
    elbowServo.write(pos);
    Serial.println("Elbow: " + String(pos));
    delay(1000);
  }
  elbowServo.write(90);
  
  for (int pos = WRIST_MIN; pos <= WRIST_MAX; pos += 30) {
    wristServo.write(pos);
    Serial.println("Wrist: " + String(pos));
    delay(1000);
  }
  wristServo.write(90);
  
  // Test gripper
  openGripper();
  delay(1000);
  closeGripper();
  delay(1000);
  openGripper();
  
  resetArm();
  Serial.println("Test sequence completed.");
}