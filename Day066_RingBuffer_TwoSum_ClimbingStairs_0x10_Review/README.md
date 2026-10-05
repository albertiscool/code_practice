# Day 66: 環形緩衝區 Ring Buffer、兩數之和 Two Sum、爬樓梯滾動 DP 與 0x10 嵌入式 C 語言經典觀念大滿貫

今天延續「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的黃金訓練節奏。順利攻克了 LeetCode 3 大經典白板題（環形緩衝區 Ring Buffer、兩數之和 Two Sum、爬樓梯滾動 DP），並全對通關了 0x10 嵌入式 C 語言三大核心命題（結構體記憶體對齊算術、typedef vs define 差異、while(1) vs for(;;) 編譯器層面剖析）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【設計環形緩衝區 (Design Circular Queue / Ring Buffer - LeetCode #622)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day66_RingBuffer_TwoSum_ClimbingStairs_0x10_Review/p1.c)
* **背景**：嵌入式韌體（UART RX/TX 中斷、DMA 傳輸、Audio FIFO）最核心之 FIFO 資料結構。
* **實作細節**：
  * 兩階段動態記憶體配置 (結構體本體 `malloc` + 內部 `buffer` 陣列 `malloc`)。
  * `enQueue`：寫入 `buffer[rear]` 並將 `rear = (rear + 1) % capacity`，`count++`。
  * `deQueue`：將 `front = (front + 1) % capacity`，`count--`。
  * `Rear()`：防負數環形索引 `(rear - 1 + capacity) % capacity`。

---

### 2. 題目 2【兩數之和 (Two Sum - LeetCode #1)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day66_RingBuffer_TwoSum_ClimbingStairs_0x10_Review/p2.c)
* **演算法**：雙重迴圈枚舉法，$O(N^2)$ 時間, $O(1)$ 空間。
* **C 語言指標傳值機制**：
  * 未排序陣列中，雙指標跳躍法會錯過答案，白板題使用雙重 `for` 迴圈最安全且零 Bug。
  * 動態配置 `malloc(2 * sizeof(int))`，並透過 `*returnSize = 2` 將長度傳回給呼叫者。

---

### 3. 題目 3【爬樓梯 (Climbing Stairs - LeetCode #70)】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day66_RingBuffer_TwoSum_ClimbingStairs_0x10_Review/p3.c)
* **演算法**：滾動變數動態規劃 (Rolling DP)，$O(N)$ 時間, $O(1)$ 空間。
* **避免爆棧與 TLE 災難**：
  * 純遞迴 `f(n-1) + f(n-2)` 會引發 $O(2^N)$ 時間複雜度（TLE）與 Stack Overflow。
  * 改用 `first` 與 `second` 兩變數由下往上 `for` 迴圈滾動遞回，達到極速 $O(N)$ 時間與 $O(1)$ 空間。

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. 結構體記憶體對齊與 Padding 算術
* `struct A { char a; int b; short c; };` ➡️ **12 Bytes**
  * `a` (1B) + 3B padding ➡️ `b` (4B) ➡️ `c` (2B) + 2B tail padding = 12。
* `struct B { char a; short c; int b; };` ➡️ **8 Bytes**
  * `a` (1B) + 1B padding ➡️ `c` (2B) ➡️ `b` (4B) = 8。

### 2. `typedef` vs `#define` 型態宣告差異
* `typedef char* PCHAR_T; PCHAR_T p1, p2;` ➡️ `p1` 與 `p2` 皆為 `char*`。
* `#define PCHAR_D char* PCHAR_D p3, p4;` ➡️ 展開為 `char *p3, p4;`，`p3` 為 `char*`，`p4` 變成了普通 `char`！

### 3. 無窮迴圈 `while(1)` vs `for(;;)` 差異
* **歷史編譯器層面**：早期的 C 編譯器寫 `while(1)` 會被編譯成 `CMP R0, #0` 加判斷；而 `for(;;)` 因無初始化與判斷，直接編譯成單一無條件跳躍指令 `JMP`，執行速度更快。
* **現代編譯器層面**：`-O2` 最佳化下兩者生成的組合語言皆為 `JMP`；但規範（如 MISRA C）偏好 `for(;;)` 語意更明確且可避免編譯器發出「常數條件判斷警告 (constant condition warning)」。
