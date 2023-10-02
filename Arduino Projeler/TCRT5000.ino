void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int sensorValue = analogRead(A0);  
  Serial.print(sensorValue);         
  if (sensorValue < 900) {
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
  delay(10);
}