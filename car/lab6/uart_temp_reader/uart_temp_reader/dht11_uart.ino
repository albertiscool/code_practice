#include <DHT.h>

// 定義 DHT11 資料腳位接在 Arduino 的 Digital Pin 2
#define DHTPIN 2     

// 定義感測器型號為 DHT11
#define DHTTYPE DHT11   

// 初始化 DHT 物件
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // 設定 UART 鮑率為 115200 (RPi 端接收需設定相同數值)
  Serial.begin(115200);
  
  // 啟動 DHT 感測器
  dht.begin();
  
  // 等待硬體穩定
  delay(1000);
}

void loop() {
  // DHT11 取樣間隔至少需 2 秒
  delay(2000);

  // 讀取濕度與攝氏溫度
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // 檢查讀取是否成功 (若 NaN 代表讀取失敗)
  if (isnan(hum) || isnan(temp)) {
    Serial.println("E:Sensor Read Error");
    return;
  }

  // 透過 UART 輸出格式化字串，範例輸出格式： T:25.0,H:60.0
  Serial.print("T:");
  Serial.print(temp, 1); // 保留小數點後 1 位
  Serial.print(",H:");
  Serial.print(hum, 1);
  Serial.println();      // 自帶換行符號 (\r\n)，方便 RPi 端 readline() 判斷
}