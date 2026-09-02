# 🏎️ 【期中專題面試簡報範本】：智慧自駕車底層控制與多感測器融合系統
## Embedded Motion Control & Multi-Sensor Fusion Autonomous Car Platform

> **專案定位**：基於 ATmega328P 微控制器之即時運動控制、閉迴路循跡與防撞安全系統  
> **核心技術**：C/C++、硬體 PWM、L298N H 橋電機驅動、IR 循跡演算法、超音波測距、藍牙狀態機、PICSimLab 虛擬硬體驗證

---

## 📑 投影片架構設計（建議 4~5 頁 PPT）

---

### 🖼️ Slide 1: 專案目標與系統硬體架構 (System Architecture)

* **專案背景與目標**：
  * 設計一台具備「自主循跡 (Line Tracking)」、「即時避障 (Obstacle Avoidance)」、「藍牙遙控 (Bluetooth Teleoperation)」三大模式切換之智慧移動小車。
  * 結合軟體模擬（PICSimLab 虛擬環境）與實體硬體驗證，確保控制邏輯在硬體組裝前 100% 驗證完畢。

* **系統硬體架構方塊圖**：
  * **主控核心 (MCU)**：ATmega328P (16 MHz, 32KB Flash, 2KB SRAM)
  * **執行機構 (Actuators)**：L298N 雙通道 H 橋驅動模組 + 雙直流減速馬達 + SG92R 伺服舵機 (轉向雲台)
  * **感知系統 (Sensors)**：5 路紅外線循線感測器陣列 (IR) + HC-SR04 超音波測距模組
  * **通訊模組 (Wireless)**：HC-05/06 藍牙串列模組 (UART 9600 bps)

---

### 🖼️ Slide 2: 核心運動控制與循跡演算法 (Motion Control & Line Tracking)

#### 1. 雙輪差速運動學模型 (Differential Drive Kinematics)
* 利用左右輪獨立 PWM 占空比產生轉速差，實現前進、後退、原地旋轉 (In-Place Turn) 與平滑弧形轉向。

#### 2. 紅外線循線決策邏輯 (Line Tracking Algorithm)
* 透過 5 路紅外線數位訊號（白底黑線反射率差異），建立即時轉向偏差查表與閉迴路調速控制：
  * 偏左 ➔ 調高右輪 PWM、調低左輪 PWM，動態向右修正。
  * 偏右 ➔ 調高左輪 PWM、調低右輪 PWM，動態向左修正。
  * 急彎/直角彎 ➔ 單輪反轉進行原地急轉。

---

### 🖼️ Slide 3: 超音波避障與有限狀態機架構 (Obstacle Avoidance & FSM)

#### 1. 超音波測距原理與物理推導 (Ultrasonic Physics)
* 利用聲速 $340 \text{ m/s} = 0.03434 \text{ cm/}\mu\text{s}$，推導出超音波單程距離公式：
  $$\text{距離 (cm)} = \frac{\text{Echo 脈衝時間 (}\mu\text{s)}}{2} \div 29.1$$
* 建立前向警戒安全距離閥值（$< 15 \text{ cm}$ 觸發主動防撞）。

#### 2. 多模式切換有限狀態機 (Finite State Machine, FSM)
* 系統運行四種狀態平滑轉換：
  * `STATE_IDLE` (待機停止)
  * `STATE_LINE_TRACKING` (自動循線模式)
  * `STATE_AVOIDANCE` (避障繞道模式)
  * `STATE_MANUAL_BT` (手機藍牙手動遙控模式)

---

### 🖼️ Slide 4: 重大工程挑戰、除錯實戰與架構優化 (Challenges & Architecture Analysis)

#### 🛠️ 1. 【實際實作驗證】馬達啟動電源干擾與防撞除錯 (Implementation & Debugging)
* **問題**：實體車輛在馬達啟動瞬間，超音波測距偶發性出現 0cm 雜訊。
* **解決**：
  * 在超音波電源端加入濾波電容，並在軟體端實作連續採樣中值濾波法 (Median Filter)，剔除突波干擾。

#### 💡 2. 【架構分析與優化提案】L298N 制動機制與 Timer 輸入捕獲 (Proposed Optimization)
* **L298N 主動動態煞車 (Dynamic Braking) 理論分析**：
  * 深入分析傳統 `PWM=0` 慣性滑行 (Coasting) 的煞車距離缺陷，提出將 `IN1=LOW, IN2=LOW, PWM=255` 兩端短路之架構方案，利用**馬達反電動勢 (Back-EMF)** 產生電磁力矩達成瞬間急煞。
* **Timer Input Capture 取代 `pulseIn()` 提案**：
  * 分析 `pulseIn()` 阻塞 CPU 達 30ms 之問題，提出利用硬體 Timer 輸入捕獲中斷，達成 0% CPU 佔用與微秒級精準測距。

---

### 🖼️ Slide 5: 實体验證成果與技能總結 (Key Achievements & Skills)

* **成果展示**：
  * PICSimLab 模擬器中完成 100% 邏輯預驗證。
  * 實體小車順利通過 S 彎軌道循線、遇到障礙物自動煞停轉向、手機 App 藍牙即時遙控。
* **技術關鍵字**：`C/C++`、`ATmega328P`、`L298N Motor Control`、`PWM`、`UART`、`FSM`、`PICSimLab`、`Sensors Integration`。
