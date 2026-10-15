# Day 117 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-15
- **訓練主軸**: 二進位間距解析 (Binary Gap)、陣列奇偶原地雙指標穿插 (Sort Array By Parity II)、單向鏈結串列奇偶雙鏈跳步重組 (Odd Even Linked List)、巨集展開副作用與運算子優先級陷阱、`sizeof` 不求值特性與陣列退化 (Array Decay)、RTOS Mutex 擁有權 vs Semaphore 同步與優先級繼承 (Priority Inheritance)、暫存器 Read-Modify-Write (RMW) 競態與 ARM Cortex-M BSRR 單週期原子寫入、自走車 I2C 匯流排鎖死與九脈衝解鎖術 (9-Clock Recovery)。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #868 - Binary Gap (二進位間距)
- **核心思維**: $O(\log n)$ 位元逐位右移掃描（Bitwise Shift & Mask）。整數 $n$（$1 \le n \le 10^9$）在 32-bit 架構下最多僅約 30 個有效位元，利用 `while (n > 0)` 每次檢查最低位 `n & 1`，並維護 `last` 索引與 `max` 距離，迴圈最多僅跑 32 次，兼具極致常數時間與 $O(1)$ 空間複雜度。
- **邊界盲點與除錯**:
  - **致命運算子優先級陷阱**：原始特判寫法 `if (n & (n - 1) == 0)`，因關係運算子 `==` (Level 9) 優先級高於位元運算子 `&` (Level 8)，實際被編譯器解析為 `n & ((n - 1) == 0)`，導致特判完全失效！必須手動加上括號 `if (((n & (n - 1)) == 0)`。
  - **分支消除優化**：不論是否創下新最大間距，只要遇到 bit 1 皆必須統一更新 `last = i`。
