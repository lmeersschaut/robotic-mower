#include <LiquidCrystal_I2C.h>

// ================= PIN CONFIG =================

// Button
const int buttonPin = 2;

// Drive motors (H-bridge, simple HIGH/LOW)
const int leftMotor1 = 3;
const int leftMotor2 = 4;
const int rightMotor1 = 5;
const int rightMotor2 = 6;

// Blade motor (PWM for softstart)
const int bladePWM = 9;
const int bladeDir1 = 7;
const int bladeDir2 = 8;

// Ultrasonic sensors
const int trigL = 10, echoL = 11;
const int trigM = 12, echoM = 13;
const int trigR = A1, echoR = A2;

// Battery
const int voltPin = A0;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= BATTERY =================
const float R1 = 47000.0;
const float R2 = 10000.0;
const float Vmin = 21.0;
const float Vmax = 25.2;

// ================= STATE =================
bool robotRunning = false;
bool lastButtonState = HIGH;

// ================= SETUP =================
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  pinMode(bladePWM, OUTPUT);
  pinMode(bladeDir1, OUTPUT);
  pinMode(bladeDir2, OUTPUT);

  pinMode(trigL, OUTPUT); pinMode(echoL, INPUT);
  pinMode(trigM, OUTPUT); pinMode(echoM, INPUT);
  pinMode(trigR, OUTPUT); pinMode(echoR, INPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  stopAll();
}

// ================= LOOP =================
void loop() {
  handleButton();

  if (robotRunning) {
    runRobot();
  } else {
    stopAll();
    displayStatus("STOPPED");
  }

  delay(100);
}

// ================= BUTTON =================
void handleButton() {
  bool currentState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentState == LOW) {
    robotRunning = !robotRunning;
    delay(300); // debounce
  }

  lastButtonState = currentState;
}

// ================= MAIN LOGIC =================
void runRobot() {
  float distL = readDistance(trigL, echoL);
  float distM = readDistance(trigM, echoM);
  float distR = readDistance(trigR, echoR);

  // Start blade with softstart once
  static bool bladeStarted = false;
  if (!bladeStarted) {
    softStartBlade();
    bladeStarted = true;
  }

  // Obstacle logic
  if (distM < 30) {
    stopDrive();
    delay(200);

    if (distL > distR) {
      turnLeft();
    } else {
      turnRight();
    }
    delay(random(400, 800));
  }
  else if (distL < 20) {
    turnRight();
    delay(300);
  }
  else if (distR < 20) {
    turnLeft();
    delay(300);
  }
  else {
    forward();
  }

  displayStatus("RUNNING");
}

// ================= MOTOR CONTROL =================
void forward() {
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
}

void turnLeft() {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
}

void turnRight() {
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
}

void stopDrive() {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
}

void stopAll() {
  stopDrive();
  analogWrite(bladePWM, 0);
}

// ================= BLADE SOFTSTART =================
void softStartBlade() {
  digitalWrite(bladeDir1, HIGH);
  digitalWrite(bladeDir2, LOW);

  for (int pwm = 0; pwm <= 255; pwm++) {
    analogWrite(bladePWM, pwm);
    delay(20); // smooth ramp
  }
}

// ================= ULTRASONIC =================
float readDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(5);

  digitalWrite(trig, HIGH);
  delayMicroseconds(15);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);

  if (duration == 0) return 999;

  return duration * 0.0343 / 2.0;
}

// ================= BATTERY =================
float getBatteryPercent() {
  int raw = analogRead(voltPin);
  float voltage = raw * (5.0 / 1023.0);
  voltage = voltage * ((R1 + R2) / R2);

  float percent = ((voltage - Vmin) / (Vmax - Vmin)) * 100.0;

  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;

  return percent;
}

// ================= LCD =================
void displayStatus(String state) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(state);

  lcd.setCursor(0, 1);
  lcd.print("Bat: ");
  lcd.print((int)getBatteryPercent());
  lcd.print("%");
}
