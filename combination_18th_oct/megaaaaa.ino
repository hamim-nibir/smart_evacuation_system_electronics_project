// Mega_Sensor_Sender.ino
#include <DHT.h>
#include <Servo.h>
#include <Wire.h>

#define NANO_ADDR 0x08

long totalCount = 0;
long escC4 = 0;
long escC5 = 0;

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

#define DHTTYPE DHT22
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

void setup() {
  Wire.begin();           // Mega as I2C Master
  Serial.begin(115200);   // USB serial
  Serial.println("=== Mega Started (Master) ===");

  pinMode(LED1_R1, OUTPUT);
  pinMode(LED2_R1, OUTPUT);
  pinMode(LED1_R3, OUTPUT);
  pinMode(LED2_R3, OUTPUT);

  pinMode(LED_c1, OUTPUT);
  pinMode(LED_c2, OUTPUT);

  exitServo.attach(SERVO_PIN);

  dht1.begin(); dht2.begin(); dht3.begin(); dht4.begin(); dht5.begin();
}

void loop() {
  // ===== 1. Request human counts from Nano =====
  Wire.requestFrom(NANO_ADDR, 20);  
  String data = "";
  while (Wire.available()) {
    char c = Wire.read();
    data += c;
  }

  if (data.length() > 0) {
    int idx1 = data.indexOf(',');
    int idx2 = data.indexOf(',', idx1+1);
    if (idx1 > 0 && idx2 > idx1) {
      totalCount = data.substring(0, idx1).toInt();
      escC4 = data.substring(idx1+1, idx2).toInt();
      escC5 = data.substring(idx2+1).toInt();
    }
  }

  // ===== 2. Read corridor sensors =====
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

  // ===== 3. Risk calculations =====
  int risk1 = (flameVal1 < 400) ? 15 : (flameVal1 < 900) ? 10 : 5;
  int risk2 = (flameVal2 < 400) ? 15 : (flameVal2 < 900) ? 10 : 5;
  int risk3 = (flameVal3 < 400) ? 15 : (flameVal3 < 900) ? 10 : 5;
  int risk4 = (flameVal4 < 400) ? 15 : (flameVal4 < 900) ? 10 : 5;
  int risk5 = (flameVal5 < 400) ? 15 : (flameVal5 < 900) ? 10 : 5;

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

  // ===== 4. Send to Serial (ESP reads this) =====
  Serial.print(totalCount); Serial.print(",");
  Serial.print(escC4);      Serial.print(",");
  Serial.print(escC5);      Serial.println(" ");

  Serial.print(corridor1_id); Serial.print(","); Serial.print(flameVal1); Serial.print(","); Serial.print(gasVal1); Serial.print(","); Serial.print(temp1); Serial.print(","); Serial.println(hum1);
  Serial.print(corridor2_id); Serial.print(","); Serial.print(flameVal2); Serial.print(","); Serial.print(gasVal2); Serial.print(","); Serial.print(temp2); Serial.print(","); Serial.println(hum2);
  Serial.print(corridor3_id); Serial.print(","); Serial.print(flameVal3); Serial.print(","); Serial.print(gasVal3); Serial.print(","); Serial.print(temp3); Serial.print(","); Serial.println(hum3);
  Serial.print(corridor4_id); Serial.print(","); Serial.print(flameVal4); Serial.print(","); Serial.print(gasVal4); Serial.print(","); Serial.print(temp4); Serial.print(","); Serial.println(hum4);
  Serial.print(corridor5_id); Serial.print(","); Serial.print(flameVal5); Serial.print(","); Serial.print(gasVal5); Serial.print(","); Serial.print(temp5); Serial.print(","); Serial.println(hum5);

  delay(3000);
}
