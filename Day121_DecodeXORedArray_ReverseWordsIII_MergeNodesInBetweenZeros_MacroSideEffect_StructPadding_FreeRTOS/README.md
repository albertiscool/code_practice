# Day 121 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-20
- **訓練主軸**: 異或代數逆運算與差分還原 (Decode XORed Array)、字串純指標雙指針滑動視窗原地翻轉 (Reverse Words in a String III)、以 0 分隔封包鏈結串列原地節點複用合併 (Merge Nodes in Between Zeros)、巨集運算子優先級與重複求值副作用 (`MAX(x++, y++)`)、結構體自然對齊 Padding 計算法則、中斷服務常式 (ISR) 禁忌與「上半部 / 下半部」分工架構、互斥鎖 (Mutex) 與號誌 (Semaphore) 本質差異 (所有權 Ownership) 與優先級反轉 (Priority Inversion) 繼承機制、自走車 FreeRTOS 多工任務優先級架構 (100 Hz 硬即時馬達 PID 控制 vs 通訊下半部)。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #1720 - Decode XORed Array
- **考點對照**: 群聯 SSD 控制器 RAID 5/6 條帶差分校驗鏈 (Parity XOR Chain) 與資料逆向還原。
- **核心思維**:
  - 根據 XOR 自反代數性質：$A \oplus B = C \iff B = A \oplus C$。
  - 得出遞迴關係式：`arr[i + 1] = arr[i] ^ encoded[i]`。
  - 單趟 $O(N)$ 迴圈推導，空間複雜度除回傳陣列外嚴格 $O(1)$。
  - 零提示純白板一次直接 AC！
