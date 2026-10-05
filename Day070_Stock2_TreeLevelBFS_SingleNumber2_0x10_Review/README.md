# Day 70: 買賣股票 II (Medium 貪心)、二元樹層序走訪 (BFS 佇列)、只出現一次的數字 II (Medium 32-bit 位元統計) 與 0x10 嵌入式快取一致性

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的高效訓練節奏。順利攻克了 LeetCode 3 大經典白板題（買賣股票的最佳時機 II [Medium 貪心演算法]、二元樹層序走訪 [BFS 佇列演算法]、只出現一次的數字 II [Medium 32 位元統計直式加法]），並 100% 全對通關了 0x10 嵌入式 C 語言三大核心命題（volatile 指標 3 種宣告組合與暫存器/雙緩衝應用、ARM Cortex-M 雙堆疊指標 MSP 與 PSP 任務隔離架構、Cache 一致性與 DMA 傳輸 Clean/Invalidate 災難防範）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【買賣股票的最佳時機 II (Best Time to Buy and Sell Stock II - LeetCode #122)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day70_Stock2_TreeLevelBFS_SingleNumber2_0x10_Review/p1.c)
* **演算法**：貪心演算法 (Greedy Algorithm)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 將長途波段拆解為每日短途波動（數學相消原理：$(3-1)+(5-3)+(8-5) = 8-1$）。
  * 只要 `prices[i+1] > prices[i]`，就將正差額累加進總利潤；下跌時不操作（避開虧損）。

---

### 2. 題目 2【二元樹的層序走訪 / 廣度優先搜尋 (Binary Tree Level Order Traversal - LeetCode #102)】
* **難度**：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day70_Stock2_TreeLevelBFS_SingleNumber2_0x10_Review/p2.c)
* **演算法**：佇列廣度優先搜尋 (Queue-based BFS)，時間 $O(N)$，空間 $O(N)$。
* **核心邏輯**：
  * 利用 Queue 先進先出 (FIFO) 特性：爸爸出隊 (`head++`) 時將左右小孩推入隊尾 (`tail++`)。
  * 純 C 語言中以指標陣列 `struct TreeNode* queue[]` 配合 `head` 與 `tail` 下標極簡模擬 Queue。

---

### 3. 題目 3【只出現一次的數字 II (Single Number II - LeetCode #137)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠高頻位元神題)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day70_Stock2_TreeLevelBFS_SingleNumber2_0x10_Review/p3.c)
* **演算法**：32-Bit 位元統計法 (Bit-Counting Column Arithmetic)，時間 $O(32N) = O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 外層迴圈遍歷 32 個 bit (`i` 從 0 走到 31)；內層迴圈走訪陣列所有數字統計該 bit 1 的總數 `sum`。
  * 出現 3 次的數字在任何 bit 上的 1 總數必為 3 的倍數；若 `sum % 3 != 0`，代表目標數字在第 `i` bit 必為 1。
  * 透過 `result |= (1U << i)` 還原出獨一無二的數字。

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. `volatile` 指標的三大宣告組合解讀
* `volatile int *p;`：指向的整數數值 `*p` 為 volatile（如周邊硬體暫存器 `GPIOA->IDR`）。
* `int * volatile p;`：指標變數 `p` 本身儲存的位址為 volatile（如 ISR 中斷切換雙緩衝區 Ping-Pong Buffer）。
* `volatile int * volatile p;`：指向的數值與指標位址兩者均為 volatile。

### 2. ARM Cortex-M 雙堆疊指標 MSP 與 PSP 架構
* **PSP (Process Stack Pointer)**：專供使用者的 Task 應用程式使用。
* **MSP (Main Stack Pointer)**：專供 OS Kernel 核心與硬體 ISR 中斷服務常式共用。
* **隔離好處**：
  1. Task 爆棧 (Stack Overflow) 僅損壞該 Task 的 PSP，不影響 OS 核心與中斷，防止整機死當。
  2. 所有中斷共用單一 MSP 堆疊空間，省去每個 Task 預留中斷空間的開銷，大幅節省 RAM。

### 3. 快取一致性 (Cache Coherency) 與 DMA 傳輸災難
* **CPU ➔ DMA 發送 (TX)**：CPU 寫入資料暫存於 D-Cache (Write-Back) 尚未同步至 RAM；發動 DMA 前必須呼叫 **`SCB_CleanDCache()`** 強制將快取資料 Flush 回實體 RAM，否則 DMA 傳出舊資料。
* **DMA ➔ CPU 接收 (RX)**：DMA 將硬體新資料直接寫入 RAM；CPU 讀取前必須呼叫 **`SCB_InvalidateDCache()`** 將快取標記為失效，強迫 CPU 重新自 RAM 抓取最新資料。
