//CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09
// BURGLAR ALARM 

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
