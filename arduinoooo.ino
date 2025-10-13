// Mega_Sensor_Sender.ino
#include <DHT.h>
#include <Servo.h>

// ===================== Pins =====================
#define LED1_R1 30
#define LED2_R1 31
#define LED1_R3 32
#define LED2_R3 33

#define SERVO_PIN 9

// ===================== Corridor led Pins =====================
#define LED_c1 52
#define LED_c2 53

Servo exitServo;
int lastServoPos = 0; // track servo position

#define DHTPIN1 22
#define DHTPIN2 23
#define DHTPIN3 24
#define DHTPIN4 25
#define DHTPIN5 26

#define DHTTYPE DHT22   // change to DHT11 if that's what you have
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);

const int flamePin1 = A0;
const int gasPin1   = A1;
const int flamePin2 = A2;
const int gasPin2   = A3;
const int flamePin3 = A4;
const int gasPin3   = A5;
const int flamePin4 = A6;
const int gasPin4   = A7;
const int flamePin5 = A8;
const int gasPin5   = A9;

const int corridor1_id = 1;
const int corridor2_id = 2;
const int corridor3_id = 3;
const int corridor4_id = 4;
const int corridor5_id = 5;

// ===================== Human Counting =====================
// Entrance sensors
#define EN_IR1 7
#define EN_IR2 8

// Corridor 4 exit sensors
#define C4_IR1 10
#define C4_IR2 11

// Corridor 5 exit sensors
#define C5_IR1 12
#define C5_IR2 13

volatile long totalHumans = 0;
volatile long escapedC4 = 0;
volatile long escapedC5 = 0;

// Trackers
int enFirstTriggered = 0;
int c4FirstTriggered = 0;
int c5FirstTriggered = 0;

// Reset functions with timeout
void resetTrigger(int &firstTrig, int pin1, int pin2) {
  unsigned long start = millis();
  while ((digitalRead(pin1) == LOW || digitalRead(pin2) == LOW) && millis() - start < 1000) {
    delay(5);
  }
  firstTrig = 0;
}

// Entrance count
void handleEntranceCount() {
  int ir1State = digitalRead(EN_IR1);
  int ir2State = digitalRead(EN_IR2);

  if (enFirstTriggered == 0) {
    if (ir1State == LOW) enFirstTriggered = 1;
    else if (ir2State == LOW) enFirstTriggered = 2;
  }

  if (enFirstTriggered == 1 && ir2State == LOW) {
    totalHumans++; // entering
    resetTrigger(enFirstTriggered, EN_IR1, EN_IR2);
  }
  if (enFirstTriggered == 2 && ir1State == LOW) {
    totalHumans--; // exiting
    if (totalHumans < 0) totalHumans = 0;
    resetTrigger(enFirstTriggered, EN_IR1, EN_IR2);
  }
}

// Corridor 4 escape count
void handleC4Count() {
  int ir1State = digitalRead(C4_IR1);
  int ir2State = digitalRead(C4_IR2);

  if (c4FirstTriggered == 0) {
    if (ir1State == LOW) c4FirstTriggered = 1;
    else if (ir2State == LOW) c4FirstTriggered = 2;
  }

  if (c4FirstTriggered == 1 && ir2State == LOW) {
    escapedC4++;
    totalHumans--;
    if (totalHumans < 0) totalHumans = 0;
    resetTrigger(c4FirstTriggered, C4_IR1, C4_IR2);
  }
  if (c4FirstTriggered == 2 && ir1State == LOW) {
    resetTrigger(c4FirstTriggered, C4_IR1, C4_IR2); // just reset
  }
}

// Corridor 5 escape count
void handleC5Count() {
  int ir1State = digitalRead(C5_IR1);
  int ir2State = digitalRead(C5_IR2);

  if (c5FirstTriggered == 0) {
    if (ir1State == LOW) c5FirstTriggered = 1;
    else if (ir2State == LOW) c5FirstTriggered = 2;
  }

  if (c5FirstTriggered == 1 && ir2State == LOW) {
    escapedC5++;
    totalHumans--;
    if (totalHumans < 0) totalHumans = 0;
    resetTrigger(c5FirstTriggered, C5_IR1, C5_IR2);
  }
  if (c5FirstTriggered == 2 && ir1State == LOW) {
    resetTrigger(c5FirstTriggered, C5_IR1, C5_IR2);
  }
}

void setup() {
  pinMode(LED1_R1, OUTPUT);
  pinMode(LED2_R1, OUTPUT);
  pinMode(LED1_R3, OUTPUT);
  pinMode(LED2_R3, OUTPUT);

  pinMode(LED_c1, OUTPUT);
  pinMode(LED_c2, OUTPUT);

  exitServo.attach(SERVO_PIN);

  // IR pins
  pinMode(EN_IR1, INPUT_PULLUP);
  pinMode(EN_IR2, INPUT_PULLUP);
  pinMode(C4_IR1, INPUT_PULLUP);
  pinMode(C4_IR2, INPUT_PULLUP);
  pinMode(C5_IR1, INPUT_PULLUP);
  pinMode(C5_IR2, INPUT_PULLUP);

  Serial.begin(115200); // USB serial
  Serial.println("System Started...");

  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
}

