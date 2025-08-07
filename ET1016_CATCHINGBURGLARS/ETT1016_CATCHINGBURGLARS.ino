// CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09
// BURGLAR ALARM
#include <math.h>
#include <Wire.h>

#include "RichShieldLightSensor.h"
#include "RichShieldPassiveBuzzer.h"

// Pin definitions
#define BTN 8
#define BUZZER 3
#define RED 4
#define GREEN 5
#define BLUE 6
#define LIGHTSENSOR_PIN A2

// Setup buzzer and light sensor objects
PassiveBuzzer buz(BUZZER);
LightSensor lightsensor(LIGHTSENSOR_PIN);

// Notes for melody
#define NOTE_L1  262
#define NOTE_L2  294
#define NOTE_L3  330
#define NOTE_L4  349
#define NOTE_L5  392
#define NOTE_L6  440
#define NOTE_L7  494
#define NOTE_M1  523
#define NOTE_M2  587
#define NOTE_M3  659
#define NOTE_M4  698
#define NOTE_M5  784
#define NOTE_M6  880
#define NOTE_M7  988
#define NOTE_H1  1046
#define NOTE_H2  1175
#define NOTE_H3  1318
#define NOTE_H4  1397
#define NOTE_H5  1568
#define NOTE_H6  1760
#define NOTE_H7  1976

const static int doubleTiger[] = { 
  NOTE_M1, NOTE_M2, NOTE_M3, NOTE_M1, 
  NOTE_M1, NOTE_M2, NOTE_M3, NOTE_M1, 
  NOTE_M3, NOTE_M4, NOTE_M5, 
  NOTE_M3, NOTE_M4, NOTE_M5, 
  NOTE_M5, NOTE_M6, NOTE_M5, NOTE_M4, NOTE_M3, NOTE_M1, 
  NOTE_M5, NOTE_M6, NOTE_M5, NOTE_M4, NOTE_M3, NOTE_M1, 
  NOTE_M2, NOTE_L5, NOTE_M1, 0, 
  NOTE_M2, NOTE_L5, NOTE_M1, 0
};

const static int tempo[] = { 
  2, 2, 2, 2, 
  2, 2, 2, 2, 
  2, 2, 1, 
  2, 2, 1,  
  2, 2, 2, 2, 1, 1,
  2, 2, 2, 2, 1, 1,
  1, 1, 1, 1, 1, 1
};

int armed = 0;
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 300;

const int ledPins[] = {RED, GREEN, BLUE};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// Threshold for lux below which alarm triggers (adjust as needed)
const float luxThreshold = 20.0;

// Turn off all LEDs
void turnOffAllLEDs() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

// Show disarmed (green)
void showDisarmed() {
  turnOffAllLEDs();
  digitalWrite(GREEN, HIGH);
}

// Show armed (blue)
void showArmed() {
  turnOffAllLEDs();
  digitalWrite(BLUE, HIGH);
}

// Show alarm (red)
void showAlarm() {
  turnOffAllLEDs();
  digitalWrite(RED, HIGH);
}

// Check if intrusion detected by light sensor reading
int isIntrusionDetected() {
  float Rsensor = lightsensor.getRes(); // resistance in KOhms
  float lux = 325 * pow(Rsensor, -1.4);
  
  Serial.print("Resistance (KOhm): ");
  Serial.print(Rsensor, 1);
  Serial.print(" | Lux: ");
  Serial.println(lux, 1);

  // Return 1 if lux below threshold (dark = possible intrusion)
  if (lux < luxThreshold) return 1;
  else return 0;
}

// Play alarm melody
void sing() {
  int size = sizeof(doubleTiger) / sizeof(int);
  for (int i = 0; i < size; i++) {
    int noteDuration = 500 / tempo[i];
    buz.playTone(doubleTiger[i], noteDuration);
    delay(noteDuration * 0.3); // spacing between notes
  }
}

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
  showDisarmed();
}

void loop() {
  // Toggle armed/disarmed with button press
  if (digitalRead(BTN) == LOW && millis() - lastButtonTime > debounceDelay) {
    armed = !armed;
    lastButtonTime = millis();
  }

  if (armed) {
    showArmed();
    if (isIntrusionDetected()) {
      showAlarm();
      sing();
    }
  } else {
    showDisarmed();
  }

  delay(50);
}
