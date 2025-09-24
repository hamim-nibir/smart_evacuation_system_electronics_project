#define IR_1 7
#define IR_2 8 
#define buzzer 13

volatile long count = 0;  // counter variable

int firstTriggered = 0;   // 0 = none, 1 = IR_1 first, 2 = IR_2 first

void setup() {
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ir1State = digitalRead(IR_1);
  int ir2State = digitalRead(IR_2);

  // Detect first trigger
  if (firstTriggered == 0) {
    if (ir1State == LOW) {
      firstTriggered = 1; // IR_1 first
    } else if (ir2State == LOW) {
      firstTriggered = 2; // IR_2 first
    }
  }

  // If IR_1 was first and IR_2 also triggered → count++
  if (firstTriggered == 1 && ir2State == LOW) {
    count++;
    resetTrigger();
  }

  // If IR_2 was first and IR_1 also triggered → count--
  if (firstTriggered == 2 && ir1State == LOW) {
    count--;
    resetTrigger();
  }

  // Example buzzer: ON if count > 10
  if (count > 10) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  // Always print latest count
  Serial.print("Count: ");
  Serial.println(count);

  delay(50); // small delay just to reduce serial spam
}

// Reset after one full crossing
void resetTrigger() {
  // Wait until both sensors are clear
  while (digitalRead(IR_1) == LOW || digitalRead(IR_2) == LOW) {
    delay(5); // check quickly, don’t block for too long
  }
  firstTriggered = 0;
}
