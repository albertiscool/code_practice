# Day 63: 科技大廠頂級高頻白板題馬拉松 (LeetCode Top Interview 7 大經典神題全通關)

今天是針對 Garmin、聯發科、群聯、瑞昱、台達電等科技大廠「高頻白板手寫題」的極速衝刺。我們在單日內連續攻克了 **7 道 LeetCode 官方經典面試神題**，涵蓋二分搜尋防溢位、快慢指標、BST 迭代走訪、位元運算神技 (Power of Two / XOR 消去)、雙指標陣列原地壓縮、以及股票單次遍歷狀態追蹤！

---

## 🏆 今日 7 大經典白板題總覽與解析

### 1. 題目 1【二分搜尋法 (Binary Search - LeetCode #704)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p1.c)
* **核心技巧**：$O(\log N)$ 時間複雜度。
* **面試防溢位陷阱**：
  * ❌ `(left + right) / 2` 在大陣列中會發生 32-bit `INT_MAX` 溢位變成負數！
  * ⭕ **`left + (right - left) / 2`** 保證絕不溢位！
* **邊界條件**：`while (left <= right)`，縮小邊界為 `mid - 1` 與 `mid + 1`。

---

### 2. 題目 2【尋找鏈結串列中間節點 (Middle of the Linked List - LeetCode #876)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p2.c)
* **核心技巧**：快慢指標 (Fast & Slow Pointers)。
  * `slow` 每次走 1 步 (`slow = slow->next`)。
  * `fast` 每次走 2 步 (`fast = fast->next->next`)。
  * 當 `fast` 抵達尾端時，`slow` 恰好停在正中間節點（奇數長度命中正中，偶數長度命中第二個中間節點）。
* **標準防呆條件**：`while (fast != NULL && fast->next != NULL)`。

---

### 3. 題目 3【二元搜尋樹搜尋節點 (Search in a BST - LeetCode #700)】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p3.c)
* **核心技巧**：BST「左小右大」二分走訪。
* **嵌入式頂級寫法 (Iterative 迭代法)**：
  * 使用 `while (curr != NULL)` 迴圈走訪，達到 **$O(1)$ 空間複雜度（0 CPU Stack 開銷）**，避免遞迴 Stack Overflow！

---

### 4. 題目 4【2 的冪次方判定 (Power of Two - LeetCode #231)】
* **原始碼檔案**：[`p4.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p4.c)
* **核心技巧**：2 的冪次方在二進位中只有 1 個 bit 是 1（例如 `16 = 00010000`）。
* **位元運算公式**：`n & (n - 1) == 0`（`16 & 15 == 0`）。
* **C 語言兩大關鍵陷阱**：
  1. **運算子優先級**：`==` 優先級大於 `&`，因此必須括號 **`((n & (n - 1)) == 0)`**！
  2. **邊界防呆**：必須排除 `0` 與負數，加上 **`n > 0`**。
* **最終 1 行滿分代碼**：`return (n > 0) && ((n & (n - 1)) == 0);`

---

### 5. 題目 5【只出現一次的數字 (Single Number - LeetCode #136)】
* **原始碼檔案**：[`p5.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p5.c)
* **核心技巧**：XOR 互斥或（`^`）的三大神奇物理性質：
  1. $x \oplus x = 0$（成對數字相遇自動抵消）
  2. $x \oplus 0 = x$（保留原值）
  3. 交換律與結合律：陣列中所有數字全部連做 XOR，所有出現兩次的數字自動歸零，最終留下來的即為唯一落單值！
* **陣列走訪注意**：陣列不是鏈結串列（結尾非 NULL），必須依據 `numsSize` 走訪，且取數值用 `nums[i]` 或 `*nums`。

---

### 6. 題目 6【移動零到末尾 (Move Zeroes - LeetCode #283)】
* **原始碼檔案**：[`p6.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p6.c)
* **核心技巧**：雙指標原地壓縮 (In-Place Array Compaction)。
  * `slow` 記錄下一個非零放置位置，`fast` 掃描陣列。
  * `if (nums[fast] != 0) nums[slow++] = nums[fast];`
  * 遍歷結束後，`while (slow < numsSize) nums[slow++] = 0;` 尾端全數補零。
  * 達成 **時間 $O(N)$、空間 $O(1)$** 完美原地搬移！

---

### 7. 題目 7【買賣股票最佳時機 (Best Time to Buy and Sell Stock - LeetCode #121)】
* **原始碼檔案**：[`p7.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day63_Top_Interview_Whiteboard_Coding_Marathon/p7.c)
* **核心技巧**：單次遍歷狀態追蹤（商人思維）。
  * 隨時記錄 `min_price`（迄今歷史最低進貨價）與 `max_profit`（迄今最高獲利）。
* **為什麼使用 `else if` 互斥邏輯？**
  * 若今天創下歷史新低價 (`prices[i] < min_price`)，當天潛在賣出利潤為 $0$，在數學上絕對不可能大於 `max_profit`！因此「買進日」與「賣出日」絕對互斥，使用 `else if` 既嚴密又節省運算！
