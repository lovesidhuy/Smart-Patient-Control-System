
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <IRremote.h>
// Pins
const int tempPin = A0; // Tempurature Sensor (Analog Input)
const int IRPIN = 11; //ir
const int potPin = A1; // Potentiometer Pin (Analog Input)
const int pirPin = 2; // Motion Detector Pin (Digital Input)
const int redPin = 5; // Red LED (Digital Output)
const int bluePin = 6; // Blue LED (Digital Output)
const int greenPin = 3; // Green LED (Digital Output)
const int buzzPin = 7; // Buzzer (Digital Ouput)
const int servoPin = 10; // Servo
const int ackPin = 8; //Ack/Reset Button
const int modePin = 9; //Mode Button
// IR remote button codes
const int BUTTON0_Ack = 0x0C;
const int BUTTON1_Mode = 0x10;
const int BUTTON2 = 0x11;
const int BUTTON3 = 0x12;
const int BUTTON4 = 0x14;
const int BUTTON5 = 0x15;
const int BUTTON6 = 0x16;
const int BUTTON7 = 0x18;
const int BUTTON8 = 0x19;
const int BUTTON9 = 0x1A;
Servo serv;
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool isSilenced = false;
bool isUnlocked = false;
bool nurseAct = false;
bool lastMode = LOW;
bool lastAck = LOW;
void setup() {
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(ackPin, INPUT);
  pinMode(modePin, INPUT);
  serv.attach(servoPin);
  serv.write(0);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(1000);
  lcd.clear();
  // Start IR receiver
  //IrReceiver.begin(IRPIN, ENABLE_LED_FEEDBACK);
}
void loop() {
  // ===== SENSOR DATA =====
  float voltage = analogRead(tempPin) * (5.0 / 1024.0);
  float tempinC = (voltage - 0.5) * 100;
  int bpm = map(analogRead(potPin), 0, 1023, 0, 300);
  bool motion = digitalRead(pirPin);
  // ===== CONDITIONS =====
  bool tempCrit = (tempinC > 38.5 || tempinC < 35.0);
  bool bpmCrit = (bpm > 130 || bpm < 50);
  bool tempCaut = (tempinC >= 37.6 && tempinC <= 38.5);
  bool bpmCaut = (bpm >= 111 && bpm <= 130);
  bool tempnorm = (tempinC >= 35.0 && tempinC <= 38.5);
  bool bpmnorm = (bpm <= 110 && bpm >= 50);
  bool allNormal = tempnorm && bpmnorm;
  // ===== BUTTONS =====
  bool modeState = digitalRead(modePin);
  bool ackState = digitalRead(ackPin);
  if (lastMode == LOW && modeState == HIGH && allNormal) {
    nurseAct = true;
  }
  if (tempCrit || bpmCrit) {}
  if (lastAck == LOW && ackState == HIGH) {
    isSilenced = true;
    isUnlocked = true;
    nurseAct = false;
  }
  lastMode = modeState;
  lastAck = ackState;
  // ===== IR REMOTE =====
  //if (IrReceiver.decode()) {
  // int cm = IrReceiver.decodedIRData.command;
  // Serial.print("IR Command: 0x");
  // Serial.println(cm, HEX);
  // switch (cm) {
  // case BUTTON1_Mode:
  // if (allNormal) {
  // nurseAct = true;
  // }
  // break;
  // case BUTTON0_Ack:
  // isSilenced = true;
  // isUnlocked = true;
  // nurseAct = false;
  // break;
  // }
  // IrReceiver.resume();
  // }
  // ===== LCD LINE 1 =====
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempinC, 1);
  lcd.print(" B:");
  lcd.print(bpm);
  lcd.print(" ");
  Serial.print("Temp: ");
  Serial.print(tempinC);
  Serial.print(" | BPM: ");
  Serial.println(bpm);
  // ===== LCD LINE 2 =====
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  // --- DOUBLE CRITICAL ---
  if (tempCrit && bpmCrit) {
    lcd.print("CRIT TB");
    setColor(255, 0, 0);
    buzz(1000);
    if ((millis() / 500) % 2 == 0) {
      setColor(0, 0, 255);
    }
    lock(true);
  }
  // --- CRITICAL ---
  else if (tempCrit) {
    lcd.print("CRIT T:");
    lcd.print(tempinC, 1);
    setColor(255, 0, 0);
    buzz(1000);
    lock(false);
  } else if (bpmCrit) {
    lcd.print("CRIT B:");
    lcd.print(bpm);
    setColor(255, 0, 0);
    buzz(1000);
    lock(false);
  }
  // --- CAUTION ---
  else if (tempCaut) {
    lcd.print("CAUT T:");
    lcd.print(tempinC, 1);
    setColor(255, 255, 0);
    buzz(0);
    lock(false);
  } else if (bpmCaut) {
    lcd.print("CAUT B:");
    lcd.print(bpm);
    setColor(255, 255, 0);
    buzz(0);
    lock(false);
  }
  // --- NURSE CALL ---
  else if (nurseAct) {
    lcd.print("NURSE CALL");
    setColor(0, 0, 255);
    buzz(800);
    lock(false);
  }
  // --- MOTION ---
  else if (motion && !isUnlocked) {
    lcd.print("MOTION LOCK");
    setColor(255, 0, 0);
    buzz(1000);
    lock(true);
  }
  // --- NORMAL ---
  else {
    isSilenced = false;
    isUnlocked = false;
    lcd.print("NORMAL");
    setColor(0, 255, 0);
    buzz(0);
  }
  delay(200);
}
// ===== BUZZER =====
void buzz(int hz) {
  if (isSilenced == true || hz == 0) {
    noTone(buzzPin);
  } else {
    tone(buzzPin, hz);
  }
}
// ===== RGB LED =====
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
// ===== SERVO LOCK =====
void lock(bool val) {
  if (val) serv.write(90);
  else serv.write(0);
}
