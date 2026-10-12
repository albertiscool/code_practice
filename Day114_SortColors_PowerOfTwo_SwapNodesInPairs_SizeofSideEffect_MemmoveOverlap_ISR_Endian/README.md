# Day 114 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-12
- **訓練主軸**: 排序與三向分割 (3-Way Partitioning)、位元遮罩與記憶體對齊 (Power of Two)、鏈結串列指標精密重組 (Swap in Pairs)、編譯期不求值陷阱、記憶體重疊拷貝、ISR 開發禁忌與大小端模式。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #75 - Sort Colors (荷蘭國旗 3-Way Partition)
- **核心思維**: 三向分割 (Dijkstra's Dutch National Flag)。使用 `left`, `curr`, `right` 三個指標在原地將陣列分類為 0、1、2。此演算法為 Quick Sort 面對大量重複鍵值時防止時間複雜度退化為 $O(N^2)$ 的核心基石。
- **邊界盲點與除錯**: 迴圈終止條件必須為 `while (curr <= right)` 而非 `while (curr < right)`，因為未知區間 $[curr, right]$ 在 `curr == right` 時仍包含一個未檢查的元素。
- **LeetCode 成績**:
  - **狀態**: Accepted (90/90 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 10.5 MB
  - **Submission ID**: `2170195503`

### P2: LeetCode #231 - Power of Two (2 的冪次方判定)
- **核心思維**: 暫存器遮罩與位元清零操作。判斷 `((n > 0) && ((n & (n - 1)) == 0))`。
- **底層考點與防禦**:
  - `(n > 0)` 透過短路求值 (Short-circuit Evaluation) 排除 0、負數，並防範 `n = INT_MIN (-2147483648)` 觸發 signed integer underflow 未定義行為。
  - 運算子優先級：關係運算子 `==` 優先級高於位元運算子 `&`，必須在 `(n & (n - 1))` 外加上完整括號。
  - 韌體價值：用於 SSD Page/Block 對齊檢查、無鎖環形佇列 (Lock-free Ring Buffer) 容量校驗以使用 `& (size - 1)` 替代除法取餘數。
- **LeetCode 成績**:
  - **狀態**: Accepted (1110/1110 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 8.5 MB
  - **Submission ID**: `2170199365`

### P3: LeetCode #24 - Swap Nodes in Pairs (兩兩交換鏈結串列節點)
- **核心思維**: 虛擬頭節點 (Dummy Node Pattern) 與指標三步縫合重接。
- **指標重接口訣**:
  1. `first->next = second->next;` (指向下一組)
  2. `second->next = first;` (反咬完成相鄰互換)
  3. `prev->next = second;` (前驅節點接上新排頭)
  4. `prev = first;` (前進到下一組的前驅點)
- **邊界盲點與除錯**: 終止條件必須為 `while (prev->next != NULL && prev->next->next != NULL)`，確保前方至少有一對節點可供交換，且短路求值避免 NULL 解引用 HardFault。
- **LeetCode 成績**:
  - **狀態**: Accepted (55/55 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 9.6 MB
  - **Submission ID**: `2170219452`

---

## Part 2: 面試經典致命陷阱題

### Trap 1: `sizeof` 運算元不求值副作用 (Side Effect)
- **程式碼**:
  ```c
  int a = 5;
  int b = sizeof(a++); // 32-bit MCU
  ```
- **解答**: `a = 5`, `b = 4`。
- **原理剖析**: `sizeof` 是編譯期運算子 (Compile-time Operator)，其運算元為「不求值運算元 (Unevaluated Operand)」。編譯期推導其型別為 `int` 後直接替換為常數 `4`，不產生任何機器碼指令，因此 `a++` 絕不執行，無副作用。

### Trap 2: 記憶體重疊拷貝指標判定 (`memcpy` vs `memmove`)
- **問題**: 什麼情況下向前複製會踩踏？由後向前複製的判斷式？
- **解答**:
  - 當目的地位址落於來源緩衝區內部時 (`dest > src && dest < src + n`)，向前複製會將尚未讀取的來源資料提前覆蓋，造成資料毀損。
  - **判定條件式**: `if (dest > src && dest < src + n)` (在 C 語言中指標比較前須轉型為 `char*` 或 `uint8_t*`)，此時切換為 Backward Copy (`n-1` 到 `0`)。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: 中斷服務常式 (ISR) 的四大禁忌與共享變數規範
1. **四大禁忌**:
   - **嚴禁不可重入函式 (Non-reentrant Functions)**：如 `printf()`, `malloc()`, `free()`，避免破壞靜態緩衝區或 Heap 破碎。
   - **嚴禁阻塞與休眠 (Blocking & Sleeping)**：不可使用 Mutex 睡眠鎖、`sleep()` 或大迴圈 `delay_ms()`，因為 ISR 無法被排程器換出，會導致整個系統鎖死。
   - **無回傳值、無參數**：硬體向量表直接觸發跳轉，原型一律為 `void ISR(void)`。
   - **嚴禁耗時運算 (KISS 原則)**：只做緊急暫存器讀取與清除旗標，重度運算推至主迴圈或 Top/Bottom Half。
2. **共享變數修飾**: 必須宣告為 **`volatile`**，避免編譯器快取至 CPU 內部暫存器，強制每次存取實體 RAM。

### Q2: 大小端模式 (Little-Endian vs Big-Endian) 與實作判定
1. **定義**:
   - **Little-Endian**: 低位元組 (LSB) 存放在低記憶體位址 (Low Address)。
   - **Big-Endian**: 高位元組 (MSB) 存放在低記憶體位址 (Low Address)。網路通訊協定預設為 Big-Endian。
2. **C 語言判定程式碼**:
   ```c
   uint16_t a = 0x0001;
   if (*(uint8_t*)&a == 0x01) {
       // Little-Endian
   } else {
       // Big-Endian
   }
   ```
   或使用 Union 判定：
   ```c
   union { uint16_t s; uint8_t c; } u = { .s = 0x0001 };
   if (u.c == 0x01) { /* Little-Endian */ }
   ```
3. **架構**: ARM Cortex-M 與 x86/x64 PC 預設皆為 **Little-Endian**。

---

## Part 4: 自走車專案實務科普 (第 114 講)
### 馬達 H 橋急停突波、反電動勢 (Back-EMF) 與 MCU 重置防禦
- **物理成因**:
  - 馬達線圈電感 $L$ 在瞬間急停或正反轉換向時產生反電動勢突波 $V = -L \frac{di}{dt}$，瞬間衝出高達數十伏特的逆向高壓。
  - 大電流啟動拉垮電池電壓觸發 MCU 的 Brown-out Reset (BOR)，或逆向突波干擾地線觸發 HardFault。
- **硬體解決方案**:
  - H 橋上下臂並聯快速恢復肖特基續流二極體 (Flyback Diode, 1N5819) 提供釋放迴路。
  - 動力電與控制電獨立供電，控制訊號經光耦隔離，電源入口並聯 470µF~1000µF 大電容濾波。
- **韌體解決方案**:
  - 軟體平滑控制 (Slew Rate Control / Soft Stop)：切換轉向時嚴禁直切，須經 PWM 漸降 $\to$ 剎車死區 (Dead-time, 50ms) $\to$ 反向漸增 PWM，抑制 $\frac{di}{dt}$。
