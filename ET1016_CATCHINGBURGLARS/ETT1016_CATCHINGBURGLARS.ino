// BURGLAR ALARM 
// CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09

#include <math.h>
#include <Wire.h>
#include "RichShieldLightSensor.h"
#include "RichShieldPassiveBuzzer.h"
#include "RichShieldNTC.h"
#include "RichShieldTM1637.h"

// ----- PIN SETUP -----
#define BUTTON_PIN 8
#define BUZZER_PIN 3
#define RED_LED 4
#define GREEN_LED 5
#define BLUE_LED 6
#define LDR_PIN A2
#define TEMP_PIN A1
#define CLK 10
#define DIO 11

#define ALARM_NOTE 698  // M4 note

// ----- SETTINGS -----
const float lightThreshold = 20.0;
const int tempThreshold = 28;
const int debounceTime = 300;

// ----- LIBRARY OBJECTS -----
PassiveBuzzer buzzer(BUZZER_PIN);
LightSensor ldr(LDR_PIN);
NTC tempSensor(TEMP_PIN);
TM1637 display(CLK, DIO);

// ----- STATE VARIABLES -----
int isArmed = 0;
unsigned long lastBtnPress = 0;
const int leds[] = {RED_LED, GREEN_LED, BLUE_LED};
const int ledCount = sizeof(leds) / sizeof(leds[0]);

// ----- LED FUNCTIONS -----
void turnOffLEDs() {
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void showLED(int ledPin) {
  turnOffLEDs();
  digitalWrite(ledPin, HIGH);
}

// ----- DISPLAY TEMPERATURE -----
void displayTemp(int8_t tempVal) {
  int8_t digits[4];

  if (tempVal < 0) {
    digits[0] = INDEX_NEGATIVE_SIGN;
    tempVal = abs(tempVal);
  } else {
    digits[0] = (tempVal < 100) ? INDEX_BLANK : tempVal / 100;
  }

  tempVal %= 100;
  digits[1] = tempVal / 10;
  digits[2] = tempVal % 10;
  digits[3] = 12; // 'C'

  display.display(digits);
}

// ----- CHECK LDR -----
int isLightDetected() {
  float r = ldr.getRes();
  float lux = 325 * pow(r, -1.4);
  Serial.print("LDR Lux: ");
  Serial.println(lux);

  return lux < lightThreshold;
}

// ----- SETUP -----
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
  }

  Serial.begin(9600);
  display.init();
  delay(1000);
  showLED(GREEN_LED); // Start in disarmed state
}

// ----- LOOP -----
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW && millis() - lastBtnPress > debounceTime) {
    isArmed = !isArmed;
    lastBtnPress = millis();
  }

  int tempVal = (int)tempSensor.getTemperature();
  displayTemp(tempVal);

  if (isArmed) {
    showLED(BLUE_LED); // Armed mode

    if (isLightDetected() || tempVal >= tempThreshold) {
      showLED(RED_LED); // Alarm mode
      buzzer.playTone(ALARM_NOTE, 1000);
    }

  } else {
    showLED(GREEN_LED); // Disarmed mode
  }

  delay(100);
}
