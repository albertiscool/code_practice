# Day 122 研發替代役 / 碩士預聘實戰訓練

## 📋 本日訓練大綱
* **Part 1 白板程式題（HackMD 考古題高頻變形，無提示純白板實戰）**
  * **P1**: LeetCode #485 - Max Consecutive Ones（陣列滑動計數、短路求值保護）
  * **P2**: LeetCode #415 - Add Strings（大數加法、字元轉數值、原地雙指標翻轉）
  * **P3**: LeetCode #1669 - Merge In Between Linked Lists（單向鏈結串列區間摘除與拼接、動態記憶體 free 防漏）
* **Part 2 面試經典致命陷阱題**
  * **Trap 1**: 複合運算子優先級與副作用（`*p1++`, `(*p2)++`, `++*p3`, `*++p4`）
  * **Trap 2**: 陣列下標語法糖本質與指標算術（`ptr[2]` vs `2[ptr]`, `&array[6] - &array[2]`）
* **Part 3 基本 0x10 韌體與作業系統觀念題**
  * **Q1**: 靜態環形佇列 (Ring Buffer) vs 動態鏈結串列 (LinkedList)（記憶體碎片、Determinism、Cache Locality）
  * **Q2**: 韌體中嚴格禁止遞迴 (Recursion) 的硬體本質（Stack Overflow、MISRA C Rule 17.2、迭代與有限深度堆疊改寫）
* **自走車專案實務科普（系統廠 / IC 設計廠面試亮點）**
  * 馬達編碼器正交解碼（Quadrature Encoder）、中斷風暴防範與 PID 閉迴路速度控制

---

## 💻 Part 1 白板程式題總結

