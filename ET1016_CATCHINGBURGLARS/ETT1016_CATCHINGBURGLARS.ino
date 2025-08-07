// BURGLAR ALARM 
// CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09
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

#define NOTE_M4  698

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
      buz.playTone(698,10000);
    }
  } else {
    showDisarmed();
  }

  delay(50);
}
