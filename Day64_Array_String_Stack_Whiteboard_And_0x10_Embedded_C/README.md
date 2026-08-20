# Day 64: 陣列倒序三指標、字串對撞、純 C 堆疊 Stack 與 0x10 嵌入式 C 語言精髓特訓

今天聚焦於三大關鍵資料結構（陣列 In-Place 操作、字串雙指標對撞、純 C 堆疊 Stack 模擬）之 LeetCode 經典面試白板題，並結合全台科技大廠必考的 **「0x10 嵌入式 C 語言 16 題核心觀念」** 進行高強度實戰特訓。

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【合併兩個有序陣列 (Merge Sorted Array - LeetCode #88)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day64_Array_String_Stack_Whiteboard_And_0x10_Embedded_C/p1.c)
* **演算法**：倒序三指標法 (Reverse 3-Pointers)，$O(M + N)$ 時間, $O(1)$ 空間。
* **核心防覆蓋機制**：
  * 若從前往後排會覆蓋尚未比較的資料。
  * 改由最尾端空位 `p = m + n - 1` 開始填入兩陣列中較大的數字（`p1 = m - 1`, `p2 = n - 1`）。
  * 若 `nums2` 尚有剩餘，全數填入 `nums1` 前端；若 `nums1` 剩餘則本就在定位無須移動。

---

### 2. 題目 2【驗證迴文串 (Valid Palindrome - LeetCode #125)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day64_Array_String_Stack_Whiteboard_And_0x10_Embedded_C/p2.c)
* **演算法**：雙指標左右對撞法 (Two-Pointer Collision)，$O(N)$ 時間, $O(1)$ 空間。
* **C 語言指標防呆關鍵**：
  * 右指標初始化：`char *last = s + strlen(s) - 1;`
  * 內層過濾 `!isalnum(*p)` 必須加上 `first < last` 邊界防禦，避免純空格字串引發記憶體越界崩潰。
  * `tolower()` 屬於回傳型態，直接於 `if (tolower(*first) != tolower(*last))` 進行比對。

---

### 3. 題目 3【有效的括號 (Valid Parentheses - LeetCode #20)】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day64_Array_String_Stack_Whiteboard_And_0x10_Embedded_C/p3.c)
* **演算法**：純 C 陣列模擬 LIFO 堆疊 (Stack)，$O(N)$ 時間, $O(N)$ 空間。
* **神級對稱匹配技**：
  * 遇到左括號時，直接將「預期的右括號」壓入堆疊 (`stack[++top] = ...`)。
  * 遇到右括號時，只需檢查 `if (top < 0 || stack[top--] != *s) return false;`。
  * 結尾檢查 `return top == -1;` 確保堆疊全數清空。

---

## 📚 嵌入式 0x10 經典 16 題複習精華 (Embedded C 0x10 Core Concepts)

### 1. 指標宣告語法解讀 (Clockwise / Spiral Rule)
* `int *a[10];` ➡️ **指標陣列**：大小為 10 的陣列，每個元素都是 `int*` 指標。
* `int (*a)[10];` ➡️ **陣列指標**：一個指標，指向長度為 10 的整數陣列。
* `int (*a)(int);` ➡️ **函式指標**：一個指標，指向接收 `int` 並回傳 `int` 的函式。
* `int (*a[10])(int);` ➡️ **函式指標陣列**：大小為 10 的陣列，每個元素皆為函式指標（常用於中斷向量表與狀態機轉移表）。

### 2. 硬體暫存器與 `volatile` 關鍵字
* **語法**：`*((volatile uint32_t *)0x40001000) = 0x5A5A;`
* **為什麼必加 `volatile`？**
  1. **防止 CPU 暫存器快取**：避免輪詢旗標時被編譯器最佳化成 `while(R0 == 0)` 死機。
  2. **防止無效程式碼消除 (Dead Code Elimination)**：確保連續脈衝寫入（如 `*p = 0; *p = 1;`）不被編譯器刪除。
  3. **強制直達匯流排**：保證每次讀寫皆走實體 Bus 讀寫實體暫存器。

### 3. 巨集陷阱與副作用 (Macro Side Effects)
* **標準防禦寫法**：`#define MIN(A, B) (((A) < (B)) ? (A) : (B))`
* **副作用範例**：`MIN(*p++, b)` 會導致 `*p++` 被評估兩次，指標位址連續遞增兩次產生非預期 Bug。
