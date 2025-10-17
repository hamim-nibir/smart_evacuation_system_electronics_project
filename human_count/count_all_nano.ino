#include <Wire.h>

// ==== Function Declarations ====
void entranceLogic();
void exitLogicC4();
void exitLogicC5();
void resetTrigger();
void sendCounts();

// I2C address of Nano (slave)
#define NANO_ADDR 0x08  

// Entrance IR sensors
#define ENTR_IR1 2
#define ENTR_IR2 3

// Exit C4 sensors
#define C4_IR1 4
#define C4_IR2 5

// Exit C5 sensors
#define C5_IR1 6
#define C5_IR2 7

// Optional buzzer
#define BUZZER 13

volatile long totalCount = 0;  // Total number of people
volatile long escC4 = 0;       // Number of people escaped via C4
volatile long escC5 = 0;       // Number of people escaped via C5

int firstTriggered = 0;        // 0 = none, 1 = IR1 first, 2 = IR2 first

// ==========================
//       SETUP FUNCTION
// ==========================
void setup() {
  pinMode(ENTR_IR1, INPUT);
  pinMode(ENTR_IR2, INPUT);
  pinMode(C4_IR1, INPUT);
  pinMode(C4_IR2, INPUT);
  pinMode(C5_IR1, INPUT);
  pinMode(C5_IR2, INPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(NANO_ADDR);       // Join I2C as slave
  Wire.onRequest(sendCounts);  // Callback when ESP requests data
  Serial.begin(9600);

  Serial.println("=== Human Counter System Started ===");
}

// ==========================
//        MAIN LOOP
// ==========================
void loop() {
  entranceLogic();   // Count people entering/exiting at main door
  exitLogicC4();     // Count people escaping via C4
  exitLogicC5();     // Count people escaping via C5

  // Optional buzzer alert
  if (totalCount > 10) digitalWrite(BUZZER, HIGH);
  else digitalWrite(BUZZER, LOW);

  // Print counts to Serial Monitor
  Serial.print("Total Count: ");
  Serial.print(totalCount);
  Serial.print(" | Escaped via C4: ");
  Serial.print(escC4);
  Serial.print(" | Escaped via C5: ");
  Serial.println(escC5);

  delay(100);
}

// ==========================
//  ENTRANCE SMART LOGIC
// ==========================
void entranceLogic() {
  int ir1State = digitalRead(ENTR_IR1);
  int ir2State = digitalRead(ENTR_IR2);

  // Detect which sensor triggered first
  if (firstTriggered == 0) {
    if (ir1State == LOW) firstTriggered = 1;  // IR1 first → entering
    else if (ir2State == LOW) firstTriggered = 2;  // IR2 first → exiting
  }

  // IR1 first, then IR2 → person entered
  if (firstTriggered == 1 && ir2State == LOW) {
    totalCount++;
    Serial.println("→ Person Entered");
    resetTrigger();
  }

  // IR2 first, then IR1 → person exited
  if (firstTriggered == 2 && ir1State == LOW) {
    if (totalCount > 0) totalCount--;
    Serial.println("← Person Exited (Entrance)");
    resetTrigger();
  }
}

// ==========================
//  RESET TRIGGER FUNCTION
// ==========================
void resetTrigger() {
  // Wait until both sensors are clear again
  while (digitalRead(ENTR_IR1) == LOW || digitalRead(ENTR_IR2) == LOW) {
    delay(5);
  }
  firstTriggered = 0;
}

// ==========================
//    EXIT LOGIC FOR C4
// ==========================
void exitLogicC4() {
  if (digitalRead(C4_IR1) == LOW) {
    delay(50);  // small debounce
    if (digitalRead(C4_IR2) == LOW) {
      escC4++;
      Serial.println("→ Person Escaped via C4");
      while (digitalRead(C4_IR2) == LOW);  // wait until released
    }
  }
}

// ==========================
//    EXIT LOGIC FOR C5
// ==========================
void exitLogicC5() {
  if (digitalRead(C5_IR1) == LOW) {
    delay(50);  // small debounce
    if (digitalRead(C5_IR2) == LOW) {
      escC5++;
      Serial.println("→ Person Escaped via C5");
      while (digitalRead(C5_IR2) == LOW);  // wait until released
    }
  }
}

// ==========================
//     I2C SEND FUNCTION
// ==========================
void sendCounts() {
  String data = String(totalCount) + "," + String(escC4) + "," + String(escC5);
  Wire.write(data.c_str());
}