void loop() {
  // Update human counts
  handleEntranceCount();
  handleC4Count();
  handleC5Count();

  int flameVal1 = analogRead(flamePin1);
  int gasVal1   = analogRead(gasPin1);
  int flameVal2 = analogRead(flamePin2);
  int gasVal2   = analogRead(gasPin2);
  int flameVal3 = analogRead(flamePin3);
  int gasVal3   = analogRead(gasPin3);
  int flameVal4 = analogRead(flamePin4);
  int gasVal4   = analogRead(gasPin4);
  int flameVal5 = analogRead(flamePin5);
  int gasVal5   = analogRead(gasPin5);

  float temp1 = dht1.readTemperature(); float hum1 = dht1.readHumidity();
  float temp2 = dht2.readTemperature(); float hum2 = dht2.readHumidity();
  float temp3 = dht3.readTemperature(); float hum3 = dht3.readHumidity();
  float temp4 = dht4.readTemperature(); float hum4 = dht4.readHumidity();
  float temp5 = dht5.readTemperature(); float hum5 = dht5.readHumidity();

  if (isnan(temp1)) temp1 = 0; if (isnan(hum1)) hum1 = 0;
  if (isnan(temp2)) temp2 = 0; if (isnan(hum2)) hum2 = 0;
  if (isnan(temp3)) temp3 = 0; if (isnan(hum3)) hum3 = 0;
  if (isnan(temp4)) temp4 = 0; if (isnan(hum4)) hum4 = 0;
  if (isnan(temp5)) temp5 = 0; if (isnan(hum5)) hum5 = 0;

  // ===== Risk calculations =====
  int risk1 = (flameVal1 < 700) ? 15 : (flameVal1 < 900) ? 10 : 5;
  int risk2 = (flameVal2 < 700) ? 15 : (flameVal2 < 900) ? 10 : 5;
  int risk3 = (flameVal3 < 700) ? 15 : (flameVal3 < 900) ? 10 : 5;
  int risk4 = (flameVal4 < 700) ? 15 : (flameVal4 < 900) ? 10 : 5;
  int risk5 = (flameVal5 < 700) ? 15 : (flameVal5 < 900) ? 10 : 5;

  digitalWrite(LED2_R1, risk1 == 15 ? HIGH : LOW); 
  digitalWrite(LED1_R1, risk2 == 15 ? HIGH : LOW);
  digitalWrite(LED_c1, risk1 == 15 ? HIGH : LOW);

  digitalWrite(LED2_R3, risk2 == 15 ? HIGH : LOW); 
  digitalWrite(LED1_R3, risk3 == 15 ? HIGH : LOW);
  digitalWrite(LED_c2, risk2 == 15 ? HIGH : LOW); 

  // Servo logic
  if (risk4 == 15 && lastServoPos != 90) {
    exitServo.write(90);
    lastServoPos = 90;
  } else if (risk4 != 15 && lastServoPos != 0) {
    exitServo.write(0);
    lastServoPos = 0;
  }

  // ===== Serial Output =====
  Serial.print(corridor1_id); Serial.print(","); Serial.print(flameVal1); Serial.print(","); Serial.print(gasVal1); Serial.print(","); Serial.print(temp1); Serial.print(","); Serial.println(hum1);
  Serial.print(corridor2_id); Serial.print(","); Serial.print(flameVal2); Serial.print(","); Serial.print(gasVal2); Serial.print(","); Serial.print(temp2); Serial.print(","); Serial.println(hum2);
  Serial.print(corridor3_id); Serial.print(","); Serial.print(flameVal3); Serial.print(","); Serial.print(gasVal3); Serial.print(","); Serial.print(temp3); Serial.print(","); Serial.println(hum3);
  Serial.print(corridor4_id); Serial.print(","); Serial.print(flameVal4); Serial.print(","); Serial.print(gasVal4); Serial.print(","); Serial.print(temp4); Serial.print(","); Serial.println(hum4);
  Serial.print(corridor5_id); Serial.print(","); Serial.print(flameVal5); Serial.print(","); Serial.print(gasVal5); Serial.print(","); Serial.print(temp5); Serial.print(","); Serial.println(hum5);

  // Human counts
  Serial.print("COUNT,"); 
  Serial.print(totalHumans); Serial.print(",");
  Serial.print(escapedC4); Serial.print(",");
  Serial.println(escapedC5);

  delay(3000);
}
