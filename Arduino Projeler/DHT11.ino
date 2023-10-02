#include <dht11.h>  
int DhtPin = 2;     
dht11 dht_sensor;   
void setup() {
  Serial.begin(9600);  
}
void loop() {
  int chk = dht_sensor.read(DhtPin);
  Serial.print("Nem Orani (%): ");
  Serial.println((float)dht_sensor.humidity, 2);
  Serial.print("Sicaklik (Celcius): ");
  Serial.println((float)dht_sensor.temperature, 2);
  Serial.print("Sicaklik (Kelvin): ");
  Serial.println(dht_sensor.kelvin(), 2);
  Serial.print("Sicaklik (Fahrenheit): ");
  Serial.println(dht_sensor.fahrenheit(), 2);
  Serial.print("Cig Olusma Noktasi: ");
  Serial.println(dht_sensor.dewPoint(), 2);
  Serial.println("------------------");
  delay(1000);
}