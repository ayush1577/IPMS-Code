#include <Servo.h>  // Library for controlling servo motors
Servo myservo1;     // Entry gate servo
Servo myservo2;     // Exit gate servo

#include <LiquidCrystal.h>            // Library for LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  // LCD pins (RS, E, D4, D5, D6, D7)

// Red LEDs for occupied slots
int redLED[8] = { 31, 32, 33, 34, 35, 36, 37, 38 };

// Green LEDs for vacant slots
int greenLED[8] = { 21, 22, 23, 24, 25, 26, 27, 28 };

/// IR Proximity Sensors pin (1 per slot + entry + exit)
int ir1 = 9, ir2 = 10, ir3 = 11, ir4 = 12, ir5 = 13, ir6 = 14, ir7 = 15, ir8 = 16;
int ext = 19, entry = 18;  // Exit and Entry IR sensors
int count, total,buzz=42,entrygreen=41,entryred=40;          // count = vacant slots, total = total vehicles inside

/// IR Proximity Sensors Initial Value
int status1 = 0, status2 = 0, status3 = 0, status4 = 0;
int status5 = 0, status6 = 0, status7 = 0, status8 = 0;
int statusext = 0, statusentry = 0;  // For entry and exit IR sensors

void setup() {
  /// Set slot IR sensor pins as input
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
  pinMode(ir6, INPUT);
  pinMode(ir7, INPUT);
  pinMode(ir8, INPUT);
  
  pinMode(buzz, OUTPUT);
  pinMode(entrygreen, OUTPUT);
  pinMode(entryred, OUTPUT);


  /// Set slot LEDs as output
  for (int i = 0; i < 8; i++) {
    pinMode(greenLED[i], OUTPUT);
    pinMode(redLED[i], OUTPUT);
  }

  /// Entry and Exit IR pins
  pinMode(ext, INPUT);
  pinMode(entry, INPUT);

  /// Initialize LCD
  lcd.begin(16, 2);

  /// Attach servos to pins
  myservo1.attach(8);   // Entry gate servo
  myservo1.write(0);    // Keep entry gate closed initially
  myservo2.attach(17);  // Exit gate servo
  myservo2.write(0);    // Keep exit gate closed initially

  total = 0;  // Start with no vehicles
}

void loop() {
  
  /// Read IR sensors for slots
  status1 = digitalRead(ir1);
  status2 = digitalRead(ir2);
  status3 = digitalRead(ir3);
  status4 = digitalRead(ir4);
  status5 = digitalRead(ir5);
  status6 = digitalRead(ir6);
  status7 = digitalRead(ir7);
  status8 = digitalRead(ir8);

  /// Read entry and exit sensors
  statusext = digitalRead(ext);
  statusentry = digitalRead(entry);
  digitalWrite(entrygreen,HIGH);
  /// Start assuming all slots are vacant
  count = 8;

  /// Show initial LCD messages
  lcd.setCursor(0, 0);
  lcd.print("    Vacant   :");
  lcd.setCursor(0, 1);
  lcd.print("Total Vehicle:");

  /// Slot checking logic (if sensor detects a car = occupied)
  if (status1 == 1) {
    count--;
    digitalWrite(redLED[0], HIGH);
    digitalWrite(greenLED[0], LOW);
  } else {
    digitalWrite(greenLED[0], HIGH);
    digitalWrite(redLED[0], LOW);
  }

  if (status2 == 1) {
    count--;
    digitalWrite(redLED[1], HIGH);
    digitalWrite(greenLED[1], LOW);
  } else {
    digitalWrite(greenLED[1], HIGH);
    digitalWrite(redLED[1], LOW);
  }

  if (status3 == 1) {
    count--;
    digitalWrite(redLED[2], HIGH);
    digitalWrite(greenLED[2], LOW);
  } else {
    digitalWrite(greenLED[2], HIGH);
    digitalWrite(redLED[2], LOW);
  }

  if (status4 == 1) {
    count--;
    digitalWrite(redLED[3], HIGH);
    digitalWrite(greenLED[3], LOW);
  } else {
    digitalWrite(greenLED[3], HIGH);
    digitalWrite(redLED[3], LOW);
  }

  if (status5 == 1) {
    count--;
    digitalWrite(redLED[4], HIGH);
    digitalWrite(greenLED[4], LOW);
  } else {
    digitalWrite(greenLED[4], HIGH);
    digitalWrite(redLED[4], LOW);
  }

  if (status6 == 1) {
    count--;
    digitalWrite(redLED[5], HIGH);
    digitalWrite(greenLED[5], LOW);
  } else {
    digitalWrite(greenLED[5], HIGH);
    digitalWrite(redLED[5], LOW);
  }

  if (status7 == 1) {
    count--;
    digitalWrite(redLED[6], HIGH);
    digitalWrite(greenLED[6], LOW);
  } else {
    digitalWrite(greenLED[6], HIGH);
    digitalWrite(redLED[6], LOW);
  }

  if (status8 == 1) {
    count--;
    digitalWrite(redLED[7], HIGH);
    digitalWrite(greenLED[7], LOW);
  } else {
    digitalWrite(greenLED[7], HIGH);
    digitalWrite(redLED[7], LOW);
  }

  /// If no slots available
  lcd.setCursor(0, 0);
  if (count == 0) {
    digitalWrite(entrygreen,LOW);
    digitalWrite(entryred,HIGH);

    lcd.print("  No Vacancy");  // Show "No Vacancy" on LCD
    lcd.setCursor(0, 1);
    lcd.print("Total Vehicle:");  // Show total vehicles
    lcd.setCursor(14, 1);
    lcd.print(total);
    delay(350);
    myservo1.write(0);  // Keep entry gate closed
  }
  else
    digitalWrite(entryred,LOW);
  

  /// Exit gate control
  static int prevEntry2 = 0;
  if (statusext == 1) {
    myservo2.write(80);  // Open exit gate when car detected
    
    delay(2000);
  
  } 
  prevEntry2 = statusext;  // Update previous state
  if (statusext == 0) {
    delay(500);
    myservo2.write(0);  // Close entry gate
  }

  /// Entry gate control (using rising edge detection)
  static int prevEntry = 0;
  if (statusentry == 1 && prevEntry == 0) {
    if (count > 0) {        // Only allow entry if slot available
      myservo1.write(120);  // Open entry gate
      digitalWrite(buzz,HIGH);
      delay(2000);          // Wait for vehicle to pass
      digitalWrite(buzz,LOW);
      total++;              // Increase vehicle count
    }
  }
  prevEntry = statusentry;  // Update previous state
  if (statusentry == 0) {
    delay(500);
    myservo1.write(0);  // Close entry gate
  }

  /// Update LCD with live data
  delay(340);
  lcd.setCursor(14, 0);
  lcd.print(count);  // Show vacant slots
  lcd.setCursor(14, 1);
  lcd.print(total);  // Show total vehicles
  delay(340);
}