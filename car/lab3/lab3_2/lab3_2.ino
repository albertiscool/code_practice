#include <SoftwareSerial.h>

// 定義 SoftwareSerial 物件，接收腳(RX)為 2，傳送腳(TX)為 3
SoftwareSerial BT(2, 3); 

char val; // 儲存接收資料的變數

void setup() {
  // 與電腦序列埠監控視窗通訊的波特率
  Serial.begin(9600);
  
  // 設定腳位 3 為輸出模式
  pinMode(3, OUTPUT); 
  
  // 與藍牙模組通訊的波特率 (HC-05 AT 模式預設通常是 38400)
  BT.begin(115200);
}

void loop() {
  // 若收到「序列埠監控視窗」的資料，則送到藍牙模組
  if (Serial.available()) {
    val = Serial.read();
    Serial.print(val);
    BT.print(val);
  }

  // 若收到藍牙模組的資料，則送到序列埠監控視窗
  if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  }
}