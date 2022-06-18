#define MicroSW PB13
void setup() {
  Serial.begin(115200);
  pinMode(MicroSW, INPUT);

  attachInterrupt(digitalPinToInterrupt(MicroSW), stopAlarmClock, FALLING);

}

void loop() {
  Serial.println("..");
  delay(500);
}

void stopAlarmClock() {
  Serial.println("DESPERTADOR PARADO");
  delay(500);
}
