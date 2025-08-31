//libraries
#include "DHT.h"

//pin definitions
#define DHTPIN 22        // Pin where the DHT22 data pin is connected
#define DHTTYPE DHT22   // Define sensor type
#define TRIG_PIN_1 23 //sonar 1 trig pin
#define ECHO_PIN_1 24 //sonar 1 echo pin
#define TRIG_PIN_2 25 //sonar 2 trig pin
#define ECHO_PIN_2 26 //sonar 2 echo pin


DHT dht(DHTPIN, DHTTYPE);
int fs_1a = A0; //fs_1 = flame sensor of node 1 analog pin
int gs_1a = A1; //gs_1 = gas sensor of node 1 analog pin


//value initialization
int gs_1a_val = 0;
int fs_1a_val = 0;
long duration_1;
long duration_2;
float distance_1;
float distance_2;

void setup()
{
  Serial.begin(9600);

  //pin definition
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  dht.begin();
  Serial.println("DHT22 Sensor Reading...");
  
}
void loop()
{
  fs_1a_val = analogRead(fs_1a); //reading flame sensor value
  gs_1a_val = analogRead(gs_1a); // reading gas sensor value
  
  
  delay(2000); // DHT22 needs ~2 seconds delay between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  float temperatureF = dht.readTemperature(true); // Fahrenheit

  //Testing purpose: the flame sensor r gas sensor is giving output though it is not connected. need to fex this !!!
  Serial.print("Flame sensor value:" );
  Serial.println(fs_1a_val);
  Serial.print("Gas sensor value:" );
  Serial.println(gs_1a_val);
  //Testing purpose: the flame sensor r gas sensor is giving output though it is not connected. need to fex this !!!

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature) || isnan(fs_1a_val) || isnan(gs_1a_val)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  //ultrasonic sensors
    // Clear the TRIG pin
  digitalWrite(TRIG_PIN_1, LOW);
  digitalWrite(TRIG_PIN_2, LOW);
  delayMicroseconds(2);

  // Send 10 µs pulse to TRIG
  digitalWrite(TRIG_PIN_1, HIGH);
  digitalWrite(TRIG_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_1, LOW);
  digitalWrite(TRIG_PIN_2, LOW);

  // Read the ECHO pulse width
  duration_1 = pulseIn(ECHO_PIN_1, HIGH);
  duration_2 = pulseIn(ECHO_PIN_2, HIGH);

  // Calculate distance in cm
  distance_1 = (duration_1 * 0.0343) / 2;
  distance_2 = (duration_2 * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance_1);
  Serial.println(" cm");
  Serial.print("Distance: ");
  Serial.print(distance_2);
  Serial.println(" cm");

  //ultrasonic sensors

  Serial.print("Flame sensor value:" );
  Serial.println(fs_1a_val);

  Serial.print("Gas sensor value:" );
  Serial.println(gs_1a_val);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  |  ");
  Serial.print(temperatureF);
  Serial.println(" °F");
}
