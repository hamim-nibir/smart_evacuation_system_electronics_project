int checker = 9; // checker led  
int fs_1a = A0; //fs_1 = flame sensor of node 1 analog pin
int fs_1a_val = 0;
void setup()
{
  pinMode(checker, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  fs_1a_val = analogRead(fs_1a);
  Serial.print("Flame sensor value:" );
  Serial.println(fs_1a_val);
  if(fs_1a_val<500){
    Serial.println("Flame Detected!");
    digitalWrite(checker, HIGH); // Led ON
  } else {
    Serial.println("No Flame Detected!");
    digitalWrite(checker, LOW); // Led OFF
  }
  delay(1000);
}
