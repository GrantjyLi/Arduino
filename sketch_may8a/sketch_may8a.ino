void setup() {
  // put your setup code here, to run once:
  pinMode(D4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D4, HIGH);
  delay(200);
  digitalWrite(D4, LOW);
  delay(200);
}
