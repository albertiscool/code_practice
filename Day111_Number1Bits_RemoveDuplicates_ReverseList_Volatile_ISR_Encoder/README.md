# Day 111: 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式 (位元計數 Kernighan × 原地雙指標去重 × 鏈結反轉三巨頭 × 巨集/sizeof 陷阱 × volatile/ISR 核心 × 編碼器四倍頻)

## 📑 今日訓練概覽
- **P1 (位元運算神技之王)**: LeetCode #191: Number of 1 Bits (Easy) - Brian Kernighan 演算法 `n & (n - 1)` 抹除最低位 1，嚴格 $O(K)$ 硬體級別優化 (0 ms, Beats 100.00%, Sub ID: 2166104125)
- **P2 (陣列雙指標原地覆寫)**: LeetCode #26: Remove Duplicates from Sorted Array (Easy) - 快慢指標（Two Pointers）原地覆寫不變量維護，嚴格 $O(1)$ 額外空間通用解 (0 ms, Beats 100.00%, Sub ID: 2166108452)
- **P3 (鏈結串列白板絕對之王)**: LeetCode #206: Reverse Linked List (Easy) - 經典三指標手術（`pre`, `curr`, `next_node`）行雲流水原地反轉，防護斷鏈與記憶體洩漏 (0 ms, Beats 100.00%, Sub ID: 2166111312)
- **面試經典致命陷阱題**:
  - 巨集（Macro）展開括號陷阱、四則運算優先級與 `a++` 副作用引發未定義行為（UB）
  - `sizeof` vs `strlen` 差異，以及陣列作為函式參數時強制退化為指標（`int *`）之尺度突變
- **基本 0x10 韌體核心觀念題**:
  - `volatile` 核心物理意義（禁止暫存器快取與指令重排）與三大必備情境（MMIO、ISR 旗標、多執行緒共享）
  - 中斷服務常式（ISR）三大致命禁忌（嚴禁阻塞、嚴禁 `printf`/`malloc` 等不可重入函式、無回傳無參數）與 Top-Half / Bottom-Half 解偶架構
