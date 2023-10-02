int sure = 60; 
int ledKirmizi = 2;
int ledMavi = 3;
void setup() {
   pinMode(ledKirmizi,OUTPUT); 
   pinMode(ledMavi,OUTPUT); 
}
void loop() {
   for(int i=0;i<3;i++){
      digitalWrite(ledKirmizi,HIGH); 
      delay(sure);
      digitalWrite(ledKirmizi,LOW);
      delay(sure);
   }
   for(int i=0;i<3;i++){
      digitalWrite(ledMavi,HIGH); 
      delay(sure);
      digitalWrite(ledMavi,LOW);
      delay(sure);
   }
}