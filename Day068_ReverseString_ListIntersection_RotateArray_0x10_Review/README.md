# Day 68: 字串反轉、鏈結串列相交點、旋轉陣列三次反轉 (Medium) 與 0x10 嵌入式 C 語言大滿貫

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的黃金訓練節奏。順利攻克了 LeetCode 3 大經典白板題（反轉字串、鏈結串列相交點、旋轉陣列三次反轉法 [Medium 中等題]），並通關了 0x10 嵌入式 C 語言三大核心命題（const volatile 組合應用、malloc 記憶體碎片化與 FreeRTOS heap_4 區塊縫合、ARM NVIC 搶佔/子優先級中斷嵌套與 RTOS 優先級反轉）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【反轉字串 (Reverse String - LeetCode #344)】
* **難度**：🟢 Easy
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day68_ReverseString_ListIntersection_RotateArray_0x10_Review/p1.c)
* **演算法**：雙指標對撞反轉法，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * `head = s`, `tail = s + sSize - 1`。
  * 在 `head < tail` 條件下三段式交換 `*head` 與 `*tail`，並同步移動指標 `head++`, `tail--`。

---

### 2. 題目 2【兩個單向鏈結串列的相交節點 (Intersection of Linked Lists - LeetCode #160)】
* **難度**：🟢 Easy / 🟡 Medium
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day68_ReverseString_ListIntersection_RotateArray_0x10_Review/p2.c)
* **演算法**：浪漫雙指標對換走訪法，時間 $O(N + M)$，空間 $O(1)$。
* **代數幾何原理**：
  * `pA` 走完 A 串列走 B 串列；`pB` 走完 B 串列走 A 串列。
  * 兩者走過的總步數皆為 $a + b + c$。若相交會在相交節點碰頭；若不相交會在 `NULL` 碰頭跳出 `while (pA != pB)`。

---

### 3. 題目 3【旋轉陣列 (Rotate Array - LeetCode #189)】
* **難度**：🟡 **Medium (中等題)**
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day68_ReverseString_ListIntersection_RotateArray_0x10_Review/p3.c)
* **演算法**：三次反轉法 (Three Reversals Trick)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 取模防呆 `k = k % numsSize`。
  * 1. 反轉整體 `[0 ~ N-1]` ➡️ 2. 反轉前 $k$ 個 `[0 ~ k-1]` ➡️ 3. 反轉剩餘 `[k ~ N-1]`。

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. `const volatile` 組合應用 (`const volatile int *p`)
* **`const`**：約束 C 軟體程式 code 為唯讀 (Read-Only)，防止軟體誤寫。
* **`volatile`**：告訴編譯器該記憶體/暫存器隨時會被**外部硬體/周邊**非同步修改（如 GPIO 輸入腳位、RTC 時間、UART IDR），禁止 CPU 快取，每次必須走實體匯流排讀取。

### 2. `malloc` 記憶體碎片化與解決方案
* **碎片化災難**：頻繁 `malloc/free` 導致 Heap 散落大量微小空閒塊。雖然總剩餘容量足夠，但缺乏大片連續記憶體，導致 `malloc` 回傳 `NULL` 系統崩潰 (OOM)。
* **業界 3 大解法**：
  1. **FreeRTOS `heap_4.c`**：在釋放時自動將相鄰空閒區塊進行縫合 (Coalescing)。
  2. **靜態預先配置 (Static Allocation)**：系統初始化完成後禁止動態分配。
  3. **固定區塊記憶體池 (Memory Pool)**：統一記憶體區塊大小，完全杜絕外部碎片。

### 3. NVIC 硬體中斷優先級 vs RTOS 優先級反轉
* **搶佔優先級 (Preemption Priority)**：高搶佔中斷可直接打斷低搶佔中斷（**中斷嵌套 Interrupt Nesting**）。
* **子優先級 (Subpriority)**：若搶佔優先級相同且同時觸發，子優先級較高者先執行（不能相互打斷）。
* **RTOS 優先級反轉 (Priority Inversion)**：低優先級 Task 持有 Mutex 遭中優先級 Task 搶佔 CPU，導致高優先級 Task 遭無窮等待。解法為 **優先級繼承 (Priority Inheritance)**。
