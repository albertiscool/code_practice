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

// 🛡️ =========================================================
// 🛡️ 【工業級 Failsafe 通訊看門狗 (Communication Watchdog)】
// 🛡️ =========================================================
unsigned long lastHeartbeatTime = 0;           // 上一次收到合法指令或心跳的時間戳
const unsigned long FAILSAFE_TIMEOUT_MS = 300; // 超時門檻：300ms (容許掉 1~2 封包，超時立即判定斷線)
bool isFailsafeActive = false;                 // 是否觸發緊急煞車鎖定

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
  brakeCar_Dynamic(); // 上電預設電磁制動
  
  lastHeartbeatTime = millis(); // 初始化看門狗計時器
}

// ─────────────────────────────────────────────────────────
// 📌 主要邏輯迴圈（整合 Failsafe 看門狗與動態煞車）
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
      lastHeartbeatTime = millis(); // 啟動自動模式時重置看門狗
      isFailsafeActive = false;
      
      // 🟢 核心修正：手動啟動時強行前進 1500ms，期間不斷清空 Serial 攔截樹莓派指令
      moveForward(); 
      unsigned long startTime = millis();
      while (millis() - startTime < 1500) { 
        if (Serial.available() > 0) { 
          Serial.read(); 
        }
      }
      stopCar();
      delay(100); 
      lastHeartbeatTime = millis();
      
    } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzPin, HIGH); delay(80); digitalWrite(buzPin, LOW); delay(80); digitalWrite(buzPin, HIGH); delay(80); digitalWrite(buzPin, LOW); 
      brakeCar_Dynamic(); // 關閉自動模式時強制電磁煞停
      attackCount = 0;
      isFailsafeActive = false;
    }
  }
  lastButtonState = currentButtonState; 

  // 🛡️ -------------------------------------------------------------
  // 🛡️ [Failsafe 通訊看門狗監控]
  // 🛡️ 在自動模式下，若超過 300ms 未收到任何來自樹莓派的指令或心跳，
  // 🛡️ 判定上層當機或 UART 傳輸線甩脫，立即切換為動態電磁煞車防暴衝！
  // 🛡️ -------------------------------------------------------------
  if (isAutoMode) {
    if (millis() - lastHeartbeatTime > FAILSAFE_TIMEOUT_MS) {
      if (!isFailsafeActive) {
        isFailsafeActive = true;
        brakeCar_Dynamic(); // 立即短路馬達繞組，利用反電動勢 (Back-EMF) 瞬間制動！
        digitalWrite(buzPin, HIGH); delay(50); digitalWrite(buzPin, LOW); // 警報音
        digitalWrite(ledPin, LOW); // 熄滅就緒燈
        Serial.println(F("[FAILSAFE_ALERT] Comm Timeout > 300ms! Dynamic Brake Triggered!"));
      }
      return; // 斷線狀態下直接返回，拒絕執行任何殘留舊指令！
    }
  }

  // 🎯 接收樹莓派序列埠指令
  if (Serial.available() > 0) {
    char cmd;
    while (Serial.available() > 0) { cmd = Serial.read(); } // 讀取最新指令
    
    // 收到任何字元皆代表通訊鏈路正常，立即「餵狗 (Feed Watchdog)」！
    lastHeartbeatTime = millis();
    
    // 若原本處於 Failsafe 狀態，通訊恢復後解除鎖定
    if (isFailsafeActive) {
      isFailsafeActive = false;
      digitalWrite(ledPin, HIGH);
      Serial.println(F("[FAILSAFE_INFO] Communication Restored. System Normal."));
    }

    // 💓 收到獨立心跳包 'H'，僅更新時間戳，無需其他動作
    if (cmd == 'H') {
      return;
    }
    
    if (cmd == 'X') { isAutoMode = true; digitalWrite(ledPin, HIGH); return; }
    if (cmd == 'x') { isAutoMode = false; digitalWrite(ledPin, LOW); brakeCar_Dynamic(); attackCount = 0; return; } 
    
    // 收到 'S' 指令時執行煞車，不解除自動模式
    if (cmd == 'S') { brakeCar_Dynamic(); return; } 
    
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
          moveBackward(); delay(150); 
          stopCar(); delay(200);
          
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
// 🛠️ 馬達運動控制（馬力分配：左 200 / 右 240~255）
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

// 🛑 =========================================================
// 🛑 【主動動態電磁煞車 (Active Dynamic Braking via Back-EMF)】
// 🛑 =========================================================
// 原理：將 L298N H 橋雙下臂開關同時導通 (IN1=LOW, IN2=LOW) 並將 PWM 設為 255 (ENA/ENB=HIGH)，
// 將直流馬達線圈兩端直接短路。馬達旋轉時的反電動勢 (Back-EMF) 會在短路迴路中產生
// 巨大逆向感應電流與阻尼制動力矩 (Lenz's Law)，在 8cm 內定桿煞停，徹底消除慣性衝撞！
void brakeCar_Dynamic() {
  digitalWrite(LEFT1, LOW);   digitalWrite(LEFT2, LOW);   analogWrite(LEFT_PWM, 255);
  digitalWrite(RIGHT1, LOW);  digitalWrite(RIGHT2, LOW);  analogWrite(RIGHT_PWM, 255);
}

// 🌿 慣性滑動停車 (Coasting - PWM 歸零，H 橋開路浮接 High-Z)
void stopCar() {
  digitalWrite(LEFT1, LOW);   digitalWrite(LEFT2, LOW);   analogWrite(LEFT_PWM, 0);
  digitalWrite(RIGHT1, LOW);  digitalWrite(RIGHT2, LOW);  analogWrite(RIGHT_PWM, 0);
}