# Day 119 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式 (HackMD 考古專題 II)

## 訓練概況
- **日期**: 2026-10-19
- **訓練主軸**: HackMD 群聯面試共筆深度實戰化、位元差異與翻轉計數 (Minimum Bit Flips to Convert Number)、已排序陣列缺失數字探測與二分搜尋 (Kth Missing Positive Number)、鏈結串列雙虛擬頭節點原地位移分流 (Partition List)、特定位址寫入修飾詞 (`volatile` & `const`) 與對齊寬度防禦、函式參數傳遞機制 (Call by Value vs Pointer vs Reference) 底層組語映射、Stack vs Heap 記憶體佈局與高可靠韌體禁用執行期動態分配、暫存器位元操作四部曲 (Get/Set/Clear/Toggle) 與 RMW 原子性硬體解決方案 (BSRR / Bit-Band)、自走車雙核心通訊斷線安全防護 (Heartbeat Failsafe) 與獨立看門狗 (IWDG) 架構。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #2220 - Minimum Bit Flips to Convert Number
- **HackMD 考點對照**: 對應筆記中電訪與面試核心「`a & (~a + 1)` 與 `a & (a - 1)` 位元操作」、「暫存器狀態 Bit 差異比對與翻轉計數」。
- **核心思維**:
  - 利用 XOR 互斥或運算 (`start ^ goal`) 精準萃取出所有相異位元（相同為 0，相異為 1）。
  - 使用 Brian Kernighan 演算法 `a = a & (a - 1)` 快速消除最右側的 1，以 $O(\text{1 的個數})$ 常數級迴圈完成計數。
  - 採用 `unsigned int` 避免符號擴展與 signed integer overflow 溢位異常。