- **LeetCode 成績**:
  - **狀態**: Accepted (76/76 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 25.3 MB
  - **Submission ID**: `2176824957`

---

### P2: LeetCode #557 - Reverse Words in a String III
- **考點對照**: 群聯 HackMD 考古「字串反轉」升級題型；廣達/和碩韌體團隊純指標原地邊界探測。
- **核心思維**:
  - 雙指標滑動視窗：`slow` 錨定單詞開頭，`fast` 探測單詞結尾邊界（`' '` 或 `'\0'`）。
  - 遇到空格時，在原始緩衝區上以 `while (slow < fast)` 原地對調字元，輔助空間為 $O(1)$。
  - 迴圈結束後，嚴謹補上收尾的最後一個單詞翻轉，邊界邏輯滴水不漏。
  - 零提示純白板一次直接 AC！
- **LeetCode 成績**:
  - **狀態**: Accepted (29/29 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 9.5 MB
  - **Submission ID**: `2176843494`

---

### P3: LeetCode #2181 - Merge Nodes in Between Zeros
- **考點對照**: 韌體通訊協議定界符 (0x00 Delimiter / COBS Framing) 封包重組與鏈結串列結構走訪。
- **核心思維**:
  - **極致原地節點複用 (In-Place Node Reuse)**：
    - 直接複用原始串列中的 `0` 節點來存放各區段的累加值（第一個 0 存放 4，第二個 0 存放 11），不額外配置任何新節點，達到 0 `malloc` 開銷！
    - 沿途主動呼叫 `free(fast)` 釋放中間合併節點，記憶體零洩漏。
  - **末端邊界截斷防禦**：
    - 當 `fast->next == NULL`（到達最後一個定界 0 節點）時，直接令 `slow->next = NULL` 並呼叫 `free(fast)`，完美剔除結尾多餘的 0。
- **LeetCode 成績**:
  - **狀態**: Accepted (39/39 Passed)
  - **記憶體**: 99 MB (0 額外 heap 配置)
  - **Submission ID**: `2176887551`

---

## Part 2: 面試經典致命陷阱題

### Trap 1: 巨集運算子優先級與重複求值副作用
- **問題分析**:
  `#define SQUARE(x) (x * x)` 呼叫 `SQUARE(a + 1)`；
  `#define MAX(a, b) ((a) > (b) ? (a) : (b))` 呼叫 `MAX(x++, y++)`（$x=5, y=10$）。
- **解答與深度推導**:
  1. `SQUARE(a + 1)` 展開為 `(a + 1 * a + 1)`。乘法優先級高於加法，當 $a=3$ 時計算為 $3 + 3 + 1 = \mathbf{7}$（非 16）。
     - 防禦寫法：`#define SQUARE(x) ((x) * (x))`。
  2. `MAX(x++, y++)` 展開為 `((x++) > (y++) ? (x++) : (y++))`：
     - 條件比對 `5 > 10` 為 False，結算後 $x=6, y=11$；
     - 隨後執行 False 分支 `(y++)`，回傳當前值 **11**（`res2 = 11`），接著 $y$ 再次遞增變為 **12**！
     - 最終：`res2 = 11, x = 6, y = 12`（$y$ 被加了兩次！）。
  3. **現代 C 語言標準替代方案**：使用 **`static inline` 函式**。函式在進入前以 Call-by-Value 各自求值一次，徹底杜絕巨集重複求值的副作用，同時享有與巨集同等的零調用開銷。

---

### Trap 2: 結構體記憶體對齊與填充 (Structure Padding)
- **問題分析**:
  `struct DataA { char a; int b; short c; };`
  `struct DataB { char a; short c; int b; };`
- **解答與記憶體分佈**:
  1. **大小計算**：
     - `sizeof(struct DataA) = 1 + 3(pad) + 4 + 2 + 2(pad) = 12 Bytes`
     - `sizeof(struct DataB) = 1 + 1(pad) + 2 + 4 = 8 Bytes`
     - 在 32-bit 與 64-bit 系統上大小完全相同（因成員為 char, short, int，兩架構下長度均為 1, 2, 4 bytes，無指標型別）。
  2. **記憶體排列差異**：
     - `DataA` 中，`char a` 後面為了讓 `int b` 對齊在 4 的倍數，浪費了 3 bytes padding；且末端為了滿足最大對齊寬度 4，又補齊了 2 bytes padding。
     - `DataB` 將 `short c` 緊接在 `char a` 後，僅需 1 byte padding，後續的 `int b` 剛好落在 offset 4，末端無需任何 padding，省下近 33% 記憶體！

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: 中斷服務常式 (ISR) 編寫禁忌與「上半部 / 下半部」架構
1. **ISR 嚴格禁忌之底層機制**：
   - **不可重入 (Non-reentrant)**：`printf()` 與 `malloc()` 內部使用全域 buffer 或 Heap 鍊表臨界區，ISR 中途插隊呼叫會引發資料損毀或死鎖。
   - **無獨立 Task Context**：ISR 運行在中斷上下文，無獨立 TCB 視窗，排程器無法對其進行 Block/Sleep，進入阻塞直接觸發 HardFault 當機。
   - **中斷飢餓**：長期佔用 ISR 會屏蔽其他中斷，引發通訊掉包。
2. **上半部 (Top Half) 與下半部 (Bottom Half) 標準架構**：
   - **Top Half（ISR 內部，微秒級）**：清中斷旗標、硬體暫存器資料搬入 RAM 環狀緩衝區、送出 Semaphore / Task Notification，隨即退出。
   - **Bottom Half（RTOS Task / Workqueue，毫秒級）**：在普通執行緒上下文中被喚醒，允許睡眠與等待，處理複雜的 CRC 校驗、浮點數運算與封包解析。

---

### Q2: 互斥鎖 (Mutex) vs 號誌 (Semaphore) 與優先級反轉
1. **所有權 (Ownership) 根本差異**：
   - **Mutex 具備所有權（誰借誰還）**：只有成功 Take 該 Mutex 的 Task 才有權限 Give 釋放它。中斷 ISR 不是 Task，無法持有 Mutex，因此 **ISR 內部絕對禁止使用 Mutex**。
   - **Semaphore 無所有權（同步通知機制）**：Task A 等待信號，可由 ISR 或 Task B 發出信號，適合事件通知與生產者-消費者模型。
2. **優先級反轉與優先級繼承 (Priority Inheritance)**：
   - **反轉現象**：低優先級 Task L 佔有共享資源，高優先級 Task H 等待該資源而阻塞；此時無關的中優先級 Task M 搶佔了 Task L 的 CPU 時間，導致最高優先級的 Task H 反過來被中優先級的 Task M 延遲！
   - **優先級繼承解法**：當 Task H 嘗試獲取已被 Task L 持有的 Mutex 時，RTOS 自動將 Task L 的優先級**暫時提升至與 Task H 相同**，防止被 Task M 插隊；待 Task L 釋放 Mutex 後，其優先級立即恢復原狀，Task H 立即搶佔執行！

---

## 🚗 自走車專案實務科普

### 今日主題：自走車 FreeRTOS 多工即時排程 —— 100 Hz 硬即時馬達 PID 控制 vs 通訊解析下半部分工架構

#### 1. 面試痛點場景（廣達、和碩、台積電 IMC 高頻提問）
面試官提問：
> *「在你的智慧車專案中，MCU 既要以 100 Hz 週期精準計算雙輪馬達的 PID 閉迴路控制，同時上位機樹莓派又隨時會透過 UART 傳送大量的導航封包與感測器請求。如果通訊資料突然爆量，你的底層系統如何保證馬達控制不會發生抖動（Jitter）、延遲甚至轉速失控？」*

#### 2. FreeRTOS 多工排程架構設計
在自走車專案中，絕不能將通訊解析與 PID 計算塞在同一個 `while(1)` 迴圈中，必須依即時性需求進行**優先級分層排程**：

1. **Task 1: Motor PID Control Task (最高優先級，如 Priority 4，Hard Real-Time 100 Hz)**
   - **週期嚴格確定性**：使用 FreeRTOS 的 `vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));` 確保每 10ms 精準喚醒一次。
   - **工作內容**：讀取編碼器 Counter、計算增量式 PID、寫入 Timer PWM 暫存器（CCR）。
   - **特點**：執行時間極短（< 0.3ms），**絕對不允許呼叫任何可能阻塞的 I/O**，即使通訊塞車，PID 迴路也能以最高優先級搶佔 CPU 運算，保證速度控制 100% 穩定無抖動！

2. **Task 2: UART Command Processing Task (中優先級，如 Priority 2，通訊下半部)**
   - 平時處於 Blocked 狀態，不佔用任何 CPU。
   - 當上位機傳來一包 32-byte 的 `cmd_vel` 封包時，UART DMA 觸發中斷（上半部），中斷以 `vTaskNotifyGiveFromISR()` 在 2 微秒內喚醒此任務。
   - 本任務負責耗時的封包解包、CRC16 校驗、合法性檢查，完成後更新全域目標速度 `target_vel`。

3. **Task 3: Telemetry & Heartbeat Task (低優先級，如 Priority 1)**
   - 背景回傳當前車速、電池電壓、超音波距離給上位機，可隨時被 Task 1 或 Task 2 搶佔。

#### 3. 資料共享防競態：為什麼不用 Mutex？
* **問題**：Task 2 更新 `target_vel`，Task 1 每 10ms 讀取 `target_vel`，兩者如何避免競態（Race Condition）？
* **面試應答亮點**：
  * 在 32-bit ARM Cortex-M 架構下，`int32_t` 或 `float` 變數的讀寫**本身就是單週期完成的硬體原子操作（Atomic Access）**！
  * 使用 Mutex 頻繁加鎖反而會引入額外的上下文切換（Context Switch）與排程器開銷。
  * 若有多個變數需要同時同步（如線速度與角速度），工業級做法是在寫入時使用極短的臨界區遮蔽中斷：
    ```c
    taskENTER_CRITICAL();
    shared_target_linear  = parsed_linear;
    shared_target_angular = parsed_angular;
    taskEXIT_CRITICAL();
    ```
  * 臨界區僅耗時數個 CPU 週期（納秒級），既保證了多變數的一致性，又完全不影響 100 Hz PID 迴路的硬即時表現！
