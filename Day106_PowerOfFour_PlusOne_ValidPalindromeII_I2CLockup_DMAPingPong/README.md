# Day 106: 群聯 Phison ＋ 電子五哥 韌體實戰模式 (位元遮罩 × 大數加一 × 雙指標回文分支 × I2C 死鎖救援 × UART DMA 雙緩衝)

## 📑 今日訓練概覽
- **P1 (位元運算與硬體暫存器遮罩)**: LeetCode #342: Power of Four (Easy) - `n & (n - 1) == 0` 二的冪次 + `0x55555555` 奇數位元遮罩 (0 ms, Beats 100.00%)
- **P2 (陣列原地操作與記憶體配置)**: LeetCode #66: Plus One (Easy) - 原地進位檢測 + 全 9 溢位擴展 `digitsSize + 1` 配置 (0 ms, Beats 100.00%)
- **P3 (雙指標與字串驗證)**: LeetCode #680: Valid Palindrome II (Easy) - 貪心盲點突破 (Greedy Ambiguity) + 輔助子函式二選一分支驗證 (0 ms, Beats 100.00%)
- **自走車專案實務科普 (第 106 講)**: 雙核心通訊協議狀態機（UART Binary Packet FSM Parser）與防撕裂機制
  - 幀頭 `0xAA 0x55` + Length + Command + Payload + Checksum
  - MCU 端非阻塞 Byte-by-Byte FSM 解析器
  - 馬達 EMI 突波防護與 200ms Communication Watchdog / Heartbeat Failsafe 急停機制
