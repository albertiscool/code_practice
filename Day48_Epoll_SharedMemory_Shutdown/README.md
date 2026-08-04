# Day 48: 總複習 Part 8 - Linux Epoll O(1)、Shared Memory 零拷貝與 Graceful Shutdown

今天是 10 天特訓總複習的第八天，我們深入複習了 Linux I/O 多路復用 (Epoll $O(1)$、紅黑樹 + 就緒雙向鏈表)、Level Triggered (LT) vs Edge Triggered (ET) 邊緣觸發模式險境、Shared Memory 零拷貝原理與 `volatile sig_atomic_t` 優雅關機 (Graceful Shutdown)。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【效能對比題：Linux I/O 多路復用 select / poll vs epoll】
* **原問題**：$O(N)$ vs $O(1)$ 的原因？epoll 核心兩大資料結構？為何不需要每次複製整份 FD？
* **解析與解答**：
  1. **紅黑樹 + 就緒鏈表 (O(1) 極速響應)**：`select`/`poll` 無狀態，需 $O(N)$ 逐一遍歷所有 FD。`epoll` 於核心維護 **紅黑樹 (Red-Black Tree)** 儲存 FD，並由硬體中斷自動將已準備好的 FD 掛載進 **就緒雙向鏈表 (rdllist)**。`epoll_wait()` 僅需檢查就緒鏈表，達成 $O(1)$ 響應。
  2. **`eventpoll` 持久化核心物件**：`epoll_create()` 在核心建立長久存活的 `eventpoll` 物件。`epoll_ctl()` 預先將 FD 註冊進紅黑樹，故 `epoll_wait()` 呼叫時免除每次從 User Space 複製萬個 FD 的巨額開銷。

### 題目 2【模式對比與除錯題：Level Triggered (LT) vs Edge Triggered (ET)】
* **原問題**：剩餘 1KB 資料時 LT 模式行為？ET 模式下沒讀完會怎樣？為何 ET 必須搭配 Non-blocking Socket？
* **解析與解答**：
  1. **LT 模式 (持續提醒)**：只要 Socket 接收緩衝區有資料，`epoll_wait()` 便會持續喚醒，直到資料清空。
  2. **ET 模式險境與 Non-blocking 必備**：ET 僅於邊緣轉變（新封包抵達）時通知 1 次。未讀完的 1KB 不會丟棄，但若無新封包，`epoll_wait()` 再也不會喚醒，引發 **Data Starvation 沉睡死鎖**。故 ET 必須搭配 `while(read())` 讀至 `EAGAIN`。若使用 **Blocking Socket**，讀空後的最後一次 `read()` 將導致 **CPU 執行緒永久卡死**！

### 題目 3【IPC 零拷貝與安全關機：Shared Memory & Signal Handling】
* **原問題**：Shared Memory 零拷貝原理？Signal Handler 中為何只做 `g_running = 0` 而不呼叫 `printf`/`malloc`？
* **解析與解答**：
  1. **MMU 實體記憶體映射 (零拷貝)**：`shm_open()` + `mmap()` 將 Process A 與 B 的虛擬記憶體頁表對應至 **同一個實體 RAM 頁框**。A 寫入資料 B 立刻可見，完全無需經過核心 `memcpy` 拷貝。
  2. **Async-Signal-Safe 優雅關機**：Signal Handler 屬於非同步中斷，呼叫非可重入函式 (如 `printf` 或需搶鎖的 `malloc`) 極易引發死鎖與 Heap 損毀。標準做法為僅修改 `volatile sig_atomic_t g_running = 0` 旗標，由主迴圈離開後執行安全資源釋放與優雅關機 (Graceful Shutdown)。

---

## 🚀 專題特訓：Linux 高效能 I/O、ET 模式險境與安全關機總複習

### 1. `select`/`poll` ($O(N)$) vs `epoll` ($O(1)$) 管理員比喻
* **`select`/`poll` ($O(N)$)**：每次外送員送來便當，住戶都要重新填寫 10,000 戶名單複製給管理員。管理員需跑 `for` 迴圈挨家挨戶敲 10,000 戶門 ($O(N)$) 尋找誰的便當到了。
* **`epoll` ($O(1)$)**：
  * **紅黑樹 (Red-Black Tree)**：管理員大廳立了公告牌，住戶只需註冊一次 (`epoll_ctl`)，免去每次複製名單開銷。
  * **就緒雙向鏈表 (Ready List)**：便當抵達時，硬體中斷自動將名牌掛入就緒鏈表。
  * **`epoll_wait()`**：管理員只需查看就緒鏈表，秒殺取得目標 FD，時間複雜度為 **$O(1)$**。

### 2. LT (準位觸發) vs ET (邊緣觸發) 鬧鐘比喻與 Non-blocking 必備
* **LT (Level Triggered)** ➡️ **強迫症老媽鬧鐘**：只要緩衝區內還有 1KB 殘留資料（Level 1），每次 `epoll_wait()` 都會持續喚醒提醒。
* **ET (Edge Triggered)** ➡️ **只響一聲的冷酷鬧鐘**：僅在「新封包抵達（0 變 1）」上升邊緣通知 1 次。若未一次讀光，殘留 1KB 將滯留緩衝區引發 **Data Starvation 沉睡死鎖**。
* **ET 搭配 Non-blocking 必備**：為讀光資料必須寫 `while(read())` 迴圈。若 Socket 為 **Blocking (阻塞)**，讀空後的最後一次 `read()` 將導致 **CPU 執行緒永久睡死**！使用 **Non-blocking** 可在讀空時回傳 `-1` (帶 `EAGAIN` / `EWOULDBLOCK`) 安全跳出迴圈。

### 3. Shared Memory 零拷貝與 Async-Signal-Safe 優雅關機
* **Shared Memory 零拷貝**：`shm_open()` + `mmap()` 透過 MMU 將 Process A 與 B 的虛擬頁表直接對應至 **同一個實體 RAM 頁框**。A 寫入資料 B 瞬間可見，中途 **0 次 `memcpy` 拷貝**。
* **Async-Signal-Safe 優雅關機**：Signal Handler 會在任意代碼位置中斷主程式。Handler 內部若呼叫 `printf` 或 `malloc`（需搶內部 Heap 鎖），會引發 **死鎖 (Deadlock) 或 Heap 損毀**。標準作法為 Handler 僅將 `volatile sig_atomic_t g_running = 0` 修改，由主迴圈跳出後執行安全資源清理與 Graceful Shutdown。
