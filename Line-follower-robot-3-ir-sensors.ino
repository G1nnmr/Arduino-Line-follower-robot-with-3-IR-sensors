// IR sensor pins
#define IR_SENSOR_RIGHT  11
#define IR_SENSOR_CENTER 2
#define IR_SENSOR_LEFT   12

// Motor driver pins
const int enableRightMotor = 6;
const int rightMotorPin1 = 7;
const int rightMotorPin2 = 8;

const int enableLeftMotor = 5;
const int leftMotorPin1 = 9;
const int leftMotorPin2 = 10;

// Adjust these values during testing
const int BASE_SPEED   = 150;
const int TURN_SPEED   = 85;
const int SEARCH_SPEED = 100;
const int MAX_SPEED    = 220;

int lastTurnDirection = 0;

void setup() {
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_CENTER, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  rotateMotor(0, 0);
}

void loop() {
  // Assumes LOW means the sensor sees black
  bool leftOnLine =
      (digitalRead(IR_SENSOR_LEFT) == LOW);

  bool centerOnLine =
      (digitalRead(IR_SENSOR_CENTER) == LOW);

  bool rightOnLine =
      (digitalRead(IR_SENSOR_RIGHT) == LOW);

  
  if (leftOnLine && centerOnLine && rightOnLine) {
    rotateMotor(BASE_SPEED, BASE_SPEED);
  }

  // Line centered: move straight
  else if (!leftOnLine && centerOnLine && !rightOnLine) {
    rotateMotor(BASE_SPEED, BASE_SPEED);
  }

  // Line is toward the left
  else if (leftOnLine && centerOnLine && !rightOnLine) {
    rotateMotor(BASE_SPEED, TURN_SPEED);
    lastTurnDirection = -1;
  }

  // Line is toward the right
  else if (!leftOnLine && centerOnLine && rightOnLine) {
    rotateMotor(TURN_SPEED, BASE_SPEED);
    lastTurnDirection = 1;
  }

  // Sharp left correction
  else if (leftOnLine && !centerOnLine && !rightOnLine) {
    rotateMotor(BASE_SPEED, -BASE_SPEED);
    lastTurnDirection = -1;
  }

  // Sharp right correction
  else if (!leftOnLine && !centerOnLine && rightOnLine) {
    rotateMotor(-BASE_SPEED, BASE_SPEED);
    lastTurnDirection = 1;
  }

  else if (leftOnLine && !centerOnLine && rightOnLine) {
    rotateMotor(BASE_SPEED, BASE_SPEED);
  }

  
  else {
    if (lastTurnDirection == -1) {
      rotateMotor(-SEARCH_SPEED, SEARCH_SPEED);
    }
    else if (lastTurnDirection == 1) {
      rotateMotor(SEARCH_SPEED, -SEARCH_SPEED);
    }
    else {
      rotateMotor(SEARCH_SPEED, SEARCH_SPEED);
    }
  }
}


// Positive = forward; negative = reverse
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  rightMotorSpeed = constrain(
      rightMotorSpeed, -MAX_SPEED, MAX_SPEED);
  leftMotorSpeed = constrain(
      leftMotorSpeed, -MAX_SPEED, MAX_SPEED);

  // Right motor direction
  if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  // Left motor direction
  if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
