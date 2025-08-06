//CATCHINGBURGLARS
// NITIN & EVAN 
// P2517704 & P2517478
// DCEP/FT/1A/09
// BURGLAR ALARM 
#define BTN 8
#define BUZZER 3
#define RED 4
#define GREEN 5
#define BLUE 6

#define LDR A0
#define IR 2

int armed = 0;
int ldrThreshold = 300;

void setup() {
  pinMode(BTN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(IR, INPUT);
  Serial.begin(9600);
  showDisarmed();
}

void loop() {
  if (digitalRead(BTN)) {
    armed = !armed;
    delay(300); // debounce
  }

  if (armed) {
    showArmed();
    int light = analogRead(LDR);
    int motion = digitalRead(IR);
    Serial.print("LDR: "); Serial.print(light);
    Serial.print(" | IR: "); Serial.println(motion);

    if (light < ldrThreshold || motion) siren();
    else noTone(BUZZER);
  } else {
    showDisarmed();
    noTone(BUZZER);
  }
}

void siren() {
  digitalWrite(RED, HIGH);
  for (int f = 500; f < 1000; f += 10) {
    tone(BUZZER, f); delay(2);
    if (!armed) break;
  }
  for (int f = 1000; f > 500; f -= 10) {
    tone(BUZZER, f); delay(2);
    if (!armed) break;
  }
}

void showDisarmed() {
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, LOW);
}

void showArmed() {
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, LOW);
}
