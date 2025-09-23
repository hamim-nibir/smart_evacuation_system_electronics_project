long randTemp;
long randHum;
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop() {
  // put your main code here, to run repeatedly:
  randTemp = random(100,200);
  randHum = random(500, 600);
  delay(500);
  Serial.println("Temperature: ");
  Serial.print(randTemp);
  Serial.println("Humidity: ");
  Serial.print(randHum);

}
