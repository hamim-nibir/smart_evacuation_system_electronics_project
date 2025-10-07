// Mega_Sensor_Sender.ino
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22   // change to DHT11 if that's what you have
DHT dht(DHTPIN, DHTTYPE);

const int flamePin = A0;
const int gasPin   = A1;

const int corridor_id = 1; // change per physical corridor if sending from multiple Megas

void setup() {
  Serial.begin(115200); // USB serial
  dht.begin();
}

void loop() {
  int flameVal = analogRead(flamePin); // 0-1023
  int gasVal   = analogRead(gasPin);   // 0-1023

  float temp = NAN, hum = NAN;
  // if DHT available
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(temp)) temp = 0;
  if (isnan(hum)) hum = 0;

  // Send CSV to Serial in format: corridor,flame,gas,temp,hum
  // Example: 1,234,512,28.6,61.2
  Serial.print(corridor_id); Serial.print(",");
  Serial.print(flameVal); Serial.print(",");
  Serial.print(gasVal); Serial.print(",");
  Serial.print(temp); Serial.print(",");
  Serial.println(hum);

  delay(3000); // send every 3s -- adjust as needed
}
