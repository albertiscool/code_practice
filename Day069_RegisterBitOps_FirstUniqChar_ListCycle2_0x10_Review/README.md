# Day 69: 暫存器位元大四喜、第一個唯一字元、鏈結串列環入口點 (Medium) 與 0x10 嵌入式 C 語言大滿貫

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的黃金訓練節奏。順利攻克了 LeetCode 3 大經典白板題（暫存器位元操作大四喜、字串第一個唯一字元、鏈結串列環入口點 [Medium 中等題]），並 100% 全對通關了 0x10 嵌入式 C 語言三大核心命題（volatile 局限性與原子性/執行緒安全迷思、ARM NVIC 硬體自動壓棧機制與 Tail-Chaining、FreeRTOS 軟體 Timer 與 Daemon Task 阻塞禁忌）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【韌體暫存器位元操作大四喜 (Register Bit Manipulation)】
* **難度**：🟢 Easy / 韌體實戰
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day69_RegisterBitOps_FirstUniqChar_ListCycle2_0x10_Review/p1.c)
* **核心邏輯**：
  * Set Bit：`val |= (1U << n)`
  * Clear Bit：`val &= ~(1U << n)`
  * Toggle Bit：`val ^= (1U << n)`
  * Read Bit：`(val >> n) & 1U`
  * 專業加分點：使用 `1U` 無號整數避免第 31 bit 發生 signed int 溢位之未定義行為 (UB)。

---

### 2. 題目 2【字串中的第一個唯一字元 (First Unique Character in a String - LeetCode #387)】
* **難度**：🟢 Easy
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day69_RegisterBitOps_FirstUniqChar_ListCycle2_0x10_Review/p2.c)
* **演算法**：頻率統計陣列法 (Frequency Array)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 開長度為 26 的整數陣列 `count[26]`。
  * 第 1 遍走訪統計 `count[s[i] - 'a']++`。
  * 第 2 遍走訪找到第一個 `count[s[i] - 'a'] == 1` 的字元，立即回傳其索引。

---

### 3. 題目 3【鏈結串列環的入口點 (Linked List Cycle II - LeetCode #142)】
* **難度**：🟡 **Medium (中等題)**
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day69_RegisterBitOps_FirstUniqChar_ListCycle2_0x10_Review/p3.c)
* **演算法**：Floyd 龜兔賽跑兩階段演算法，時間 $O(N)$，空間 $O(1)$。
* **代數幾何推導 ($x = nC - y$)**：
  * 階段 1：`slow` 走 1 步，`fast` 走 2 步，兩者相遇代表有環。
  * 階段 2：重置 `slow = head`，`fast` 留於相遇點，兩者改為**每次走 1 步**。
  * 由代數推導 $x = nC - y$ 可知，兩人在 $x$ 步後必定於「環入口點」完美碰頭！

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. `volatile` 的局限性與執行緒安全迷思
* `volatile` 只負責「禁止編譯器快取與指令刪除優化」，強制走實體記憶體讀寫。
* 絕不保證**原子性 (Atomicity)**（如 `count++` 在組合語言中為 READ-MODIFY-WRITE 三步驟）、不保證**記憶體屏障 (Memory Barrier)**、不保證**執行緒安全 (Thread Safety)**。

### 2. ARM Cortex-M NVIC 硬體自動壓棧 (Auto Stacking)
* 硬體 NVIC 於中斷發動時，會自動由硬體電路在 12 個時脈週期內將 `R0-R3, R12, LR, PC, xPSR` 8 個暫存器自動 PUSH 入 Stack。
* **好處**：零延遲即時響應、允許工程師直接用純 C 語言撰寫 ISR（符合 C-ABI 規範）、支援尾鏈接 (Tail-Chaining) 效能最佳化。

### 3. 硬體 Timer vs 軟體 Timer 與 Block 禁忌
* **硬體 Timer**：由晶片內部實體計數器 (Prescaler/Counter) 驅動，觸發真實硬體 ISR。
* **軟體 Timer**：由 RTOS 核心任務 (FreeRTOS `Timer Service Task / prvTimerTask`) 統一託管，由 SysTick 提供 Tick 時脈。
* **Callback 嚴禁 Block 原因**：所有的軟體 Timer Callback 均共用同一個 `Timer Service Task` 任務內容。若某一 Callback 呼叫 `vTaskDelay` 或等待 Semaphore 進入 Block 狀態，整個 Timer Task 將被卡死，導致全系統所有軟體 Timer 停止運作！
