// Motor A control pins
#define ENA 5   // PWM pin for speed
#define IN1 2
#define IN2 3

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  stopMotor();  // start with motor off
}

void loop() {
  // Move forward
  moveForward(200);   // speed range: 0–255
  delay(2000);

  // Stop
  stopMotor();
  delay(1000);

  // Move backward
  moveBackward(200);
  delay(2000);

  // Stop
  stopMotor();
  delay(2000);
}

// ===== Motor Functions =====
void moveForward(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedVal);
}

void moveBackward(int speedVal) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedVal);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}
