// Mega_Sensor_Sender.ino
#include <DHT.h>

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

void setup() {
  Serial.begin(115200); // USB serial
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
}

void loop() {
  int flameVal1 = analogRead(flamePin1); // 0-1023
  int gasVal1   = analogRead(gasPin1);   // 0-1023
  int flameVal2 = analogRead(flamePin2); // 0-1023
  int gasVal2   = analogRead(gasPin2);   // 0-1023
  int flameVal3 = analogRead(flamePin3); // 0-1023
  int gasVal3   = analogRead(gasPin3);   // 0-1023
  int flameVal4 = analogRead(flamePin4); // 0-1023
  int gasVal4   = analogRead(gasPin4);   // 0-1023
  int flameVal5 = analogRead(flamePin5); // 0-1023
  int gasVal5   = analogRead(gasPin5);   // 0-1023

  float temp1 = NAN, hum1 = NAN;
  float temp2 = NAN, hum2 = NAN;
  float temp3 = NAN, hum3 = NAN;
  float temp4 = NAN, hum4 = NAN;
  float temp5 = NAN, hum5 = NAN;
  // if DHT available
  temp1 = dht1.readTemperature();
  hum1 = dht1.readHumidity();
  temp2 = dht2.readTemperature();
  hum2 = dht2.readHumidity();
  temp3 = dht3.readTemperature();
  hum3 = dht3.readHumidity();
  temp4 = dht4.readTemperature();
  hum4 = dht4.readHumidity();
  temp5 = dht5.readTemperature();
  hum5 = dht5.readHumidity();
  if (isnan(temp1)) temp1 = 0;
  if (isnan(hum1)) hum1 = 0;
  if (isnan(temp2)) temp2 = 0;
  if (isnan(hum2)) hum2 = 0;
  if (isnan(temp3)) temp3 = 0;
  if (isnan(hum3)) hum3 = 0;
  if (isnan(temp4)) temp4 = 0;
  if (isnan(hum4)) hum4 = 0;
  if (isnan(temp5)) temp5 = 0;
  if (isnan(hum5)) hum5 = 0;

  // Send CSV to Serial in format: corridor,flame,gas,temp,hum
  // Example: 1,234,512,28.6,61.2
  Serial.print(corridor1_id); Serial.print(",");
  Serial.print(flameVal1); Serial.print(",");
  Serial.print(gasVal1); Serial.print(",");
  Serial.print(temp1); Serial.print(",");
  Serial.println(hum1);

  Serial.print(corridor2_id); Serial.print(",");
  Serial.print(flameVal2); Serial.print(",");
  Serial.print(gasVal2); Serial.print(",");
  Serial.print(temp2); Serial.print(",");
  Serial.println(hum2);

  Serial.print(corridor3_id); Serial.print(",");
  Serial.print(flameVal3); Serial.print(",");
  Serial.print(gasVal3); Serial.print(",");
  Serial.print(temp3); Serial.print(",");
  Serial.println(hum3);

  Serial.print(corridor4_id); Serial.print(",");
  Serial.print(flameVal4); Serial.print(",");
  Serial.print(gasVal4); Serial.print(",");
  Serial.print(temp4); Serial.print(",");
  Serial.println(hum4);

  Serial.print(corridor5_id); Serial.print(",");
  Serial.print(flameVal5); Serial.print(",");
  Serial.print(gasVal5); Serial.print(",");
  Serial.print(temp5); Serial.print(",");
  Serial.println(hum5);

  delay(3000); // send every 3s -- adjust as needed
}
