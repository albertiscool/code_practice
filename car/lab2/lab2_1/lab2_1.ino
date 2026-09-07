const int trig = 5;
const int echo = 6;
const int inter_time = 1000;
int time = 0;
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  float duration, distance;
  
  // 觸發超音波模組
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000); // 這裡維持你圖片中的 1000us
  digitalWrite(trig, LOW);
  
  // 讀取脈衝時間
  duration = pulseIn(echo, HIGH);
  
  // 計算距離 (公分)
  distance = (duration / 2) / 29;
  
  // 序列埠輸出
  Serial.print("Data:");
  Serial.print(time / 1000);
  Serial.print(", d = ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if(distance < 10)
  {
    digitalWrite(ledPin,HIGH);
    delay(500);
    digitalWrite(ledPin,LOW);
    delay(500);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  // 更新時間並延遲
  time = time + inter_time;
  delay(inter_time);
}