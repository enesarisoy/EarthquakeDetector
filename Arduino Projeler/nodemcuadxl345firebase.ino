#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "ADXL345.h"

ADXL345 sensor;

#define FIREBASE_HOST "deprem-1dede-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "emzZxjCibsYPIQXA8unftGkjvs4cku7KHCq48YtK"
#define WIFI_SSID "TTNET_TP-LINK_06F1"
#define WIFI_PASSWORD "sK0yfWqv"

// İvmeölçer verilerini saklamak için değişkenler
int numReadings = 10;
int readings_x[10];
int readings_y[10];
int readings_z[10];
float previous_average_x = 0.0;
float previous_average_y = 0.0;
float previous_average_z = 0.0;
short ivme_x, ivme_y, ivme_z;

bool ilkGiris = true;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  sensor.initialize();
  Serial.print("Test: ");
  Serial.println(sensor.testConnection() ? "BAŞARILI" : "BAŞARISIZ!");

  // WiFi bağlantısı
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Bağlanılıyor");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Bağlandı: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  static int index = 0;  // Sadece loop işlevi içinde kullanılan bir static tamsayı olarak tanımlandı.
  sensor.getAcceleration(&ivme_x, &ivme_y, &ivme_z);
  Serial.print("X: ");
  Serial.print(ivme_x);
  Serial.print("\t");
  Serial.print("Y: ");
  Serial.print(ivme_y);
  Serial.print("\t");
  Serial.print("Z: ");
  Serial.println(ivme_z);

    // Okuma değerlerini güncelle
    readings_x[index] = ivme_x;
    readings_y[index] = ivme_y;
    readings_z[index] = ivme_z;

    // İndeks arttır ve sıfıra döndür
    index = (index + 1) % numReadings;

    // Her 10 ölçüm için ortalamayı hesapla ve kontrol et
    if (index == 0) {
      float average_x = 0.0;
      float average_y = 0.0;
      float average_z = 0.0;

      for (int i = 0; i < numReadings; i++) {
        average_x += readings_x[i];
        average_y += readings_y[i];
        average_z += readings_z[i];
      }

      average_x /= numReadings;
      average_y /= numReadings;
      average_z /= numReadings;

      Serial.print("Ortalama X: ");
      Serial.println(average_x);
      Serial.print("Ortalama Y: ");
      Serial.println(average_y);
      Serial.print("Ortalama Z: ");
      Serial.println(average_z);

      if(!ilkGiris){
        if (
          (abs(average_x) > abs(previous_average_x * 1.1)) || (abs(average_x) < abs(previous_average_x * 0.9)) ||
          (abs(average_y) > abs(previous_average_y * 1.1)) || (abs(average_y) < abs(previous_average_y * 0.9)) ||
            (abs(average_z) > abs(previous_average_z * 1.1)) || (abs(average_z) < abs(previous_average_z * 0.9))) {
          Firebase.setBool("deprem", true);
        } else {
          Firebase.set("deprem", false);
        }
      }else{
        Serial.println("Bu ilk giriş");
        ilkGiris = false;
      }

      

      previous_average_x = average_x;
      previous_average_y = average_y;
      previous_average_z = average_z;
    }
  

  delay(100);
}