- **0x10 韌體架構實戰情境題**:
  1. **I2C 匯流排死鎖與 9 個 SCL Pulse 救援 SOP**: Master 讀取途中重啟引發 Slave 咬死 SDA 死鎖原理，9 個 Pulse 物理意義（8 Data Bits + 1 ACK/NACK），GPIO 模擬救援 SOP 與 STOP 訊號生成。
  2. **UART DMA 雙緩衝 (Ping-Pong Buffer) 與資料撕裂防護**: 單一緩衝區 Data Tearing 出事原理、STM32 Circular DMA 搭配 Half-Transfer (HT) / Transfer-Complete (TC) 無鎖（Lock-Free）協作機制、Buffer Overrun（被套圈）軟硬體因應與硬體流量控制（RTS/CTS）。

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Power of Four (#342 - Easy)
- **核心思維**:
  1. `n > 0` 且 `(n & (n - 1)) == 0` 保證是 2 的冪次方（二進制只有單一一個 bit 為 1）。
  2. 4 的冪次方（$4^0=1, 4^1=4, 4^2=16\dots$）其唯一為 1 的 bit 必然座落在**奇數位（偶數 index：第 0, 2, 4, 6... 位）**。
  3. 利用硬體遮罩 `0x55555555` (`0b01010101...`)：若 `(n & 0x55555555) == n`，則必為 4 的冪次方。
- **關鍵陷阱**:
  - C 語言運算子優先級：`==` 高於 `&`！若寫 `n & 0x55555555 == n` 會被解析成 `n & (0x55555555 == n)`，必須加括號 `((n & 0x55555555) == n)`。

### P2: Plus One (#66 - Easy)
- **核心思維**:
  - 從陣列尾端往前走：若當前數字小於 9，直接加一並即時回傳原陣列（$O(1)$ 額外空間）。
  - 若當前數字為 9，則原地改為 0 並繼續向前進位。
  - 若一路進位到首位仍溢出（如 `999...`），重新 `malloc(digitsSize + 1)`，將首位設為 1，其餘補 0。
- **關鍵陷阱**:
  - 忘記賦值 `*returnSize = digitsSize;`，導致 LeetCode 評判平台無法讀取結果長度而噴錯。

### P3: Valid Palindrome II (#680 - Easy)
- **反例字串**: `"abccbca"`
- **貪心盲點 (Greedy Ambiguity)**:
  - 遇到 `s[left] != s[right]` 時，若單純看眼下一步 `s[left+1] == s[right]`，可能會走進死胡同（因為兩邊同時滿足眼下一步，但只有一邊後面能成完整回文）。
  - 因為最多只能刪除 1 個字元，正確做法是轉移為二選一分支驗證：
    `isSubPalindrome(s, left + 1, right) || isSubPalindrome(s, left, right - 1)`
- **雙重累加陷阱**:
  - `if` 區塊內部有 `left++`，外部迴圈末端不可重複累加，避免一次跨步 2 格造成漏檢。

---

## 🚗 自走車實務科普：雙核心通訊協議狀態機與 Failsafe

### 二進位幀結構（Little-Endian）
```text
+-----------------------+--------+---------+--------------------+---------+
| Header (Magic 2-Byte) | Length | Command | Payload (速度資料) | Checksum|
|      0xAA   0x55      |  0x04  |  0x01   |   V_L(2B) + V_R(2B)|  Sum &  |
+-----------------------+--------+---------+--------------------+---------+
```

### 多層防護體系
1. **FSM 非阻塞解析**: 單 Byte 驅動狀態機（`WAIT_HEADER1` -> `WAIT_HEADER2` -> `GET_LEN` -> `GET_CMD` -> `GET_PAYLOAD` -> `VERIFY_CHECKSUM`），髒資料即時丟棄。
2. **200ms Communication Watchdog**: MCU 內部計時，若持續因雜訊校驗失敗超過 200ms，強制進入 Failsafe 緊急煞停（PWM=0），杜絕盲衝。
3. **物理層排查**: 星狀共地、馬達電樞並聯 $0.1\,\mu\text{F}$ 消除火花電容、長距離高雜訊環境升級差動訊號（RS-485 / CAN Bus）。

---

## ⚔️ 0x10 韌體架構實戰情境題深度複盤

### 題目一：I2C 匯流排死鎖與 9-Pulse 救援術
- **死鎖成因**:
  - Master 在讀取途中意外重開機（Brown-out Reset），此時 SDA 控制權在 Slave 手上。
  - Slave 正輸出低電位 `0` 且在死等下一個 SCL Clock；Master 開機後見 SDA 為 Low，硬體 I2C 控制器誤判 `BUSY` 永久卡死。
- **「9」的物理意義**:
  - 1 Byte 傳輸包含 **8 Data Bits + 1 ACK/NACK Bit = 9 個 SCL Clock**。送出 9 個脈衝可確保 Slave 吐完當前 Byte 並交出 SDA。
- **救援 SOP**:
  1. GPIO 切為 Open-Drain 輸出模式。
  2. 手動 Toggle SCL 9 次。
  3. Master 維持 SDA 為 High（給予 NACK），並在 SCL=High 時將 SDA 由 Low 拉高製造合法 **STOP Condition**。
  4. 切回硬體 I2C 模式並執行 `I2C_Init()`。

### 題目二：UART DMA 雙緩衝 (Ping-Pong Buffer) 與資料撕裂防護
- **單一 Buffer 痛點 (Data Tearing)**:
  - 生產者（DMA）覆蓋速度大於消費者（CPU）解析速度時，DMA 環狀回繞覆蓋 CPU 正在讀取的區域，拼裝出新舊混雜的「科學怪人封包」。
- **Circular DMA 雙緩衝無鎖協作**:
  - 陣列長度 256B：前半 128B（Buffer A）、後半 128B（Buffer B）。
  - **HT (Half-Transfer) 中斷**: DMA 寫滿 A，硬體不停機自動寫 B；CPU 收到中斷放心地去處理 A。
  - **TC (Transfer-Complete) 中斷**: DMA 寫滿 B，硬體不停機自動繞回寫 A；CPU 收到中斷放心地去處理 B。
- **DMA 套圈（Buffer Overrun）應對**:
  1. **任務解耦**: 中斷僅做指標交換或快速投遞 Ring Buffer，耗時解析移至低優先權 Task。
  2. **逾期偵測與丟包**: 設計 Busy 旗標，偵測到超車立即記錄錯誤並主動捨棄髒資料。
  3. **硬體流量控制 (RTS/CTS)**: Buffer 快飽和時 MCU 拉高 RTS 暫停發送端傳輸，從物理層徹底根除溢位。
