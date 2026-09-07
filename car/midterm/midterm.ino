#include <SoftwareSerial.h>

// 接收藍牙指令的變數
char command;

// 模式切換變數 (false = 藍牙手動, true = 紅外線自動循跡)
bool isAutoMode = false; 

// 左馬達控制設定 (對應 L298N)
const byte LEFT1 = 8;    // IN1
const byte LEFT2 = 9;    // IN2
const byte LEFT_PWM = 10;

// 右馬達控制設定 (對應 L298N)
const byte RIGHT1 = 7;   // IN3
const byte RIGHT2 = 6;   // IN4
const byte RIGHT_PWM = 5;

// 循跡紅外線感測器腳位 (左中右)
const int IR_LEFT = 13;   
const int IR_CENTER = 11; 
const int IR_RIGHT = 12;  

// 超音波感測器腳位 (使用類比腳位當數位腳位用)
const int trigPin = A0;
const int echoPin = A1;
const int stopDistance = 25; // 設定安全停止距離為 25 公分

// LED 與 蜂鳴器
const int ledPin = A5; 
const int buzPin = 4;  

// 設定預設速度
int valSpeed = 150;

int melody[] = { 523, 587, 659, 698, 784, 880, 988, 1047 };
int duration = 500;

// 建立 Software Serial (RX=2, TX=3)
SoftwareSerial BT(2, 3); 

void setup() {
  Serial.begin(9600);
  BT.begin(115200); 
  
  pinMode(LEFT1, OUTPUT); pinMode(LEFT2, OUTPUT); pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT1, OUTPUT); pinMode(RIGHT2, OUTPUT); pinMode(RIGHT_PWM, OUTPUT);
  pinMode(ledPin, OUTPUT); pinMode(buzPin, OUTPUT);
  
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // 設定超音波腳位
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // 初始確保燈是關的
  digitalWrite(ledPin, LOW);
  stopMotor();
}

void loop() {
  // 1. 優先處理藍牙指令
  if (BT.available()) {
    command = BT.read();
    Serial.println(command); 
    
    // --- 【全局指令區：任何模式下都有效】 ---
    if (command == 'X') {
      isAutoMode = true;  // 開啟自動循跡
      digitalWrite(ledPin, HIGH); // ★ 進入循跡模式，亮燈
    } 
    else if (command == 'x') {
      isAutoMode = false; // 回到藍牙控制
      stopMotor();        // 切換回藍牙時先讓車子停下安全
      digitalWrite(ledPin, LOW);  // ★ 退出循跡模式，關燈
    }
    // 判斷速度調整 (0~9)
    else if (command >= '0' && command <= '9') {
      switch (command) {
        case '0': valSpeed = 0;   break;
        case '1': valSpeed = 25;  break;
        case '2': valSpeed = 50;  break;
        case '3': valSpeed = 75;  break;
        case '4': valSpeed = 100; break;
        case '5': valSpeed = 115; break;
        case '6': valSpeed = 150; break;
        case '7': valSpeed = 175; break;
        case '8': valSpeed = 200; break;
        case '9': valSpeed = 255; break;
      }
    }

    // --- 【手動指令區：只有在藍牙手動模式才有效】 ---
    if (!isAutoMode) {
      switch (command) {
        case 'F': forward(); break;   // 前進
        case 'B': backward(); break;  // 後退
        case 'L': turnLeft(); break;  // 左轉
        case 'R': turnRight(); break; // 右轉
        case 'S': stopMotor(); break; // 停止
        
        case 'Y': // 蜂鳴器
          digitalWrite(buzPin, HIGH); delay(500); digitalWrite(buzPin, LOW);
          break;
      }
    }
  }

  // 2. 如果是「自動循跡模式」，先測距再循跡
  if (isAutoMode) {
    float distance = getDistance(); // 取得距離
    
    // 如果距離小於安全值且讀數有效
    if (distance > 0 && distance < stopDistance) {
      isAutoMode = false; // 強制切回手動模式
      stopMotor();        // 立刻煞車
      digitalWrite(ledPin, LOW); // ★ 避障觸發切回手動，關燈
      
      // 蜂鳴器發出警告音提醒已切換模式
      digitalWrite(buzPin, HIGH); 
      delay(300); 
      digitalWrite(buzPin, LOW);
    } else {
      lineTracking(); // 距離安全，繼續循跡
    }
  }
}

// --- 取得超音波距離函式 ---
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 加上 timeout 避免沒收到回音時程式卡死 (30000us 約等於 5 公尺)
  long duration = pulseIn(echoPin, HIGH, 30000); 
  
  if (duration == 0) return 999; // 沒量到東西回傳大數值
  return (duration / 2.0) / 29.1;
}

// --- 循跡邏輯函式 ---
void lineTracking() {
  int L = digitalRead(IR_LEFT);
  int C = digitalRead(IR_CENTER);
  int R = digitalRead(IR_RIGHT);

  if (C == 0 && R == 0 && L == 0) {
    forward();    
  } else if (R == 1 && C == 1) {
    turnLeft();
    delay(20);
    forward();
    delay(10);   
  } else if (L == 1 && C == 1) {
    turnRight();
    delay(20);
    forward();
    delay(10);   
  } else if (R == 1){
    turnLeft(); 
  } else if (L == 1){
    turnRight();  
  } else {
    forward();
  }
}

// --- 馬達動作函式 ---

void forward() {
  digitalWrite(LEFT1, LOW); digitalWrite(LEFT2, HIGH); analogWrite(LEFT_PWM, valSpeed);
  digitalWrite(RIGHT1, HIGH); digitalWrite(RIGHT2, LOW); analogWrite(RIGHT_PWM, valSpeed);
}

void backward() {
  digitalWrite(LEFT1, HIGH); digitalWrite(LEFT2, LOW); analogWrite(LEFT_PWM, valSpeed);
  digitalWrite(RIGHT1, LOW); digitalWrite(RIGHT2, HIGH); analogWrite(RIGHT_PWM, valSpeed);
}

void turnLeft() {
  digitalWrite(LEFT1, LOW); digitalWrite(LEFT2, HIGH); analogWrite(LEFT_PWM, valSpeed);
  digitalWrite(RIGHT1, LOW); digitalWrite(RIGHT2, HIGH); analogWrite(RIGHT_PWM, valSpeed);
}

void turnRight() {
  digitalWrite(LEFT1, HIGH); digitalWrite(LEFT2, LOW); analogWrite(LEFT_PWM, valSpeed);
  digitalWrite(RIGHT1, HIGH); digitalWrite(RIGHT2, LOW); analogWrite(RIGHT_PWM, valSpeed);
}

void stopMotor() {
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}