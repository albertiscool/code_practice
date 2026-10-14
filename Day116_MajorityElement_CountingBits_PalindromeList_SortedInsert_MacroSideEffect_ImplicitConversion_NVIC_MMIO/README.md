# Day 116 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-14
- **訓練主軸**: 摩爾投票演算法 (Boyer-Moore Voting)、位元動態查表計數 (Bitwise DP Popcount)、回文鏈結串列 (快慢指標與頭插法反轉)、有序鏈結串列零特判插入 (Sorted Insert)、巨集文字替換副作用與未定義行為 (UB)、無符號數隱式型態轉換與二補數溢位、ARM NVIC 搶占優先級中斷巢狀、MMIO 暫存器指標操作、一線 IC 廠雙重指標尾插真題拆解。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、聯發科 (MTK)、瑞昱 (Realtek)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #169 - Majority Element (多數元素)
- **核心思維**: 摩爾投票演算法 (Boyer-Moore Voting Algorithm)。
  - 利用「同歸於盡抵消原理」，將多數元素與任何相異元素一對一抵消。
  - 多數元素出現次數 $> \lfloor n / 2 \rfloor$，任何非多數元素全數抵消後，最終存活下來的必定是多數元素。
  - 時間複雜度 $O(N)$，空間複雜度 $O(1)$，完美淘汰 Hash Table ($O(N)$ 空間) 與排序 ($O(N \log N)$ 時間)。
