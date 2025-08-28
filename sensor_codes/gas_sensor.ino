int checker = 9; // checker led  
int gs_1a = A0; //gs_1 = gas sensor of node 1 analog pin
int gs_1a_val = 0;
void setup()
{
  pinMode(checker, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  gs_1a_val = analogRead(gs_1a);
  Serial.print("Gas sensor value:" );
  Serial.println(gs_1a_val);
  if(gs_1a_val<500){
    Serial.println("Gas Detected!");
    digitalWrite(checker, HIGH); // Led ON
  } else {
    Serial.println(" Clean Air!");
    digitalWrite(checker, LOW); // Led OFF
  }
  delay(1000);
}
