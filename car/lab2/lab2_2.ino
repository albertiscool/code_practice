#include "DHT.h"

#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

int ledPin = 13;

void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
  Serial.println("DHT11 test!");
  dht.begin();
}

void loop() {
  // 等待兩秒再測量一次（DHT11 讀取頻率較慢）
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // 檢查是否有讀取失敗的情況
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("*C\t");
  Serial.print(f);
  Serial.println("*F");

  if(h > 70 || t > 28)
  {
    digitalWrite(ledPin,HIGH);
    delay(500);
    digitalWrite(ledPin,LOW);
    delay(500);
  }
  else
  {
    digitalWrite(ledPin,LOW);
    delay(500);
  }
}