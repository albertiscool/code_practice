# Day 65: 雙指標陣列壓縮、Kadane 最大子陣列、32-bit 位元反轉與 0x10 嵌入式 C 語言大滿貫

今天延續「一天 3 題手寫 Code + 3 題高頻面試觀念抽考」的黃金訓練節奏。順利攻克了 LeetCode 3 大經典白板題（刪除排序陣列重複項、Kadane 最大連續子陣列和、32 位元整數 Bits 反轉），並全對通關了 0x10 嵌入式 C 語言三大核心命題（中斷 ISR 禁忌、const 四大天王、static 三大作用域與記憶體段落）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【刪除排序陣列中的重複項 (Remove Duplicates - LeetCode #26)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day65_RemoveDuplicates_Kadane_ReverseBits_0x10_Review/p1.c)
* **演算法**：快慢雙指標原地壓縮法，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * `slow` 指向已排好的不重複元素尾端，`fast` 掃描陣列。
  * 只要發現 `nums[fast] != nums[slow]`，執行 `nums[++slow] = nums[fast]`！
  * **回傳值陷阱**：`slow` 是陣列下標 (0-indexed)，不重複元素總個數必須回傳 **`slow + 1`**！

---

### 2. 題目 2【最大子陣列和 (Maximum Subarray / Kadane 演算法 - LeetCode #53)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day65_RemoveDuplicates_Kadane_ReverseBits_0x10_Review/p2.c)
* **演算法**：Kadane 貪心/動態規劃單次遍歷，$O(N)$ 時間, $O(1)$ 空間。
* **丟棄負包袱哲學**：
  * 維護 `current_sum` 與 `max_sum`（初始值設為 `nums[0]` 支援全負數陣列）。
  * 每次將 `nums[i]` 加進 `current_sum`，若打破紀錄則更新 `max_sum`。
  * **關鍵判斷**：一旦 `current_sum < 0`，代表累積和為負數（只會拖累後續數字），立刻果斷丟棄歸零（`current_sum = 0`）！

---

### 3. 題目 3【反轉 32 位元整數的 Bits (Reverse Bits - LeetCode #190)】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day65_RemoveDuplicates_Kadane_ReverseBits_0x10_Review/p3.c)
* **演算法**：32 次位元移位暫存器法 (Shift Register)，時間 $O(1)$ (固定 32 步)，空間 $O(1)$。
* **核心位元操作**：
  * `result = (result << 1) | (n & 1);`：每次將結果左移騰出最低位，塞入 `n` 的最後一位。
  * `n >>= 1;`：`n` 右移丟棄處理完的位元。
  * 必須宣告為 **`uint32_t`**，避免有號整數最高位 31 bit 溢位觸發 Undefined Behavior。

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. 中斷服務常式 (ISR) 3 大黃金禁忌
1. **無參數且無回傳值 (void)**：ISR 是由硬體事件（電位變化、Timer 溢位）非同步觸發，而非軟體呼叫，沒有軟體 Caller 傳參或接回傳值。
2. **嚴禁 `printf()` 與 `malloc()` / `free()`**：
   * **不可重入與死鎖 (Deadlock)**：`printf`/`malloc` 內部具備 Mutex 鎖，若主程式持有鎖時遭 ISR 搶鎖，ISR 無法進入 Block/Sleep 狀態（無 TCB），將引發全系統死鎖！
   * **執行時間過長**：破壞即時性 (Real-Time)，卡死其他高優先級中斷。
3. **共享變數注意事項**：必須宣告為 `volatile` 防快取，若大於 CPU 指令寬度需進入臨界區 (關中斷) 保護防資料撕裂。

### 2. `const` 關鍵字四大天王宣告解讀
* `const int a;` ➡️ 變數 `a` 的數值為常數，不可修改。
* `const int *p;` ➡️ 指向常數的指標，`*p` 的數值不可修改，`p` 的指向位址可變。
* `int * const p;` ➡️ 常數指標，`p` 指向的記憶體位址鎖死不可變，`*p` 的數值可修改。
* `const int * const p;` ➡️ 指向常數的常數指標，位址與數值皆鎖死不可修改。

### 3. `static` 關鍵字 3 大用法與記憶體架構
1. **修飾區域變數**：由 Stack (堆疊) 改存放於 **Data / BSS 段**，生命週期延長至整個程式運行期，函式結束後數值不消滅。
2. **修飾全域變數**：限制作用域僅限於當前 `.c` 檔案（內部連結 Internal Linkage），其他檔案無法透過 `extern` 存取，防止全域命名污染。
3. **修飾函式**：設定為模組內部私有函式 (Private Function)，增強封裝性，避免與其他模組產生符號衝突 (Symbol Collision)。
