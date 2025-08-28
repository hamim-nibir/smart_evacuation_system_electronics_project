#include "DHT.h"

#define DHTPIN 2        // Pin where the DHT22 data pin is connected
#define DHTTYPE DHT22   // Define sensor type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();
  Serial.println("DHT22 Sensor Reading...");
}

void loop() {
  delay(2000); // DHT22 needs ~2 seconds delay between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  float temperatureF = dht.readTemperature(true); // Fahrenheit

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  |  ");
  Serial.print(temperatureF);
  Serial.println(" °F");
}
