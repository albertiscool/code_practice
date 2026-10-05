# Day 75: 字母異位詞、兩數之和 II (Medium 對撞雙指標)、指定區間反轉鏈結串列 (Medium 頭插法) 與 0x10 MMIO/Tail-Chaining 深度實戰

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的高效訓練節奏。順利攻克了 LeetCode 3 大經典白板題（有效的字母異位詞 [頻率統計陣列]、兩數之和 II - 輸入有序陣列 [Medium 對撞雙指標]、反轉鏈結串列 II [Medium 區間反轉頭插法]），並 100% 全對通關了 0x10 嵌入式 C 語言與硬體微架構核心命題（MMIO 記憶體映射 vs PMIO 埠映射、ARM NVIC 中斷嵌套與 6 週期 Tail-Chaining 尾鏈接優化、硬體 PWM vs 軟體延時 SysTick Jitter 抖動防範）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【有效的字母異位詞 (Valid Anagram - LeetCode #242)】
* **難度**：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day75_ValidAnagram_TwoSum2_ReverseList2_0x10_Review/p1.c)
* **演算法**：頻率統計陣列法 (Frequency Array)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 先檢查 `strlen(s) != strlen(t)` 進行快速剪枝。
  * 開一個長度 26 的整數陣列 `count[26]`，單一迴圈走訪執行 `count[s[i] - 'a']++` 與 `count[t[i] - 'a']--`。
  * 最後檢查 `count` 陣列是否全為 0。

---

### 2. 題目 2【兩數之和 II - 輸入有序陣列 (Two Sum II - LeetCode #167)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (有序對撞雙指標必考)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day75_ValidAnagram_TwoSum2_ReverseList2_0x10_Review/p2.c)
* **演算法**：有序陣列對撞雙指標 (Two-Pointer Sorted Convergence)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 利用陣列已升序排序特性，首尾雙指標 `left = 0`, `right = n - 1` 向中間收縮。
  * 若 `sum > target` 則 `right--`；若 `sum < target` 則 `left++`。
  * 命中時回傳 1-indexed 下標 `[left + 1, right + 1]` 並 `break` 跳出迴圈。

---

### 3. 題目 3【反轉鏈結串列 II - 指定區間反轉 (Reverse Linked List II - LeetCode #92)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (科技廠高頻指標神題)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day75_ValidAnagram_TwoSum2_ReverseList2_0x10_Review/p3.c)
* **演算法**：Dummy Head + 局部頭插法 (Head-Insertion In-Place Reverse)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 建立 `dummy` 假節點（`dummy.next = head`），指針 `pre` 走 `left - 1` 步停在反轉區間的前驅節點。
  * `curr = pre->next` 指向反轉區間的第一個節點（反轉後會成為該區間的尾巴）。
  * 進行 `(right - left)` 次頭插操作，將後續節點一顆一顆拔到 `pre` 正後方！

---

## 🎨 題目 3 頭插法 (Head-Insertion) 逐步動態 ASCII 流程圖

以範例 `[1 -> 2 -> 3 -> 4 -> 5]`，`left = 2, right = 4`（目標：`1 -> 4 -> 3 -> 2 -> 5`）為例：

### 🎬 初始狀態：
```text
 dummy ──► [ 1 ] ──► [ 2 ] ──► [ 3 ] ──► [ 4 ] ──► [ 5 ] ──► NULL
            ▲         ▲
           pre       curr
```

---

### 🎬 第 1 回合 ($i = 0$：把節點 `[3]` 拔起來插到 `pre` 後面)：
1. `next_node = curr->next;` ➔ 鎖定 `[3]`
2. `curr->next = next_node->next;` ➔ `[2]` 跳過 `[3]` 連接 `[4]`
3. `next_node->next = pre->next;` ➔ `[3]` 連接原本的第一名 `[2]`
4. `pre->next = next_node;` ➔ `[1]` 連接插進來的 `[3]`

```text
 dummy ──► [ 1 ] ──► [ 3 ] ──► [ 2 ] ──► [ 4 ] ──► [ 5 ] ──► NULL
            ▲                   ▲
           pre                 curr
```

---

### 🎬 第 2 回合 ($i = 1$：把節點 `[4]` 拔起來插到 `pre` 後面)：
1. `next_node = curr->next;` ➔ 鎖定 `[4]`
2. `curr->next = next_node->next;` ➔ `[2]` 跳過 `[4]` 連接 `[5]`
3. `next_node->next = pre->next;` ➔ `[4]` 連接原本的第一名 `[3]`
4. `pre->next = next_node;` ➔ `[1]` 連接插進來的 `[4]`

```text
 dummy ──► [ 1 ] ──► [ 4 ] ──► [ 3 ] ──► [ 2 ] ──► [ 5 ] ──► NULL
            ▲                               ▲
           pre                             curr
```
* **結果**：回傳 `dummy.next` ➔ `1 -> 4 -> 3 -> 2 -> 5`！

---

## 📚 今日 0x10 & 硬體微架構面試精華 (Embedded C & MCU Architecture)

### 1. MMIO (記憶體映射 I/O) vs PMIO (埠映射 I/O)
* **MMIO (ARM / RISC-V)**：周邊硬體暫存器與 RAM/Flash 共用統一的 32-bit 位址空間。C 語言可以直接透過指標解引用（如 `*(volatile uint32_t*)0x40020000 = 0x01;`）搭配標準記憶體指令（`LDR`/`STR`）操控周邊，無需特殊 CPU 指令。
* **PMIO (x86)**：周邊具備獨立的 I/O Port 位址空間，必須使用專屬組合語言指令（`IN`/`OUT`）存取。

### 2. ARM NVIC 中斷嵌套與 Tail-Chaining 尾鏈接
* **中斷嵌套 (Nesting / Preemption)**：高搶占優先級中斷發動時，硬體 NVIC 自動將當前暫存器壓棧保存，立即搶占轉去執行高優先級 ISR。
* **尾鏈接 (Tail-Chaining)**：當 ISR A 結束時若有 ISR B 待處理，NVIC 跳過傳統「12 週期出棧 + 12 週期入棧」的無謂開銷，**直接保留棧中暫存器，在 6 個時脈週期內切換向量跳入 ISR B**，省下 75% 中斷切換延遲！

### 3. 硬體 PWM vs 軟體延時 SysTick Jitter
* **軟體 PWM**：受 RTOS 任務排程、SysTick 滴答時鐘與中斷搶占影響，產生嚴重的 **Jitter (時鐘抖動延遲)**，導致馬達波形頻率與占空比隨機失真，造成扭矩不均、震動與卡死。
* **硬體 PWM**：由晶片內部硬體 Timer 獨立計數器與比較器自主運行，達到 CPU 0% 負擔與零 Jitter 極致精準度。