- **自走車專案實務科普 (第 111 講)**:
  - 光電編碼器（Encoder）定時器硬體解碼模式（Timer Encoder Mode）防範中斷風暴（Interrupt Storm）
  - A/B 正交相位硬體四倍頻（4x Decoding）提升解析度原理
  - 測速演算法三大對決：M 法（定時間測脈衝，高速適用）vs T 法（定脈衝測時間，低速適用）vs M/T 法全速域結合

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Number of 1 Bits (#191 - Easy)
- **核心思維**:
  - 傳統逐位檢查需要跑滿 32 次迴圈。
  - Brian Kernighan 演算法核心：`n = n & (n - 1)`。
  - 每次運算向最低位的 1 借位，兩者做 AND 正好將最低位的 1 抹除為 0，其餘高位保持不變。
  - 迴圈只執行 $K$ 次（$K$ 為 1 的個數），時間複雜度縮短至 $O(K)$。
- **韌體防禦亮點**:
  - 宣告 `uint32_t un = (uint32_t)n;` 杜絕有號數下溢未定義行為（Signed Underflow UB）。

### P2: Remove Duplicates from Sorted Array (#26 - Easy)
- **核心思維**:
  - 快慢指標原地覆寫：`slow` 指向最後一個已確定的不重複元素，`fast` 負責向前探索。
  - 當 `nums[fast] != nums[slow]` 時，先將慢指標前進 `++slow`，再將新元素覆寫進去 `nums[slow] = nums[fast]`。
- **關鍵避坑點**:
  - 函式回傳值為新長度 $k$，索引從 0 開始，因此總個數為 `slow + 1`，嚴禁誤回傳指標 `nums`。
  - 快指標從 `fast = 1` 開始出發，自然涵蓋單一元素陣列（`numsSize == 1` 時不進入迴圈直接回傳 1）。

### P3: Reverse Linked List (#206 - Easy)
- **核心思維**:
  - 三指標原地轉向：`pre`（前驅）、`curr`（當前）、`next_node`（後繼備份）。
  - 在改變方向前，必須先備份 `next_node = curr->next`，避免單向鏈結串列斷鏈丟失。
  - 依序執行：`curr->next = pre; pre = curr; curr = next_node;`。
- **面試追問點（迭代 vs 遞迴）**:
  - 遞迴法佔用 Call Stack 空間（$O(N)$），在節點數龐大時會引發 Stack Overflow 導致 HardFault 當機。
  - 迭代法（$O(1)$ 空間）是工業級韌體的唯一標準選擇！

---

## 💣 面試經典致命陷阱題複盤

### 陷阱 1：巨集（Macro）副作用與優先級
```c
#define SQUARE(x) x * x
#define DOUBLE(x) (x + x)
int a = 3;
int res1 = SQUARE(a + 1); // 7 (3 + 1 * 3 + 1)
int res2 = 10 / DOUBLE(2); // 2 (10 / (2 + 2))
int res3 = SQUARE(a++);   // Undefined Behavior (同一運算式內兩次修改變數)
```
- **規範**：巨集參數內部與最外層皆必須包覆圓括號：`#define SQUARE(x) ((x) * (x))`，且嚴禁傳入具副作用的變數（如 `a++`）。

### 陷阱 2：`sizeof` 陣列參數傳遞退化
```c
void func(int arr[10]) {
    printf("%zu\n", sizeof(arr)); // 64-bit 系統上印出 8 (退化為 int*)
}
```
- **規範**：C 語言中陣列無法作為函式參數傳遞，`int arr[10]` 宣告實質等價於 `int *arr`，`sizeof(arr)` 計算的是指標本身大小而非陣列容量。

---

## ⚔️ 基本 0x10 韌體核心面試題複盤

### 題目一：`volatile` 關鍵字
- **物理意義**: 告知編譯器該變數值隨時會被外部事件修改，禁止將其快取在暫存器中，每次讀寫皆必須實體走匯流排存取記憶體。
- **三大時機**:
  1. 硬體周邊暫存器映射（Memory-Mapped IO）。
  2. 中斷服務常式（ISR）與主迴圈共享的 Flag 變數。
  3. 多任務/多執行緒環境下的共享變數。

### 題目二：中斷服務常式（ISR）三大禁忌
- **禁忌 1**: 嚴禁呼叫任何可能造成阻塞（Blocking）的函式（如 `delay_ms()`、獲取信號量）。
- **禁忌 2**: 嚴禁呼叫不可重入（Non-Reentrant）函式（如 `printf()`、`malloc()`、`free()`），防範 Heap 損毀與死鎖。
- **禁忌 3**: 不能有返回值，亦不能接受輸入參數。
- **最佳實務**: 採用 Top-Half（硬體響應、讀暫存器、置位標記）與 Bottom-Half（交由主任務進行耗時運算）解偶設計。

---

## 🚗 自走車實務科普：光電編碼器與測速演算法

### 1. 硬體四倍頻（4x Decoding）
- 透過偵測 A 相與 B 相的上升沿與下降沿（每個週期 4 個邊緣），將編碼器每圈線數解析度提升 4 倍（如 500 線變為 2000 個計數點）。
- 利用硬體定時器正交編碼器介面（Timer Encoder Mode）全硬體自動計數，杜絕 GPIO 外部中斷引發的中斷風暴（Interrupt Storm）。

### 2. M 法 vs T 法 測速演算法
- **M 法（定時間測脈衝）**: 固定 $\Delta T$ 時間計算脈衝增量 $M$。高速精度極佳，但低速時脈衝稀疏，量化誤差巨大。
- **T 法（定脈衝測時間）**: 量測相鄰脈衝之間經過的高頻時鐘週期。極低速時精度極高，但高速時容易溢位。
- **工業級切換**: 自走車低速起步/倒車採用 T 法，高速巡航切換為 M 法，確保全速域速度閉環控制穩定。
