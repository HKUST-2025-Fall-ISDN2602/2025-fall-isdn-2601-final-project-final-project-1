/*
 * ISDN 2601 Final Project: 5-Servo Mechanical Arm Control
 * Corrected version with inverted angles for opposite arm movement
 */

#include <Servo.h>

// Define servo objects
Servo baseServo;
Servo shoulderServo;  
Servo elbowServo;
Servo wristServo;
Servo gripperServo;

// ESP8266 pins
const int BASE_PIN = D1;
const int SHOULDER_PIN = D2;
const int ELBOW_PIN = D3;
const int WRIST_PIN = D4;
const int GRIPPER_PIN = D5;

// Servo limits
const int BASE_MIN = 0;
const int BASE_MAX = 180;
const int SHOULDER_MIN = 30;
const int SHOULDER_MAX = 150;
const int ELBOW_MIN = 20;
const int ELBOW_MAX = 160;
const int WRIST_MIN = 40;
const int WRIST_MAX = 140;
const int GRIPPER_OPEN = 70;
const int GRIPPER_CLOSE = 120;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println();
  Serial.println("=== MECHANICAL ARM CONTROL ===");
  Serial.println("Initializing servos...");
  
  // Attach servos
  baseServo.attach(BASE_PIN);
  shoulderServo.attach(SHOULDER_PIN);
  elbowServo.attach(ELBOW_PIN);
  wristServo.attach(WRIST_PIN);
  gripperServo.attach(GRIPPER_PIN);
  
  // Move to home position
  resetArm();
  delay(2000);
  
  Serial.println();
  Serial.println("=== SYSTEM READY ===");
  Serial.println("AUTONOMOUS: 1=Cube, 2=Cylinder, 3=Hat, 4=Boat, 5=All");
  Serial.println("MANUAL: B[angle], S[angle], E[angle], W[angle], G[angle]");
  Serial.println("ACTIONS: O=Open, C=Close, R=Reset, T=Test");
  Serial.println();
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
  
  switch(cmd) {
    case '1':
      Serial.println(">>> Starting CUBE sequence");
      pickAndPlaceCube();
      Serial.println(">>> CUBE sequence COMPLETED");
      break;
      
    case '2':
      Serial.println(">>> Starting CYLINDER sequence");
      pickAndPlaceCylinder();
      Serial.println(">>> CYLINDER sequence COMPLETED");
      break;
      
    case '3':
      Serial.println(">>> Starting HAT sequence");
      pickAndPlaceHat();
      Serial.println(">>> HAT sequence COMPLETED");
      break;
      
    case '4':
      Serial.println(">>> Starting BOAT sequence");
      pickAndPlaceBoat();
      Serial.println(">>> BOAT sequence COMPLETED");
      break;
      
    case '5':
      Serial.println(">>> Starting FULL sequence (all objects)");
      executeFullSequence();
      Serial.println(">>> FULL sequence COMPLETED");
      break;
      
    case 'B':
      setServoAngle(baseServo, command.substring(1).toInt(), BASE_MIN, BASE_MAX, "Base");
      break;
      
    case 'S':
      setServoAngle(shoulderServo, command.substring(1).toInt(), SHOULDER_MIN, SHOULDER_MAX, "Shoulder");
      break;
      
    case 'E':
      setServoAngle(elbowServo, command.substring(1).toInt(), ELBOW_MIN, ELBOW_MAX, "Elbow");
      break;
      
    case 'W':
      setServoAngle(wristServo, command.substring(1).toInt(), WRIST_MIN, WRIST_MAX, "Wrist");
      break;
      
    case 'G':
      setServoAngle(gripperServo, command.substring(1).toInt(), GRIPPER_OPEN, GRIPPER_CLOSE, "Gripper");
      break;
      
    case 'O':
      openGripper();
      break;
      
    case 'C':
      closeGripper();
      break;
      
    case 'R':
      resetArm();
      break;
      
    case 'T':
      testSequence();
      break;
      
    default:
      Serial.println("Unknown command. Send 1-5 for sequences");
      break;
  }
}

void setServoAngle(Servo &servo, int angle, int minAngle, int maxAngle, String name) {
  angle = constrain(angle, minAngle, maxAngle);
  servo.write(angle);
  Serial.println(name + " set to: " + String(angle));
  delay(500);
}

// ================= CORRECTED AUTONOMOUS SEQUENCES =================

void pickAndPlaceCube() {
  // Approach cube position 
  moveToPosition(180, 80, 150, 150, GRIPPER_OPEN);  // Shoulder up, elbow down
  delay(1500);
  
  // Lower to pick position
  moveToPosition(180, 80, 150, 150, GRIPPER_OPEN);   // Shoulder down, elbow up
  delay(1000);
  
  // Grab cube
  closeGripper();
  delay(1000);
  
  // Lift cube
  moveToPosition(200, 150, 60, 60, GRIPPER_CLOSE); // Shoulder up, elbow down
  delay(1000);
  
  // Move to frame position
  moveToPosition(200, 150, 65, 100, GRIPPER_CLOSE); // Base left, shoulder up
  delay(1500);
  
  // Lower to place position
  moveToPosition(200, 110, 55, 95, GRIPPER_CLOSE);  // Shoulder down, elbow up
  delay(1000);
  
  // Release cube
  openGripper();
  delay(1000);
  
  // Retract from frame
  moveToPosition(200, 150, 70, 105, GRIPPER_OPEN);  // Shoulder up, elbow down
  delay(1000);
  
  // Return home
  resetArm();
}

