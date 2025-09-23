//libraries
#include "DHT.h"

//pin definitions
#define DHTPIN 22        // Pin where the DHT22 data pin is connected
#define DHTTYPE DHT22   // Define sensor type


DHT dht(DHTPIN, DHTTYPE);
int fs_1a = A0; //fs_1 = flame sensor of node 1 analog pin
int gs_1a = A1; //gs_1 = gas sensor of node 1 analog pin


//value initialization
int gs_1a_val = 0;
int fs_1a_val = 0;

//backup
long randTemp;
long randHum;

void setup()
{
  Serial.begin(9600);

  //pin definition
  pinMode(DHTPIN, INPUT_PULLUP);

  dht.begin();
  Serial.println("DHT22 Sensor Reading...");
  randomSeed(analogRead(0));
  
}
void loop()
{
  fs_1a_val = analogRead(fs_1a); //reading flame sensor value
  gs_1a_val = analogRead(gs_1a); // reading gas sensor value
  randTemp = random(100,200);
  randHum = random(500, 600);
  
  
  delay(2000); // DHT22 needs ~2 seconds delay between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  float temperatureF = dht.readTemperature(true); // Fahrenheit

  //Testing purpose: the flame sensor r gas sensor is giving output though it is not connected. need to fex this !!!

  // Check if reading failed
  /*
  if (isnan(humidity) || isnan(temperature) || isnan(fs_1a_val) || isnan(gs_1a_val)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }
  */

  Serial.print("Flame sensor value:" );
  Serial.println(fs_1a_val);

  Serial.print("Gas sensor value:" );
  Serial.println(gs_1a_val);

  Serial.print("Humidity: ");
  Serial.print(randHum);
  Serial.print(" %\t");

  Serial.print(" | Temperature: ");
  Serial.print(randTemp);
  Serial.println(" °C  |  ");
}