- **LeetCode 成績**:
  - **狀態**: Accepted (58/58 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 11.2 MB
  - **Submission ID**: `2171972883`

### P2: LeetCode #338 - Counting Bits (位元計數)
- **核心思維**: 位元動態查表 (Bitwise DP Popcount)。
  - 核心狀態轉移方程：`result[i] = result[i & (i - 1)] + 1;`
  - `i & (i - 1)` (Brian Kernighan 操作) 會消去 $i$ 的最低位 1，其數值嚴格小於 $i$，因此該狀態在前面的迴圈中早已計算完畢。
  - 單次遍歷 $O(N)$ 線性時間完成，0 巢狀迴圈、0 分支跳轉。
- **LeetCode 成績**:
  - **狀態**: Accepted (15/15 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 15.6 MB
  - **Submission ID**: `2171979028`

### P3: LeetCode #234 - Palindrome Linked List (回文鏈結串列)
- **核心思維**: 三階段綜合實戰。
  1. 快慢指標找中點：`fast->next != NULL && fast->next->next != NULL`，使 `slow` 停在前半段末端。
  2. 後半段「頭插法 (Head-Insertion)」原地反轉：
     `curr->next = next_node->next; next_node->next = pre->next; pre->next = next_node;`
  3. 雙指標同步比對前半段與後半段值。
- **LeetCode 成績**:
  - **狀態**: Accepted (93/93 Passed)
  - **執行時間**: 2 ms
  - **記憶體**: 45.1 MB
  - **Submission ID**: `2171996945`

### P4: 韌體經典 - Sorted Linked List Insertion (有序鏈結串列插入節點)
- **核心思維**: Dummy Node (虛擬頭節點) 或 Linus Torvalds Double Pointer (`struct ListNode**`)。
- **亮點與除錯**:
  - 終止條件短路保護：`while (curr != NULL && curr->val <= val)`，防止空串列與尾部插入觸發 NULL 指標解引用 HardFault。
  - 零 `if-else` 特判：借助 Dummy Node，頭部、中間、尾部插入完全統合成兩行代碼 (`pre->next = new; new->next = curr;`)。
  - 本地 5 大極限測試案例 (空串列、插在最開頭、插在中間、插在最後面、重複數值插入) 全數通過。

---

## Part 2: 面試經典致命陷阱題

### Trap 1: 運算子優先級與巨集副作用 (Macro Side Effect)
- **程式碼**:
  ```c
  #define SQUARE(x) (x * x)
  int a = 3;
  int b = SQUARE(a++);
  ```
- **解答**: `a = 5`，`b = 12` (或編譯器相關未定義行為)。
- **原理剖析**:
  - 巨集純粹是文字展開：展開為 `(a++ * a++)`。`a++` 被執行了兩次，因此 `a` 最終值為 5。
  - 在 C 語言標準中，同一表達式內對同一變數進行多次自增且無序列點 (Sequence Point)，屬於**未定義行為 (Undefined Behavior, UB)**。
  - 加括號 `((x) * (x))` 只能解決運算子優先級，無法解決參數被重複求值的副作用。正規替代方案為 **`static inline` 函式**。

### Trap 2: `unsigned` 與 `signed` 隱式型態轉換陷阱
- **程式碼**:
  ```c
  int a = -20;
  unsigned int b = 6;
  if (a + b > 6) { printf("Condition TRUE\n"); }
  ```
- **解答**: 印出 `Condition TRUE`。
- **原理剖析**:
  - Usual Arithmetic Conversions：當 signed int 與 unsigned int 同階運算時，signed int 被隱式轉型為 unsigned int。
  - 數學上的 `-14` 在 32-bit 二補數中被解讀為 `0xFFFFFFF2 = 4,294,967,282`，遠大於 6。
  - **韌體災難**: 環形佇列計算剩餘空間 `if (head - tail < capacity)`，當指針環繞 `head = 2, tail = 5` 時，`2 - 5 = -3` 變成 42.9 億，韌體誤判空間無限，導致嚴重的 Buffer Overflow 踩爛相鄰記憶體並引發 HardFault。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: ARM NVIC 搶占優先級 (Preemption) vs 子優先級 (Subpriority)
1. **中斷巢狀 (Interrupt Nesting) 規則**:
   - 只有**搶占優先級 (Preemption Priority) 嚴格更高 (數值更小)** 的中斷才能打斷當前正在執行的中斷。
   - 子優先級 (Subpriority) 完全不具備打斷與搶占能力！
2. **優先級相同時的決策**:
   - 若搶占優先級相同，同時到達時比對子優先級，子優先級數值小者先執行。
   - 若兩者完全相同，依據硬體固化的中斷向量編號 (IRQ Number) 由小到大排定執行順序。

### Q2: 記憶體映射 I/O (MMIO) 與底層暫存器指標操作
1. **MMIO 原理**:
   - ARM 將硬體外設暫存器直接映射到 CPU 4GB 統一記憶體空間中 (如 `0x40000000` 開始的周邊匯流排)。
   - CPU 存取硬體暫存器與存取 SRAM 完全使用相同的組語指令 (`LDR` / `STR`)。
2. **標準 C 語言暫存器操作語法**:
   ```c
   *(volatile uint32_t *)0x40021018 |= (1U << 5);
   ```
   - `volatile`：防止編譯器將寫入操作優化拔除或快取於暫存器 (R0)，強制每次存取實體匯流排。
   - `1U << 5`：使用無符號常數防範 signed 溢位。

---

## Part 4: 自走車專案實務科普 (第 116 講)
### 馬達旋轉編碼器 (Encoder) 4 倍頻正交解碼與速度 PID 閉迴路控制
- **AB 相正交訊號與 4 倍頻 (4x Mode)**:
  - 霍爾/光電編碼器輸出相位差 $90^\circ$ 的兩路方波 (Channel A 與 Channel B)，根據相位先後判斷旋轉方向。
  - 同時監聽 A/B 相的上升緣與下降緣，一個週期獲得 4 個計數邊緣，使解析度提升 4 倍。
- **硬體定時器編碼器模式 (Timer Encoder Interface)**:
  - 嚴禁使用 GPIO 外部中斷軟體數脈衝 (高速轉動時每秒數萬次中斷會導致 CPU 飽和癱瘓)。
  - 由 MCU Timer 硬體自動硬線解碼計數器 (`CNT`)，CPU 佔用率為 0%。
- **速度 PID 控制迴路**:
  - 每隔 10ms 採樣一次增量計數 $\Delta \text{CNT}$ 計算當前線速度，經增量式 PID 即時動態調整輸出 PWM，抵禦電池壓降與負載阻力。

---

## Part 5: 一線 IC 廠 (Novatek/Phison) 鏈結串列面試真題深度拆解
- **題目特徵**: 給定隨機產生 10 顆節點的鏈結串列，輸出中點值與前半段最大值。
- **核心實作**:
  - `Node **head_nod, Node **last_node`：雙重指標尾插法，在修改外部指標的同時維護尾節點，達成 $O(1)$ 常數時間尾插。
  - 快慢指標找中點 (LeetCode #876)：`while (fast->next && fast->next->next)` 使 slow 精準停在 10 顆節點的第 5 顆 (`mid: 856`)。
  - 單趟遍歷極值追蹤：slow 推進同時維護 `max_val`，於 $O(N)$ 單趟掃描中同步找出前半段最大值 (`largest: 884`)。