### 1. P1: LeetCode #485 - Max Consecutive Ones
* **檔案路徑**：[Day122/p1.c](file:///c:/Users/a0907/Desktop/程式訓練/Day122/p1.c)
* **核心觀念**：
  * 邊界防禦與短路求值（Short-circuit Evaluation）：`fast < numsSize && nums[fast] == 1`，邊界檢查必須置於 `&&` 左側。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 45/45 測試案例。

### 2. P2: LeetCode #415 - Add Strings
* **檔案路徑**：[Day122/p2.c](file:///c:/Users/a0907/Desktop/程式訓練/Day122/p2.c)
* **核心觀念**：
  * 大數加法模擬：雙指標從最低有效位（`len1 - 1`, `len2 - 1`）向前遍歷，進位旗標 `carry` 維護。
  * 記憶體配置計算：最大可能長度為 `max(len1, len2) + 1`（最高位進位），加上結尾 `\0` 故空間需 `max_len + 2`。
  * 原地字串反轉：雙指標收斂交換。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 317/317 測試案例。

### 3. P3: LeetCode #1669 - Merge In Between Linked Lists
* **檔案路徑**：[Day122/p3.c](file:///c:/Users/a0907/Desktop/程式訓練/Day122/p3.c)
* **核心觀念**：
  * 單向鏈結串列區間替換：使用虛擬頭節點（`dummy`）保護邊界。
  * 韌體面試亮點：在拼接前主動使用 `free(curr)` 釋放刪除節點（$a$ 到 $b$），防止 Memory Leak。
  * 時間複雜度 $O(N + M)$，空間複雜度 $O(1)$。
* **LeetCode 成績**：**Accepted (148 ms)**，通過 61/61 測試案例。

---

## 🎯 Part 2 面試陷阱題解析

### Trap 1: 複合運算子優先級與副作用
```c
int a[] = {10, 20, 30, 40, 50};
int *p1 = a, *p2 = a, *p3 = a, *p4 = a;

int val1 = *p1++;    // 後置++優先級高但先取值：val1 = 10, p1 指向 a[1]
int val2 = (*p2)++;  // 括號先解引用再後置++：val2 = 10, a[0] 變為 11, p2 不變
int val3 = ++*p3;    // 右至左結合：a[0] 先加 1 變 12，val3 = 12, p3 不變
int val4 = *++p4;    // 前置++先遞增指標至 a[1]，再解引用：val4 = 20, p4 指向 a[1]
```
* **結果**：
  * `val1 = 10`, `val2 = 10`, `val3 = 12`, `val4 = 20`
  * `a[0] = 12`, `a[1] = 20`
  * `p1, p2, p3, p4` 偏移分別為：`1, 0, 0, 1` 個整數。

### Trap 2: 陣列下標語法糖本質與指標算術
```c
int array[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
int *ptr = array;
```
1. **下標語法糖**：
   根據 ISO C 標準：`E1[E2]` 恆等於 `*((E1) + (E2))`。因此 `ptr[2]` 編譯器直接轉為 `*(ptr + 2)`。
2. **`2[ptr]` 與 `4[array]`**：
   因加法交換律，`*(2 + ptr)` 與 `*(ptr + 2)` 等價。語法完全合法，`2[ptr] = 30`，`4[array] = 50`。但工程上違反規範，嚴禁使用。
3. **指標相減 `&array[6] - &array[2]`**：
   數值為 **`4`**。兩同型別指標相減，結果以**「元素個數」**（`ptrdiff_t`）計，而非 Byte 數。只有強制轉為 `char*` 相減才會得到位元組數（16 Bytes）。

---

## 🧠 Part 3 基本 0x10 韌體與作業系統觀念題

### Q1: 靜態 Ring Buffer vs 動態 LinkedList 在韌體中的抉擇
1. **外部碎片（External Fragmentation）與確定性（Determinism）**：
   * 動態 LinkedList 頻繁在 Heap 執行 `malloc`/`free`，長期運作會產生不可避免的記憶體外部碎片，最終導致配置失敗當機。
   * `malloc` 需搜尋空閒鏈結表，執行時間在最差情況下無法預期，不符合硬即時（Hard Real-Time）要求。
2. **CPU 快取效能（Cache Locality）**：
   * 靜態陣列記憶體實體連續（Contiguous），讀取時硬體自動將整條 64-byte Cache Line 載入快取，空間局部性（Spatial Locality）極佳，幾乎次次 Cache Hit。
   * LinkedList 節點散落在 Heap 各處，順著指標走訪時次次 Cache Miss，造成 CPU Pipeline 停擺等待記憶體搬運。
   * 且 LinkedList 指標維護負擔重（32-bit 佔 4B、64-bit 佔 8B 加上對齊），有效酬載率低。

### Q2: 韌體開發中嚴格禁止「遞迴 (Recursion)」
1. **硬體 Stack 限制與崩潰風險**：
   * 嵌入式系統 / MCU 的 Stack 空間極小（通常 FreeRTOS 每個 Task 只有幾百 Bytes 到 2 KB）。
   * 每次遞迴呼叫都會將回傳位址、暫存器內容、區域變數壓入 Stack Frame。若深度未受控，極易觸發 Stack Overflow，覆蓋相鄰全域變數或 Heap，引發 HardFault 毀滅性崩潰。
   * MISRA C:2012 Rule 17.2 明訂禁止遞迴。
2. **改寫為安全實作的方式**：
   * **迭代化（Iteration）**：將尾遞迴或單向遞迴以 `while`/`for` 迴圈搭配變數更新改寫，空間降為 $O(1)$。
   * **有限深度靜態陣列堆疊（Bounded Static Stack）**：若需樹狀回溯，手動宣告固定上限陣列 `static Node* stack[MAX_DEPTH]`，入棧前嚴格檢查 `top < MAX_DEPTH`，超限立即觸發 Failsafe 機制而非讓系統崩潰。

---

## 🚗 自走車專案實務科普：馬達編碼器正交解碼與閉迴路控制

### 1. 中斷風暴（Interrupt Storm）問題
* **情境**：
  光電/霍爾編碼器每轉輸出數百至數千個脈衝（PPR）。雙輪高速轉動時，A/B 兩相產生的中斷頻率可能飆至數十 kHz。
* **陷阱**：
  若在 MCU 上將 A 相與 B 相都接外部中斷（EXTI），CPU 會把 90% 以上的算力浪費在進出中斷常式（Context Switching），導致馬達主控迴圈、UART 通訊全面癱瘓。
* **面試應答亮點**：
  * **硬體正交解碼（Timer Encoder Interface Mode）**：使用 STM32/ESP32 等 MCU 的高階定時器硬體模式，硬體計數器自動根據 A/B 相相位差進行增減計數，**CPU 開銷為 0%**！
  * **軟體優化**：在沒有硬體解碼器的 8-bit MCU（如 Arduino），僅對 A 相單邊沿觸發中斷，中斷內直接讀取 B 相 GPIO 判定正反轉，將中斷頻率減半。

### 2. PID 採樣頻率與積分飽和防範（Anti-windup）
* **固定週期採樣**：必須使用定時器中斷（如 10ms 或 20ms）定期讀取 $\Delta \text{ticks} / \Delta t$ 換算角速度，切忌在非確定性的主迴圈中隨意計算。
* **積分飽和（Integral Windup）**：當車輪卡阻或目標速度過大時，誤差持續存在，PID 積分項 $I = \int e(t) dt$ 會暴增至極大值。障礙物排除後車輛會失控暴衝。
* **防護機制**：**Integrator Clamping（積分上下限限制）**，將積分值限制在 PWM 最大調變範圍內（如 `[-PWM_MAX, +PWM_MAX]`），避免飽和。
