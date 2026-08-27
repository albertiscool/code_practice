# Day 71: 單字原地反轉、刪除鏈結串列倒數第 N 節點 (Medium)、驗證二元搜尋樹 (Medium) 與小車專題電機控制深度實戰

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念與專案實戰」的高效訓練節奏。順利攻克了 LeetCode 3 大經典白板題（單字原地反轉 [兩次反轉法]、刪除鏈結串列倒數第 N 個節點 [Medium 雙指標 + Dummy Head]、驗證二元搜尋樹 [Medium 區間界限遞迴法]），並 100% 全對通關了 0x10 嵌入式 C 語言與自駕小車專題核心命題（`memcpy` vs `memmove` 記憶體重疊保護、超音波 $340 \text{ m/s} \to 29.1 \mu\text{s/cm}$ 物理推導與 Timer 輸入捕獲、L298N H 橋慣性滑行 vs 反電動勢主動動態制動煞車）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【反轉字串中的單字 (Reverse Words in a String - LeetCode #151 核心版)】
* **難度**：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day71_ReverseWords_RemoveNthNode_ValidateBST_0x10_Review/p1.c)
* **演算法**：兩次原地反轉法 (Two-Pass In-Place Reversal)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  1. 步驟 1：先呼叫 `reverse(s, 0, len - 1)` 將整條字串從頭到尾整體反轉（`"the sky is blue"` ➔ `"eulb si yks eht"`）。
  2. 步驟 2：逐一走訪每個單字，遇到空格 `' '` 或結尾 `'\0'` 時，將各單字內部再次反轉（`"eulb"` ➔ `"blue"`），得到 `"blue is sky the"`。

---

### 2. 題目 2【刪除鏈結串列的倒數第 N 個節點 (Remove Nth Node From End of List - LeetCode #19)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day71_ReverseWords_RemoveNthNode_ValidateBST_0x10_Review/p2.c)
* **演算法**：快慢雙指標 + 哨兵假節點 (Fast & Slow Pointers with Dummy Head)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  1. 建立 `dummy` 假節點接在 `head` 前面，統一處理刪除頭節點的邊界情況。
  2. 讓 `fast` 先往前走 $n + 1$ 步與 `slow` 拉開間距。
  3. `fast` 與 `slow` 同步每次走 1 步，直到 `fast` 抵達結尾 `NULL`，此時 `slow` 剛好停在目標節點的前驅節點。
  4. 執行 `slow->next = slow->next->next` 刪除目標節點，回傳 `dummy.next`。

---

### 3. 題目 3【驗證二元搜尋樹 (Validate Binary Search Tree - LeetCode #98)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day71_ReverseWords_RemoveNthNode_ValidateBST_0x10_Review/p3.c)
* **演算法**：區間界限遞迴法 (Range Boundary Recursion)，時間 $O(N)$，空間 $O(H)$。
* **核心邏輯**：
  1. 避免「只看父子關係」導致深層孫節點違反祖先限制的經典 Bug。
  2. 將祖先的所有限制打包為開區間 `(min_val, max_val)` 往下傳遞。
  3. 走訪左子樹時上限更新為 `node->val`；走訪右子樹時下限更新為 `node->val`。
  4. 初始呼叫使用 `LLONG_MIN` 與 `LLONG_MAX` 避開 32-bit INT 溢位陷阱。

---

## 📚 今日 0x10 & 專題面試精華 (Embedded C & Project Deep-Dive)

### 1. `memcpy` vs `memmove` 記憶體重疊保護
* 當 `dest > src` 發生記憶體重疊時，`memcpy` 從前面拷貝會踩壞未讀取的來源資料。
* `memmove` 會自動根據位址比較，在 `dest > src` 時改由「後面往前面 (從尾到頭)」拷貝，確保 100% 零資料踩踏。

### 2. 超音波測距物理常數推導與 Timer Input Capture
* **物理常數推導**：聲速 $V = 340 \text{ m/s} = 0.03434 \text{ cm/}\mu\text{s}$。走 $1 \text{ cm}$ 耗時 $\frac{1}{0.03434} \approx 29.15 \mu\text{s/cm}$。因聲波來回兩趟，公式為 $\frac{\text{duration}}{2} \div 29.1$。
* **Timer 輸入捕獲優化**：相較於 `pulseIn()` 阻塞 CPU 達 30ms，改用硬體 Timer Input Capture 中斷可達到 CPU 0% 負擔與微秒級精準度。

### 3. L298N H 橋馬達控制：滑行停止 vs 主動動態煞車
* **滑行停止 (Coasting)**：`PWM = 0` 使 H 橋 4 顆電晶體開路斷開，車輛純靠慣性慢慢滑行。
* **主動動態煞車 (Dynamic Active Braking)**：設定 `IN1 = LOW, IN2 = LOW`（或同為 HIGH）且 `PWM = 255`。馬達旋轉時化身為發電機，兩端短路產生的閉合短路電流根據冷次定律產生強大的反電動勢 (Back-EMF) 反向力矩，瞬間鎖死輪胎急煞停。