- **LeetCode 成績**:
  - **狀態**: Accepted (261/261 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 8.3 MB
  - **Submission ID**: `2172875870`

---

### P2: LeetCode #922 - Sort Array By Parity II (按奇偶排序陣列 II)
- **核心思維**: 雙指標原地就地交換（In-place Two-Pointer Swap）。指標 `i` 巡檢偶數索引（$0, 2, 4 \dots$），指標 `j` 巡檢奇數索引（$1, 3, 5 \dots$）。僅在 `nums[i]` 為奇數且 `nums[j]` 為偶數時進行一對一交換，單次遍歷 $O(N)$ 時間、$O(1)$ 額外空間，杜絕任何額外記憶體分配。
- **邊界盲點與除錯**:
  - **致命陣列越界 (Off-by-One)**：原始終止條件寫為 `while (fast <= numsSize)`，合法索引上限為 `numsSize - 1`，引發緩衝區溢位越界存取。修正為嚴格小於 `< numsSize`。
  - **雙指標交換不變量 (Loop Invariant) 遺失**：原始程式碼僅單向檢查奇數索引是否放錯，未檢查偶數索引當前是否也是放錯的，直接交換反而破壞了原本正確的偶數位置。原地交換必須嚴格建立在「兩端同時放錯」的前提下。
- **LeetCode 成績**:
  - **狀態**: Accepted (61/61 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 17.8 MB
  - **Submission ID**: `2172886387`

---

### P3: LeetCode #328 - Odd Even Linked List (奇偶鏈結串列)
- **核心思維**: 雙鏈分離跳步縫合法（Leapfrog Split & Stitch）。建立 `odd` 與 `even` 雙指標分別推進奇數序號與偶數序號節點，並保留 `evenHead` 作為偶數鏈起點。迴圈內部交替跳步（`odd->next = even->next; even->next = odd->next;`），結束後直接 `odd->next = evenHead;` 縫合兩條鏈結。
- **邊界盲點與除錯**:
  - **尾端動態搬移之環狀死結 (Cycle Formation)**：原始試圖以 `tail->next = curr` 將偶數節點掛至尾端，但未切斷 `curr->next = NULL`，且 `tail` 未同步推進，導致尾部回指形成閉環死鎖，程式陷入無窮迴圈。
  - **滿分解法優勢**：無需預算長度或尋找尾端，奇偶雙鏈各自跳步天然免疫閉環，以常數空間 $O(1)$、單趟遍歷 $O(N)$ 達成無縫拼接。
- **LeetCode 成績**:
  - **狀態**: Accepted (70/70 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 11.9 MB
  - **Submission ID**: `2172895526`

---

## Part 2: 面試經典致命陷阱題

### Trap 1: 巨集展開副作用與運算子優先級陷阱
- **問題分析**:
  ```c
  #define SQUARE(x) (x * x)
  #define MIN(a, b) ((a) < (b) ? (a) : (b))
  int a = 100 / SQUARE(2 + 3);
  int x = 5, y = 10;
  int b = MIN(x++, y);
  ```
- **解答與深度推導**:
  1. **`a = 9`**：
     * `SQUARE(x)` 定義含有外層括號但缺乏參數括號，展開為 `100 / (2 + 3 * 2 + 3) = 100 / (2 + 6 + 3) = 100 / 11 = 9`。
     * 若巨集完全無括號才會是 $100 / 2 + 3 \times 2 + 3 = 59$。此題展示了雙重陷阱考法。
  2. **`b = 6, x = 7`**：
     * 展開為 `((x++) < (y) ? (x++) : (y))`。
     * 先求條件式：$5 < 10$ 為真，跨過三元運算子 `?` 循序點，`x` 第一次自增變為 6。
     * 進入真分支執行第二個 `(x++)`：表達式取值為 6（賦值給 `b`），隨後執行第二次自增，`x` 最終變為 7。
  3. **防禦手段**：
     * 嵌入式系統首選 **`static inline` 內聯函式**，具備編譯器型別檢查與引數僅計算一次的安全保證。
     * GCC 擴展環境可使用語句表達式與 `typeof` 暫存變數（如 Linux 核心的 `min()` 實作）。

---

### Trap 2: `sizeof` 不求值副作用與陣列退化 (Array Decay)
- **問題分析**:
  ```c
  void process_buffer(int buffer[16]) {
      int counter = 10;
      size_t s1 = sizeof(buffer);
      size_t s2 = sizeof(counter++);
      printf("s1 = %zu, s2 = %zu, counter = %d\n", s1, s2, counter);
  }
  ```
- **解答與深度推導**:
  1. **`s1 = 8, s2 = 4, counter = 10`**（64-bit 架構下）。
  2. **陣列退化 (Array Decay)**：C 語言規範中，陣列作為函式參數傳遞時會自動退化為指向其首元素的指標（`int buffer[16]` 等同於 `int *buffer`），常數 16 完全被編譯器忽略，因此 `sizeof(buffer)` 得到的是指標大小（64-bit 系統下為 8 bytes）。
  3. **編譯期不求值 (Unevaluated Operand)**：依據 C99 6.5.3.4 標準，`sizeof` 是純編譯期運算子，編譯器僅依據運算式之靜態型別（`int`）直接填入常數 4，底層不產生任何機器碼，因此 `counter++` 在執行期完全不會被執行，`counter` 維持 10。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: RTOS Mutex 擁有權 vs Semaphore 同步與優先級反轉
1. **本質與擁有權 (Ownership)**：
   * `Mutex` 具有嚴格的「擁有權（Ownership）」概念，誰上鎖（Take）就必須由誰解鎖（Give），專注於臨界區互斥保護。
   * `Semaphore` 專注於信號傳遞（Signaling）與資源計數，任何任務或中斷皆可釋放（Give）與等待（Take）。
2. **ISR 嚴禁使用 Mutex 之原因**：
   * ISR 沒有獨立的任務控制塊 (TCB)，絕不可進入 Blocked/Sleep 阻塞狀態。
   * ISR 不是任務，無法登記為 Mutex 的擁有者（Owner），因此 FreeRTOS 嚴格禁止且未提供 `TakeMutexFromISR` API。
3. **優先級反轉與優先級繼承 (Priority Inheritance)**：
   * 當高優先級任務 H 等待低優先級任務 L 所持有的鎖，而無關的中優先級任務 M 搶佔 L 執行時，導致 H 間接被 M 卡死。
   * Mutex 透過**優先級繼承機制**，動態將持有鎖的 L 之優先級拉升至與 H 相同，壓制 M 的搶佔，促使 L 迅速脫離臨界區並釋放鎖，隨後 L 恢復原優先級，H 立即搶佔執行。

---

### Q2: 暫存器 Read-Modify-Write (RMW) 競態與 ARM Cortex-M BSRR
1. **標準位元操作巨集**:
   * 置 1 (Set Bit 3)：`REG |= (1U << 3);`
   * 清 0 (Clear Bit 5)：`REG &= ~(1U << 5);`
2. **RMW 致命競態條件**:
   * `REG |= (1 << 3)` 在底層編譯為 3 條指令：`LDR` (讀取舊值) $\to$ `ORR` (修改位元) $\to$ `STR` (寫回暫存器)。
   * 若在 `LDR` 之後、`STR` 之前突然爆發硬體中斷，ISR 修改了同一個暫存器的其他位元（如 Bit 7）並寫回；當中斷返回後，主程式將舊值寫回暫存器，**會直接將 ISR 所做的修改完全抹除覆蓋**，產生隨機且極難復現之嚴重硬體異常。
3. **ARM Cortex-M BSRR 單週期原子解法**:
   * STM32 等晶片提供專門的 **BSRR (Bit Set/Reset Register)**：
     * 低 16 位寫 1 代表置 1，寫 0 代表無動作。
     * 高 16 位寫 1 代表清 0，寫 0 代表無動作。
   * 因「寫 0 無動作」，軟體無須事先讀取其他 Pin 腳之現狀，只需一條單週期 `STR` 指令即可精準修改目標 Pin 腳，天然具備 100% 硬體級原子性，徹底杜絕 RMW 競態。

---

## 🚗 自走車專案實務科普

### 今日主題：自走車 I2C 匯流排鎖死（I2C Bus Lockup）與「九脈衝解鎖術（9-Clock Recovery）」
- **問題場景**: 自走車雙核心架構中，馬達急加速/轉向之大電流突波或主控看門狗重置，經常導致 I2C 通訊在傳輸資料或 ACK 期間意外中斷。
- **鎖死機制**: Slave 感測器（如 MPU6050 陀螺儀）硬體狀態機持續將 SDA 資料線拉在 LOW 狀態等待時脈；而 Master 重啟後無法在 LOW 的 SDA 線上下發 START/STOP 條件，造成 I2C 匯流排永久卡死（Bus Deadlock）。
- **九脈衝軟體自癒術 (9-Clock Recovery Routine)**:
  1. 驅動層檢測到 I2C 超時死鎖（SDA == 0）。
  2. 暫時關閉硬體 I2C 控制器，將 SCL 與 SDA 引腳切換為 GPIO 開汲極模式。
  3. 由 Master 手動在 SCL 敲擊最多 9 個 Clock 脈衝，引導 Slave 將當前 Byte 剩餘位元送完並釋放 SDA（SDA 回復為 High）。
  4. 手動產生標準 I2C STOP 條件重置 Slave 狀態機。
  5. 重新初始化硬體 I2C 控制器，恢復感測器通訊。
- **面試亮點**: 展現深厚之硬體訊號與驅動層除錯能力，大幅超越僅會呼叫抽象函式庫的初階工程師。
