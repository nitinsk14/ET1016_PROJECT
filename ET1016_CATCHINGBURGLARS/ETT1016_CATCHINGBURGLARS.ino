//CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09
// BURGLAR ALARM 
#define KNOB_PIN 0
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  delay(500);
  digitalWrite(LED_RED, LOW);
  delay(500);
}
