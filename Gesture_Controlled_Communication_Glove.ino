#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// === Pin Definitions ===
const int indexPin  = A0;   // Index finger flex sensor
const int middlePin = A1;   // Middle finger flex sensor
const int ringPin   = A2;   // Ring finger flex sensor

// === Thresholds (based on my test) ===
const int indexThreshold  = 9;   // Help
const int middleThreshold = 10;  // Restroom
const int ringThreshold   = 8;   // Hungry

// === LCD and DFPlayer Setup ===
LiquidCrystal_I2C lcd(0x27, 16, 2); I2C Address
SoftwareSerial mySoftwareSerial(9, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// === Flex sensor readings ===
int indexFlex = 0;
int middleFlex = 0;
int ringFlex = 0;

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  lcd.init();
  lcd.backlight();

  // Display project title
  lcd.setCursor(0, 0);
  lcd.print("Gesture Control");
  lcd.setCursor(0, 1);
  lcd.print("Glove");
  delay(3000);
  lcd.clear();

  // Initialize DFPlayer 
  if (myDFPlayer.begin(mySoftwareSerial)) {
    myDFPlayer.volume(30); // volume 0–30
  }

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  // Read flex sensor values
  indexFlex  = analogRead(indexPin);
  middleFlex = analogRead(middlePin);
  ringFlex   = analogRead(ringPin);

  Serial.print("Index: ");  Serial.print(indexFlex);
  Serial.print("  Middle: "); Serial.print(middleFlex);
  Serial.print("  Ring: "); Serial.println(ringFlex);

  lcd.clear();

  // === Gesture 1: Index bent ===
  if (indexFlex > indexThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("Gesture:");
    lcd.setCursor(0, 1);
    lcd.print("I NEED HELP");
    myDFPlayer.play(1);  // 001.mp3
    delay(4000);
  }

  // === Gesture 2: Middle bent ===
  else if (middleFlex > middleThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("Gesture:");
    lcd.setCursor(0, 1);
    lcd.print("RESTROOM");
    myDFPlayer.play(2);  // 002.mp3
    delay(4000);
  }

  // === Gesture 3: Ring bent ===
  else if (ringFlex > ringThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("Gesture:");
    lcd.setCursor(0, 1);
    lcd.print("I'M HUNGRY");
    myDFPlayer.play(4);  // 004.mp3
    delay(4000);
  }

  // === No Gesture ===
  else {
    lcd.setCursor(0, 0);
    lcd.print("No Gesture");
  }

  delay(200);
}