void pickAndPlaceCylinder() {
  // Approach cylinder position (INVERTED ANGLES)
  moveToPosition(120, 115, 55, 90, GRIPPER_OPEN);  // Base right, shoulder up
  delay(1500);
  
  // Lower to pick position
  moveToPosition(120, 105, 45, 85, GRIPPER_OPEN);  // Shoulder down, elbow up
  delay(1000);
  
  // Grab cylinder
  closeGripper();
  delay(1000);
  
  // Lift cylinder
  moveToPosition(120, 125, 65, 95, GRIPPER_CLOSE); // Shoulder up, elbow down
  delay(1000);
  
  // Move to frame position
  moveToPosition(30, 115, 60, 90, GRIPPER_CLOSE);  // Base left, shoulder up
  delay(1500);
  
  // Lower to place position
  moveToPosition(30, 105, 50, 85, GRIPPER_CLOSE);  // Shoulder down, elbow up
  delay(1000);
  
  // Release cylinder
  openGripper();
  delay(1000);
  
  // Retract from frame
  moveToPosition(30, 125, 65, 95, GRIPPER_OPEN);   // Shoulder up, elbow down
  delay(1000);
  
  // Return home
  resetArm();
}

void pickAndPlaceHat() {
  // Approach hat position (INVERTED ANGLES)
  moveToPosition(135, 110, 50, 80, GRIPPER_OPEN);  // Base right, shoulder up
  delay(1500);
  
  // Lower to pick position
  moveToPosition(135, 100, 40, 75, GRIPPER_OPEN);  // Shoulder down, elbow up
  delay(1000);
  
  // Grab hat
  closeGripper();
  delay(1000);
  
  // Lift hat
  moveToPosition(135, 120, 60, 85, GRIPPER_CLOSE); // Shoulder up, elbow down
  delay(1000);
  
  // Move to frame position
  moveToPosition(45, 110, 50, 80, GRIPPER_CLOSE);  // Base left, shoulder up
  delay(1500);
  
  // Lower to place position
  moveToPosition(45, 100, 40, 75, GRIPPER_CLOSE);  // Shoulder down, elbow up
  delay(1000);
  
  // Release hat
  openGripper();
  delay(1000);
  
  // Retract from frame
  moveToPosition(45, 120, 60, 85, GRIPPER_OPEN);   // Shoulder up, elbow down
  delay(1000);
  
  resetArm();
}

void pickAndPlaceBoat() {
  // Approach boat position (INVERTED ANGLES)
  moveToPosition(150, 105, 45, 70, GRIPPER_OPEN);  // Base right, shoulder up
  delay(1500);
  
  // Lower to pick position
  moveToPosition(150, 95, 35, 65, GRIPPER_OPEN);   // Shoulder down, elbow up
  delay(1000);
  
  // Grab boat
  closeGripper();
  delay(1000);
  
  // Lift boat
  moveToPosition(150, 115, 55, 75, GRIPPER_CLOSE); // Shoulder up, elbow down
  delay(1000);
  
  // Move to frame position
  moveToPosition(30, 105, 45, 70, GRIPPER_CLOSE);  // Base left, shoulder up
  delay(1500);
  
  // Lower to place position
  moveToPosition(30, 95, 35, 65, GRIPPER_CLOSE);   // Shoulder down, elbow up
  delay(1000);
  
  // Release boat
  openGripper();
  delay(1000);
  
  // Retract from frame
  moveToPosition(30, 115, 55, 75, GRIPPER_OPEN);   // Shoulder up, elbow down
  delay(1000);
  
  resetArm();
}

void executeFullSequence() {
  pickAndPlaceCube();
  delay(2000);
  pickAndPlaceCylinder();
  delay(2000);
  pickAndPlaceHat();
  delay(2000);
  pickAndPlaceBoat();
}

// ================= UTILITY FUNCTIONS =================

void moveToPosition(int base, int shoulder, int elbow, int wrist, int gripper) {
  base = constrain(base, BASE_MIN, BASE_MAX);
  shoulder = constrain(shoulder, SHOULDER_MIN, SHOULDER_MAX);
  elbow = constrain(elbow, ELBOW_MIN, ELBOW_MAX);
  wrist = constrain(wrist, WRIST_MIN, WRIST_MAX);
  gripper = constrain(gripper, GRIPPER_OPEN, GRIPPER_CLOSE);
  
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
}

void openGripper() {
  Serial.println("Opening gripper");
  gripperServo.write(GRIPPER_OPEN);
  delay(500);
}

void closeGripper() {
  Serial.println("Closing gripper");
  gripperServo.write(GRIPPER_CLOSE);
  delay(500);
}

void resetArm() {
  Serial.println("Resetting to home position");
  moveToPosition(90, 90, 90, 90, GRIPPER_OPEN);
  delay(1000);
}

void testSequence() {
  Serial.println("=== TESTING SERVO MOVEMENT ===");
  
  // Test base movement
  Serial.println("Testing base servo...");
  baseServo.write(60);
  delay(1000);
  baseServo.write(120);
  delay(1000);
  baseServo.write(90);
  delay(1000);
  
  // Test shoulder movement (inverted)
  Serial.println("Testing shoulder servo...");
  shoulderServo.write(120);  // Up position
  delay(1000);
  shoulderServo.write(60);   // Down position  
  delay(1000);
  shoulderServo.write(90);   // Middle
  delay(1000);
  
  // Test elbow movement (inverted)
  Serial.println("Testing elbow servo...");
  elbowServo.write(60);      // Down position
  delay(1000);
  elbowServo.write(120);     // Up position
  delay(1000);
  elbowServo.write(90);      // Middle
  delay(1000);
  
  // Test gripper
  Serial.println("Testing gripper...");
  openGripper();
  delay(1000);
  closeGripper();
  delay(1000);
  openGripper();
  delay(1000);
  
  resetArm();
  Serial.println("=== TEST COMPLETE ===");
}