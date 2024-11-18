#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

int BottleCount = 0;
bool idle = true;

LiquidCrystal_I2C lcd(0x27, 20, 4);
int gate_pin = 27;
Servo gate;

const int trigPin = 32;
const int echoPin = 33;
long duration;
float distanceCm;

// button
const int btn_start = 18, btn_end = 5;
int btn_start_state = 1, btn_end_state = 1;

// function initialization
void checkInsertedObject();
void printBottleCount();
bool check_if_done();

void setup() {
  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // servo setup
  gate.attach(gate_pin);
  gate.write(0);

  // ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // buttons setup
  pinMode(btn_start, INPUT_PULLUP);
  pinMode(btn_end, INPUT_PULLUP);
  digitalWrite(btn_start, HIGH);
  digitalWrite(btn_end, HIGH);
}

void loop() {
  // lcd
  lcd.setCursor(0, 0);
  lcd.print("  Welcome to WEWO   ");
  lcd.setCursor(0, 3);
  lcd.print("Start        Recieve");

  // buttons
  btn_start_state = digitalRead(btn_start);
  btn_end_state = digitalRead(btn_end);

  if (btn_start_state == LOW) {
    idle = false;
    checkInsertedObject();
  }

  delay(100);

  if (!idle) {
    checkInsertedObject();
  }

}

void checkInsertedObject() {
  bool objectDetected = false;
  gate.write(180);
  printBottleCount();
  objectDetected = check_if_done();
  while (!objectDetected) {

    objectDetected = check_if_done();
    if (objectDetected) { return; }

    // ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED/2;

    delay(1000);

    if (distanceCm < 10.0 || distanceCm > 60.0) {
      BottleCount += 1;

      // this is where the object detection will occur
      gate.write(0);
      lcd.setCursor(0,2);
      lcd.print("Processing object...");
      delay(5000);
      
      printBottleCount();
      gate.write(180);
      lcd.setCursor(0,2);
      lcd.print("Recieve when done.");
      objectDetected = true;
    }
  }
}

void printBottleCount() {
    lcd.setCursor(0, 1);
    lcd.print("Bottle count: ");
    lcd.setCursor(14, 1);
    lcd.print(BottleCount);
}

bool check_if_done() {
  btn_end_state = digitalRead(btn_end);
  if (btn_end_state == LOW) {
    gate.write(0);
    printBottleCount();
    idle = true;
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("Thank you");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");

    BottleCount = 0;
    return true;
  }
  return false;
}

