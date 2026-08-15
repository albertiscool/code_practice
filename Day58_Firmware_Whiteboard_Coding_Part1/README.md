# Day 58: 韌體工程師白板題特訓 Part 1 (C 語言工具箱、二進位 1 計數極速法、單向鏈結串列 3 指標原地反轉)

今天是韌體工程師白板手寫程式碼 (Whiteboard Coding) 特訓第一天。我們全面溫習了 5 大高頻 C 語言語法工具箱，隨後進行了白板題第 1 題與第 2 題的觀念拆解、實作、Code Review 與邊界陷阱檢討。

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
* **題目要求**：輸入 32-bit 無號整數 `n`，回傳其二進位中 `1` 的總個數。
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
    *時間複雜度 $O(k)$，數字中有幾個 1 迴圈就精準只跑幾次！*

---

### 題目 2【單向鏈結串列原地反轉 (Reverse Singly Linked List)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day58_Firmware_Whiteboard_Coding_Part1/p2.c)
* **題目要求**：輸入鏈結串列頭節點 `head`，在空間複雜度 $O(1)$ 下進行原地反轉，回傳全新頭節點。
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
* **陷阱檢討**：
  1. 若寫成 `next_node = head->next;`，因 `head` 恆定指向原頭節點，後續迴圈會錯鎖死在第 2 個節點；必須改為 `curr->next` 動態更新。
  2. 函式結尾務必包含 `return pre;` 回傳新標頭。
