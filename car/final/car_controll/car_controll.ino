// ─────────────────────────────────────────────────────────
// 📌 腳位定義 與 狀態變數（自訂按鈕 Pin 2，內建上拉）
// ─────────────────────────────────────────────────────────
const byte LEFT1 = 8;     const byte LEFT2 = 9;     const byte LEFT_PWM = 10;
const byte RIGHT1 = 7;    const byte RIGHT2 = 6;    const byte RIGHT_PWM = 5;
const int ledPin = A5;    const int buzPin = 4;     const int servoPin = 12; 
const int buttonPin = 2; 

bool isAutoMode = false;      
bool lastButtonState = HIGH;   

// 🎯 新增：連續攻擊次數計數器
int attackCount = 0; 

void writeServoAngle(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2500);
  for (int i = 0; i < 15; i++) { 
    digitalWrite(servoPin, HIGH);   delayMicroseconds(pulseWidth);
    digitalWrite(servoPin, LOW);    delay(20 - (pulseWidth / 1000)); 
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LEFT1, OUTPUT);   pinMode(LEFT2, OUTPUT);   pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT1, OUTPUT);  pinMode(RIGHT2, OUTPUT);  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(ledPin, OUTPUT);  pinMode(buzPin, OUTPUT);  pinMode(servoPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  
  writeServoAngle(10); 
  digitalWrite(buzPin, HIGH); delay(100); digitalWrite(buzPin, LOW); 
  digitalWrite(ledPin, LOW); 
  stopCar();
}

// ─────────────────────────────────────────────────────────
// 📌 主要邏輯迴圈（雙擊補刀修正版）
// ─────────────────────────────────────────────────────────
void loop() {
  // 🎯 按鈕偵測開關
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(50); // 防彈跳
    isAutoMode = !isAutoMode; 
    
    if (isAutoMode) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzPin, HIGH); delay(80); digitalWrite(buzPin, LOW); 
      
      // 🟢 核心修正：手動啟動時強行前進 2000ms，期間不斷清空 Serial 攔截樹莓派指令
      moveForward(); 
      unsigned long startTime = millis();
      while (millis() - startTime < 1500) { 
        if (Serial.available() > 0) { 
          Serial.read(); 
        }
      }
      stopCar();
      delay(100); 
      
    } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzPin, HIGH); delay(80); digitalWrite(buzPin, LOW); delay(80); digitalWrite(buzPin, HIGH); delay(80); digitalWrite(buzPin, LOW); 
      stopCar();                  
      attackCount = 0; // 關閉時重置計數
    }
  }
  lastButtonState = currentButtonState; 

  // 🎯 接收樹莓派序列埠指令
  if (Serial.available() > 0) {
    char cmd;
    while (Serial.available() > 0) { cmd = Serial.read(); } // 讀取最新指令
    
    if (cmd == 'X') { isAutoMode = true; digitalWrite(ledPin, HIGH); return; }
    if (cmd == 'x') { isAutoMode = false; digitalWrite(ledPin, LOW); stopCar(); attackCount = 0; return; } 
    
    // 收到 'S' 指令時只停車，不解除自動模式
    if (cmd == 'S') { stopCar(); return; } 
    
    if (isAutoMode == false) { stopCar(); return; }
    
    // 👣 核心動作控制
    switch(cmd) {
      case 'F': moveForward_Slowly(); delay(250); stopCar(); delay(200); attackCount = 0; break; 
      case 'L': turnLeft(); delay(70); stopCar(); delay(200); attackCount = 0; break;           
      case 'R': turnRight(); delay(85); stopCar(); delay(200); attackCount = 0; break;          
      case 'W': turnRight(); delay(100); stopCar(); delay(200); attackCount = 0; break;          
      
      case 'A': 
        stopCar();
        attackCount++; // 累加攻擊次數
        
        if (attackCount == 1) {
          // ⚔️ 第 1 次攻擊：直接揮刀
          digitalWrite(buzPin, HIGH); 
          writeServoAngle(120); delay(100);                 
          writeServoAngle(10);  digitalWrite(buzPin, LOW);
          
        } else if (attackCount >= 2) {
          // 🔄 第 2 次攻擊（代表剛剛沒砍破）：先微調位置再補刀
          
          // 1. 微幅後退（設定倒退 150ms，可根據現場車體微調）
          moveBackward(); delay(150); 
          stopCar(); delay(200);
          
          // 2. 補第二刀
          digitalWrite(buzPin, HIGH); 
          writeServoAngle(120); delay(100);                 
          writeServoAngle(10);  digitalWrite(buzPin, LOW);
          
          attackCount = 0; // 補刀完成，重置計數
        }
        break;
    }
  }
}

// ─────────────────────────────────────────────────────────
// 🛠️ 馬達運動控制（馬力分配：左 195 / 右 200）
// ─────────────────────────────────────────────────────────

void moveForward() {
  digitalWrite(LEFT1, LOW);   digitalWrite(LEFT2, HIGH);  analogWrite(LEFT_PWM, 200); 
  digitalWrite(RIGHT1, HIGH); digitalWrite(RIGHT2, LOW);   analogWrite(RIGHT_PWM, 240); 
}
void moveForward_Slowly() {
  digitalWrite(LEFT1, LOW);   digitalWrite(LEFT2, HIGH);  analogWrite(LEFT_PWM, 200); 
  digitalWrite(RIGHT1, HIGH); digitalWrite(RIGHT2, LOW);   analogWrite(RIGHT_PWM, 255); 
}
void moveBackward() {
  digitalWrite(LEFT1, HIGH);  digitalWrite(LEFT2, LOW);   analogWrite(LEFT_PWM, 200); 
  digitalWrite(RIGHT1, LOW);   digitalWrite(RIGHT2, HIGH); analogWrite(RIGHT_PWM, 200); 
}
void turnLeft() {
  digitalWrite(LEFT1, HIGH);  digitalWrite(LEFT2, LOW);   analogWrite(LEFT_PWM, 200); 
  digitalWrite(RIGHT1, HIGH); digitalWrite(RIGHT2, LOW);   analogWrite(RIGHT_PWM, 200); 
}
void turnRight() {
  digitalWrite(LEFT1, LOW);   digitalWrite(LEFT2, HIGH);  analogWrite(LEFT_PWM, 200); 
  digitalWrite(RIGHT1, LOW);   digitalWrite(RIGHT2, HIGH); analogWrite(RIGHT_PWM, 200); 
}
void stopCar() {
  analogWrite(LEFT_PWM, 0);   analogWrite(RIGHT_PWM, 0);
}