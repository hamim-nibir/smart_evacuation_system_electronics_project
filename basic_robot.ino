// Pin Definitions
// Motors
#define IN1 14
#define IN2 12
#define IN3 13
#define IN4 15

// Sonars
#define TRIG1 5
#define ECHO1 18
#define TRIG2 19
#define ECHO2 21

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sonar pins
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
}

// Simple functions to move the robot
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Function to read distance from HC-SR04
long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

void loop() {
  // Example: move forward for 2 seconds
  forward();
  delay(2000);
  stopMotors();
  delay(1000);

  // Read distances
  long frontDist = readDistance(TRIG1, ECHO1);
  long backDist = readDistance(TRIG2, ECHO2);

  Serial.print("Front Distance: ");
  Serial.print(frontDist);
  Serial.print(" cm, Back Distance: ");
  Serial.print(backDist);
  Serial.println(" cm");

  delay(1000);

  // Example: move backward for 2 seconds
  backward();
  delay(2000);
  stopMotors();
  delay(1000);
}
