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
  pinMode(23, OUTPUT);

  dht.begin();
  Serial.println("DHT22 Sensor Reading...");
  randomSeed(analogRead(0));
  
}
void loop()
{
  digitalWrite(23, LOW);
  fs_1a_val = analogRead(fs_1a); //reading flame sensor value
  gs_1a_val = analogRead(gs_1a); // reading gas sensor value
  randTemp = random(25,30);
  long temp1 = randTemp;
  randHum = random(30, 32);
  long humid1 = randHum;
  
  
  // delay(5000); // DHT22 needs ~2 seconds delay between readings

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
  Serial.print(humid1);
  Serial.print(" %\t");

  Serial.print(" | Temperature: ");
  Serial.print(temp1);
  Serial.println(" °C  |  ");

  //sending feedback
  if((humid1<=35) && (temp1<=30) && (fs_1a_val>=1015) && (gs_1a_val<=320)){
    Serial.println("No risk! Everything is Fine!");
    digitalWrite(23, LOW);
    delay(500); 
  }
  else if((humid1>35 && temp1>30) || (fs_1a_val>=800 && fs_1a_val<=950) || (gs_1a_val>650 && gs_1a_val <= 750)){
      Serial.println("Medium Risk!");
      digitalWrite(23, HIGH);
      delay(500); 
  }
  else if((humid1>35 && temp1>30) || (fs_1a_val<750) || (gs_1a_val>800)){
    Serial.println("High Risk!");
    digitalWrite(23, HIGH);
    delay(500); 
  }
}