- **LeetCode 成績**:
  - **狀態**: Accepted (250/250 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 8.4 MB
  - **Submission ID**: `2175255129`

---

### P2: LeetCode #1539 - Kth Missing Positive Number
- **HackMD 考點對照**: 直擊共筆核心「群聯三題」第 1 題與第 2 題（給定已排序陣列印出缺失數字）＋ 筆記考古第 9 題「給一段 Binary Search code 解釋」。
- **核心思維**:
  - **方案 A (直覺線性模擬, $O(N + k)$)**：維護陣列指標與自然數計數器，逐一比對與遞減 $k$。
  - **方案 B (面試加分項, $O(\log N)$ 二分搜尋法)**：
    - 在索引 `mid` 處，理論值為 `mid + 1`，因此缺失數字總數為 `missing = arr[mid] - (mid + 1)`。
    - 若 `missing < k`，表示第 $k$ 個缺失數在右半邊 (`left = mid + 1`)；否則在左半邊 (`right = mid - 1`)。
    - 迴圈終止時，由代數化簡得出答案即為 `left + k`！
- **LeetCode 成績**:
  - **狀態**: Accepted (87/87 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 8.8 MB
  - **Submission ID**: `2175275371`

---

### P3: LeetCode #86 - Partition List (分割鏈結串列)
- **HackMD 考點對照**: 對應筆記考古「linklist insert」與「群聯三題」第 3 題鏈結串列結構指標走訪重組。
- **核心思維**:
  - **雙虛擬頭分流法 (Two Dummy Nodes Demux)**：配置兩個 Stack 虛擬頭節點 `lessHead` 與 `greaterHead`，分別收集 $< x$ 與 $\ge x$ 的節點，天然保證 FIFO 原始相對順序。
  - **致命韌體防禦關鍵**：分流結束後，必須手動執行 `greater->next = NULL;` 截斷大於分區尾端，防止形成環狀死循環（Cycle Fault / TLE）。
  - 將 `less->next` 接上 `greaterHead.next`，回傳 `lessHead.next`，兼顧 $O(1)$ 額外空間與零記憶體洩漏。
- **LeetCode 成績**:
  - **狀態**: Accepted (169/169 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 10.5 MB
  - **Submission ID**: `2175596833`

---

## Part 2: 面試經典致命陷阱題 (取材自 HackMD 原題)

### Trap 1: 記憶體特定位址寫入與修飾詞排列
- **問題分析**:
  `int volatile * const p_reg = (int *) 0x1234; *p_reg = 0xaa55;`
- **解答與深度推導**:
  1. **為什麼必須加 `volatile`？**
     - 防止編譯器開 `-O2/-O3` 執行 Dead Store Elimination（死碼消除），誤以為該寫入無意義而直接刪除指令；
     - 防止輪詢暫存器時被快取在 CPU 暫存器（如 R0）中，保證每次存取皆發出實體記憶體匯流排存取週期（`STR` / `LDR`）。
  2. **`const` 在 `*` 右邊的語意**：
     - 代表指標變數本身是常數（不可修改其指向的硬體位址 `0x1234`），但指向的硬體暫存器數值可被修改。
     - 若寫成 `const volatile int *` 則代表唯讀暫存器（軟體不可寫入，但硬體狀態會隨時更新）。
  3. **記憶體對齊與寬度陷阱**：
     - 數值 `0xaa55` 僅為 16-bit 暫存器值，若宣告為 32-bit `int *`，寫入時會寫入整整 4 個 bytes，將相鄰的 `0x1236` 暫存器覆蓋破壞！
     - 工業界嚴謹宣告必須使用 `uint16_t`：`#define REG_DEV (*(volatile uint16_t * const)(0x1234))`。

---

### Trap 2: 參數傳遞機制陷阱 (Call by Value vs Pointer vs Reference)
- **問題分析**:
  ```cpp
  void change(int *a, int &b, int c) { c = *a; b = 3; *a = 2; }
  void main() { int a = 1, b = 2, c = 3; change(&a, b, c); printf("%d %d %d\n", a, b, c); }
  ```
- **解答與底層剖析**:
  1. **輸出結果**：`2 3 3`。
  2. **為什麼 `c` 不變？**
     - `c` 是傳值調用（Call by Value），Caller 僅複製一份數值副本傳入 Callee 的 Stack Frame / 暫存器中，內部修改的只是區域副本，返回後 `main` 的 `c` 完好如初。
  3. **指標（`int *`）與參考（`int &`）的底層關聯**：
     - 在 C++ 語法層面，參考是別名且不可為 NULL；
     - 在編譯器生成的組合語言中，**兩者底層完全 100% 相同**！編譯器都是傳遞變數的記憶體位址（`ADD R1, SP, #offset`），並在函式內透過指標間接定址寫入（`STR R3, [R1]`）。參考本質上就是由編譯器自動解引用的 `Type * const` 常數指標。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題 (取材自 HackMD 原題)

### Q1: Stack vs Heap 底層架構差異與高可靠韌體禁忌
1. **本質架構對比**：
   - **Stack**：向下增長，由硬體指標暫存器 `SP` 單週期加減（納秒級），0 碎裂、0 尋找開銷，函式結束自動退棧。
   - **Heap**：向上增長，由 C Runtime / OS 記憶體管理器維護 Free List 鍊表，存在 Chunk Header 與對齊額外開銷。
2. **高可靠韌體（MISRA C / 車載 / 醫療）禁用執行期 `malloc` 之三大死穴**：
   - **外部碎片化 (External Fragmentation)**：長時間運行後零碎小塊過多，突發大封包分配直接回傳 `NULL` 導致系統崩潰。
   - **非確定性延遲 (Jitter)**：鍊表遍歷時間隨碎片程度變長，破壞硬即時（Hard Real-Time）控制週期的 Deadline。
   - **記憶體洩漏 (Memory Leak)**：長期運行風險。
   - **工業級替代方案**：採用靜態配置的固定大小記憶體池（Fixed-size Memory Pool）與環狀緩衝區（Ring Buffer），借還皆為 $O(1)$ 且 0 碎片。

---

### Q2: 暫存器位元操作四部曲與 RMW 原子性防禦
1. **讀取 Bit 18 (嚴格回傳 0 或 1)**：
   `uint8_t bit = (reg >> 18) & 1;` 或 `((reg & (1 << 18)) != 0);`
2. **Set Bit 18**：`reg |= (1 << 18);`
3. **Clear Bit 18**：`reg &= ~(1 << 18);`
4. **Toggle Bit 18**：`reg ^= (1 << 18);`
5. **RMW 致命 Race Condition 與硬體解決方案**：
   - `reg |= (1 << 18)` 在底層是三條指令：`LDR` (讀取) $\to$ `ORR` (修改) $\to$ `STR` (寫回)。
   - 若在 `LDR` 之後恰好被高優先級 ISR 中斷，ISR 修改了同一個暫存器的其他 bit 並寫回；當中斷返回時，主程式以舊值計算後的結果覆蓋回暫存器，將 ISR 的更動硬生生抹殺！
   - **硬體原子性解法**：
     - **STM32 BSRR (Bit Set/Reset Register)**：寫入 1 代表觸發設置/清除，寫入 0 無作用。硬體單週期以獨立匯流排脈衝完成，無需 Read，達到純原子操作（Atomic）。
     - **ARM Cortex-M Bit-Band（位元帶）**：將暫存器的每一個 bit 映射到一個獨立的 32-bit 虛擬記憶體位址，對該位址進行字組寫入即可直接原子化操作目標 bit。

---

## 🚗 自走車專案實務科普

### 今日主題：雙核心自走車「通訊斷線安全防護 (Heartbeat Failsafe)」與「硬體獨立看門狗 (IWDG)」雙重保險架構

#### 1. 面試痛點場景（廣達、和碩、機器人團隊最高頻考題）
面試官提問：
> *「在你的智慧車專案中，上位機是樹莓派 Linux 跑 ROS 2，下位機是 STM32/Arduino 驅動雙馬達。如果車子正在以最高速向前衝，樹莓派的導航節點突然崩潰（Crash）、Linux 卡死、或是連接兩者的 USB-UART 線意外鬆脫拔掉，你的車子會發生什麼事？底層韌體如何保證車子不會失控撞人？」*

#### 2. 第一重防護：軟體心跳超時安全制動 (Communication Heartbeat Failsafe)
* **單純接收指令的致命缺陷**：若韌體單純依靠收到 `cmd_vel` 調整馬達 PWM，當通訊突然中斷時，最後一次收到的指令「全速前進」會一直留在暫存器中，馬達持續高速旋轉，造成車輛暴衝。
* **工業級通訊心跳設計**：
  1. **封包心跳標記 (Heartbeat Packet)**：樹莓派以固定頻率（如 20 Hz, 每 50ms）下發帶有循環計數（Sequence ID）的控制指令。
  2. **韌體看門計時器 (Firmware Soft Watchdog)**：STM32 啟動一個硬體定時器（如 TIM6, 100ms 週期），維護一個超時變數 `uint32_t last_cmd_timestamp`。
  3. **超時制動邏輯 (Failsafe Trigger)**：
     ```c
     void Check_Communication_Timeout(void) {
         if ((HAL_GetTick() - last_cmd_timestamp) > 200) { // 超過 200ms 未收到有效封包
             Set_Vehicle_State(STATE_FAILSAFE);
             Motor_Emergency_Stop(); // 強制緊急制動！
         }
     }
     ```
  4. **主動電氣煞車 (Active Dynamic Braking)**：
     - 單純將 PWM 設為 0 只是讓馬達「滑行（Coast）」，車體仍會因慣性往前滑動撞牆。
     - 正確做法：將 H 橋驅動器（如 L298N 或 MOSFET）的 `IN1=HIGH, IN2=HIGH`（或兩端皆接地），把馬達兩端直接短路，利用旋轉產生的**反電動勢（Back-EMF）**產生強大的反向阻尼力矩，在最短距離內將車體鎖死煞停！

#### 3. 第二重防護：微控制器硬體獨立看門狗 (Independent Watchdog, IWDG)
* **若下位機單晶片自己當機怎麼辦？**
  - 如果單晶片本身因為死迴圈、堆疊溢位（Stack Overflow）或 HardFault 卡死，定時器中斷也跟著癱瘓，軟體超時機制將完全失效。
* **IWDG 硬體保險機制**：
  - IWDG 擁有完全獨立於主時脈的專屬低速內部振盪器（LSI，約 32 kHz），即使主晶片的主震盪器（HSE）故障停擺，IWDG 依然能獨立倒數。
  - 主迴圈中定時餵狗（`HAL_IWDG_Refresh()`），若單晶片在 500ms 內未能及時餵狗，IWDG 會立即發動硬體強制復位（Hardware MCU Reset）。
  - 單晶片 Reset 瞬間，所有 GPIO 立即回到預設的高阻抗（High-Z）或弱下拉狀態，H 橋驅動信號全部斷開，確保馬達在微秒級瞬間斷電停轉。

#### 4. 面試應答總結亮點
向面試官總結：
> *「我們的雙核心架構具備『兩層異步容錯安全機制』：第一層是上位機通訊層的 200ms 心跳監聽，遇到斷線或 ROS 節點崩潰時立即觸發主動電氣反電動勢煞車；第二層是微控制器底層獨立於主時脈的硬體 IWDG，確保單晶片本體發生韌體死鎖時能在 500ms 內硬體重置並將驅動腳位歸零高阻抗，達成全系統的故障安全（Failsafe）。」*
