# Day 57: 碩士班 AI 智慧車專題 5 大硬體底層面試題大滿貫 (UART 波形與暫存器、DMA 零CPU搬運、PWM ARR/CCR 算術、mmap 零拷貝與 ARM LDREX/STREX 原子指令)

今天是將研究所《嵌入式微處理機》AI 智慧車專題（Arduino + 樹莓派 ROS 2 + YOLOv8 + L298N 馬達 + 舵機刺氣球）與晶片底層硬體原理、記憶體架構及一線 IC 設計廠（聯發科、瑞昱、聯詠等）軟韌體高頻面試題整合特訓。我們順利完成了全部 5 大底層專題面試考題的觀念拆解與實作。

---

## 📝 實戰考題與詳細解答 (Smart Car Low-Level Firmware Interview Questions)

### 題目 1【UART 序列埠：1 個 Byte 在底層暫存器與電位波形如何傳送？】
* **專題對應**：`Serial.write('A')` (Arduino 刺擊指令) 與 `ser.write(b'X')` (樹莓派自動模式)。
* **解析與解答**：
  1. **資料暫存器與移位暫存器**：CPU 透過 8 位元平行匯流排將字元 `'A'` (ASCII `0x41` / `0b01000001`) 寫入 UART Data Register (`UDR`/`DR`)。`UDR` 將資料平行載入 **移位暫存器 (Shift Register)**。
  2. **TX 引腳波形輸出**：移位暫存器依據 Baud Rate 於 TX 銅線推出波形：
     * **Start Bit (0V)** ➡️ **資料位元 (LSB 0b1 先送，依序至 MSB 0b0)** ➡️ **Stop Bit (5V)**。
  3. **RX 硬體中斷與計數器**：接收端 RX 引腳偵測下降緣啟動移位暫存器。位元計數器數至 8 採樣完資料，**數至第 9 個點判定必須為 Stop Bit (5V)** (若為 0V 則發動 Framing Error `FE` 旗標)。收滿 8 位元寫入接收 `UDR` 並觸發 **UART RX 硬體中斷 (USART_RX_IRQHandler)**。

### 題目 2【DMA (Direct Memory Access)：如何做到 0 次 CPU 介入傳送記憶體？】
* **專題對應**：巨量感測器 / 相機串流資料傳輸優化。
* **解析與解答**：
  1. **中斷 vs DMA 效能比對**：1024 Bytes 資料若走中斷會引發 **1024 次 CPU 中斷** (頻繁 PUSH/POP 暫存器導致死機)；改走 DMA 全過程 **僅在最後 1024 Bytes 搬完時發出 1 次 Transfer Complete (TC) 中斷**。
  2. **DMA 暫存器配置**：源頭位址 `PAR` (UART DR `0x40001004`，**位址不遞增 Fixed**)、目的位址 `MAR` (RAM `buffer[1024]`，**位址自動遞增 +1 Increment**)、數量 `NDTR` (1024)。
  3. **進階乒乓雙緩衝 (Circular Mode)**：開啟 `Half-Transfer` (HT) 與 `Transfer-Complete` (TC) 雙中斷，CPU 與 DMA 錯開解析與寫入，達成無縫連綿傳輸。

### 題目 3【PWM 馬達調速與定時器暫存器 (ARR & CCR) 算術】
* **專題對應**：`analogWrite(LEFT_PWM, 200)` 與 `analogWrite(RIGHT_PWM, 240)` 差速調速。
* **解析與解答**：
  1. **Timer 3 大暫存器**：`CNT` (時脈計數器)、`ARR` (Auto-Reload Register 決定 **PWM 總週期與頻率**)、`CCR` (Capture/Compare Register 決定 **高電位時間與占空比**)。
  2. **`analogWrite()` 底層**：修改暫存器 `CCR` 之數值。當 `CNT < CCR` 腳位輸出 5V HIGH，`CNT >= CCR` 輸出 0V LOW。
  3. **占空比與電壓算術**：`analogWrite(pin, 127)` 占空比為 $\frac{127}{255} \approx \mathbf{50\%}$，馬達感應平均有效電壓為 $5\text{V} \times 50\% = \mathbf{2.5\text{V}}$。

### 題目 4【Shared Memory (mmap) 零拷貝記憶體傳送真相】
* **專題對應**：`camera_node.py` 擷取影像發給 `yolo_detector_node.py` 進行 YOLO 氣球辨識。
* **解析與解答**：
  1. **ROS 2 預設 Topic 拷貝痛點**：走預設套接字/管道，資料由 User A 複製至 Kernel 再複製至 User B，發生 **2 次 `memcpy`**，吃滿記憶體頻寬。
  2. **共享記憶體 (`shm_open` + `mmap`) 零拷貝**：Linux 核心 **MMU (Memory Management Unit)** 將 Process A 與 Process B 的虛擬頁表直接映射到 **同一個實體 RAM 頁框** 上，存取資料達成 **0 次 `memcpy`** (Zero-Copy)。

### 題目 5【多任務競態條件 (Race Condition) 與 Mutex / ARM 原子指令】
* **專題對應**：`isAutoMode` 旗標與 `attackCount` 同時遭按鈕中斷與 Serial 指令修改。
* **解析與解答**：
  1. **`count++` 組合語言 3 步驟**：`LOAD` (RAM 到 CPU 暫存器) ➡️ `ADD` (ALU 加 1) ➡️ `STORE` (寫回 RAM)。中途若遭搶態，資料會被覆蓋踩壞引發 Race Condition。
  2. **ARM 底層原子指令**：Mutex 上鎖防線於 ARM Cortex 處理器底層採用 **`LDREX` (Load Exclusive)** 與 **`STREX` (Store Exclusive)** 獨佔式讀寫硬體原子指令，保證上鎖過程絕不遭中斷打斷。
