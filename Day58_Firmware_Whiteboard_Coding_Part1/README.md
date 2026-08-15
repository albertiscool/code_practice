# Day 58: 韌體工程師白板題特訓 Part 1 (C 語言工具箱、二進位 1 計數、單向鏈結串列 3 指標反轉、大小端檢測/Byte Swap 與指標陷阱)

今天是韌體工程師白板手寫程式碼 (Whiteboard Coding) 特訓。我們完成了 C 語言 5 大語法工具箱複習、二進位計數神題、單向鏈結串列 3 指標原地反轉、大小端檢查/Byte Swap，以及指標運算優先順序陷阱特訓。

---

## 🧰 C 語言韌體白板題：5 大必備語法工具箱

1. **位元操作 3 大神技 (Bitwise Tricks)**：
   * **Set / Clear / Toggle / Check**：`x |= (1 << n);`, `x &= ~(1 << n);`, `x ^= (1 << n);`, `(x >> n) & 1;`
   * **Brian Kernighan's Algorithm**：`x = x & (x - 1);` 瞬間消除最低位的 1（常用於計數與 2 的次方檢查）。
2. **雙指標技巧 (Two-Pointer Technique)**：
   * 常用於字串原地反轉、回文檢查與陣列 In-place 搬移。
3. **單向鏈結串列 3 指標反轉法 (Reverse Linked List)**：
   * 使用 `prev`, `curr`, `next_node` 3 個指標在不配置額外空間下原地翻轉。
4. **快慢指標技巧 (Fast & Slow Pointers)**：
   * 慢指標走 1 步 (`slow->next`)，快指標走 2 步 (`fast->next->next`)，常用於尋找鏈結串列中點與判斷是否有環 (Cycle)。
5. **記憶體重疊拷貝原理 (`my_memmove`)**：
   * 當 `dest < src` (目的地在左邊) ➡️ **由前往後複製 (`Head to Tail`)**。
   * 當 `dest > src` (目的地在右邊) ➡️ **由後往前複製 (`Tail to Head`)**，防止未讀取資料遭提前覆蓋踩爛。

---

## 📝 實戰白板題詳細解析與 Code Review

### 題目 1【計算二進位中 1 的個數 (Count Set Bits / Hamming Weight)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day58_Firmware_Whiteboard_Coding_Part1/p1.c)
* **檢討與解法比對**：
  * **基礎寫法**：使用 `while (n > 0)` 配合 `count += (n & 1); n >>= 1;` 逐位右移檢查。
  * **陷阱檢討**：若寫成 `int i = n; while(i > 0) { ... i--; }`，當 `n = 1000` 會不必要地跑 1000 次；若最高位為 1 (`n = 0x80000000`)，轉成有號數 `int i` 會溢位變成負數導致迴圈完全不執行。
  * **極速最佳化寫法 (100 分滿分)**：
    ```c
    int count_set_bits_fast(uint32_t n) {
        int count = 0;
        while (n != 0) {
            n = n & (n - 1); // 每次消除最低位的 1
            count++;
        }
        return count;
    }
    ```

---

### 題目 2【單向鏈結串列原地反轉 (Reverse Singly Linked List)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day58_Firmware_Whiteboard_Coding_Part1/p2.c)
* **`struct` 語法與邏輯觀念**：
  * **`->` 箭頭存取**：`curr->next` 順著指標存取成員。
  * **為什麼 `prev` 初始為 `NULL`**：反轉後原頭節點 (Node 1) 將成為最後的尾巴，其 `next` 必須指向 `NULL`；故第一回合 `curr->next = prev;` 能自動將原頭節點 `next` 設為 `NULL`。
* **核心 4 句咒語實作 (修正完成版)**：
  ```c
  struct ListNode* reverseList(struct ListNode* head) {
      struct ListNode *curr = head;
      struct ListNode *pre = NULL;
      
      while (curr != NULL) {
          struct ListNode *next_node = curr->next; // 1. 先暫存下一個節點位址 (防止斷鏈)
          curr->next = pre;                        // 2. 轉向：將當前節點 next 指向 pre
          pre = curr;                              // 3. pre 前進
          curr = next_node;                        // 4. curr 前進
      }
      return pre; // 迴圈結束後 pre 剛好停在全新頭節點
  }
  ```

---

### 題目 3【大小端 (Endianness) 檢查與 32 位元 Byte 反轉】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day58_Firmware_Whiteboard_Coding_Part1/p3.c)
* **任務 1（檢測小端 Little-Endian）**：
  ```c
  bool is_little_endian(void) {
      uint32_t x = 0x0001;
      uint8_t *y = (uint8_t *)&x; // 必須強制轉型為 (uint8_t*) 才能精確唯讀記憶體最低位址第 1 個 Byte!
      return (*y == 1);
  }
  ```
* **任務 2（32-bit Byte 反轉 0x12345678 ➡️ 0x78563412）**：
  ```c
  uint32_t swap_endian(uint32_t val) {
      return ((val & 0x000000FF) << 24) |
             ((val & 0x0000FF00) << 8)  |
             ((val & 0x00FF0000) >> 8)  |
             ((val & 0xFF000000) >> 24);
  }
  ```

---

## 🎯 指標與記憶體 4 大特訓總結

1. **指標步長 (Step Size)**：`p + 1` 位址移動 `1 * sizeof(型態)`。`(uint8_t*)&x` 能精確讀取單一 Byte。
2. **絕對記憶體/暫存器寫入**：`*( (volatile uint32_t *) 0x40001004 ) = 0xFF;`
3. **`*` 與 `++` 優先順序比較**：
   * `val = *p++` ➡️ 取舊值 `10`，指標 `p` 往前移 1 格。
   * `val = (*p)++` ➡️ 取舊值 `10`，記憶體內容 `a[0]` 變成 `11`，指標位置不變。
   * `val = *++p` ➡️ 指標 `p` 先往前移 1 格，取新值 `20`。
4. **`const` 指標黃金口訣**：
   * `const int *p` ➡️ `const` 在 `*` 左邊 ➡️ **內容唯讀**。
   * `int * const p` ➡️ `const` 在 `*` 右邊 ➡️ **位址唯讀**。
