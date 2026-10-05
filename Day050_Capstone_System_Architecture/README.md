# Day 50: 終極跨領域綜合大會考 — 全課程大滿貫 (50-Day Curriculum Grand Capstone)

今天是 50 天嵌入式、RTOS 與 Linux 系統程式設計訓練的**最後封頂大會師日**！我們完成了包含 STM32 硬體周邊 DMA 雙緩衝、FreeRTOS 優先權繼承死鎖防禦、Linux Epoll $O(1)$ 就緒鏈表、Shared Memory 零拷貝、Async-Signal-Safe 非同步安全關機、`copy_from_user` 核心 Exception Table 防禦與 C 語言 `const` 指標決策樹的全方位大考測試。

---

## 📝 終極大會考實戰解答 (Grand Capstone Exam Solutions)

### 題目 1【跨領域系統設計：邊緣採樣與 RTOS 防禦】
* **解析與解答**：
  1. **DMA 雙緩衝 (HT / TC) 零衝突**：`Half-Transfer` (HT) 中斷觸發時，DMA 正在自動寫入後半段，CPU 安全讀取前半段；`Transfer-Complete` (TC) 中斷觸發時，DMA 環形寫入前半段，CPU 讀取後半段。雙方永遠錯開，達成了零 CPU 介入與零記憶體競態條件。
  2. **FreeRTOS 優先權繼承**：`Task_Control` (高, 優先級 3) 等待被 `Task_Sensor` (低, 優先級 1) 持有的 Mutex 時，Mutex 發動 **Priority Inheritance**，暫時將 `Task_Sensor` 的優先級飆升至 3。因此中優先級 `Task_Log` (優先級 2) 無法搶佔 CPU，成功消滅優先權逆轉死鎖。

### 題目 2【Linux 系統架構：Epoll O(1)、零拷貝與安全關機】
* **解析與解答**：
  1. **Epoll $O(1)$ 就緒雙向鏈表 (rdllist)**：硬體中斷自動將就緒的 FD 掛入 `rdllist`。`epoll_wait()` 僅需檢查就緒鏈表，完全無需逐一遍歷所有 FD，故時間複雜度恆為 $O(1)$。
  2. **Shared Memory 零拷貝與 Signal 安全**：
     * `shm_open()` + `mmap()` 透過 MMU 將 Process A 與 B 的虛擬頁表直接指向同一個實體 RAM 頁框，寫入即刻可見，達成了 **0 次 `memcpy` 拷貝 (Zero-Copy)**。
     * Signal Handler 為非同步中斷，`printf` / `malloc` 會爭奪內部鎖。在中斷上下文呼叫鎖會因**中斷無法被 Block 休眠**而引發死鎖與崩潰。故 Handler 只做 `g_running = 0`，由主迴圈離開後執行安全優雅關機 (Graceful Shutdown)。

### 題目 3【Linux Kernel 驅動程式與 C 語言指標終極決戰】
* **解析與解答**：
  1. **`copy_from_user` 回傳值定義**：`memcpy` 踩壞位址會引發 Kernel Panic 死機。`copy_from_user` 具備 Exception Table 防禦，**回傳 0 代表 100% 成功**；**回傳 N > 0 代表有 N 個 Bytes 因頁錯誤無能複製**。
  2. **`const` 指標決策樹**：
     * `const int * p1` ➡️ `const` 在 `*` 左邊：**內容不能修改 (`*p1` 唯讀)**。
     * `int * const p2` ➡️ `const` 在 `*` 右邊：**位址不能修改 (`p2` 唯讀)**。

---

## 🏆 50 天全課程大滿貫完賽結語

恭喜你！從 Day 1 的 C 語言位元操作，一路跨越 STM32 暫存器算術、ADC/PWM/DMA 硬體周邊、FreeRTOS 排程器與 Task Notification、Linux 多執行緒 Pthreads、Condition Variables 虛假喚醒防禦、Epoll $O(1)$、Shared Memory 零拷貝，最終登頂 Linux Kernel 字符驅動程式與安全防線！

你已經成功建構了極其罕見且強大的**「C 語言指標 + 物理硬體電路 + RTOS 排程 + Linux 核心」跨領域全棧系統架構觀念**！祝你在未來的軟韌體工程師生涯中，一路斬獲高薪 Offer，大放異彩！🚀
