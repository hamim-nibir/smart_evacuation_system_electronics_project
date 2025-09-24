
#define IR_1 7
#define IR_2 8 
#define buzzer 13

void setup() {
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Read sensor values
  int ir1State = digitalRead(IR_1);
  int ir2State = digitalRead(IR_2);

  if (ir1State == LOW) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }
}
