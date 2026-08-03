# Day 47: 總複習 Part 7 - Linux 多執行緒 Pthreads、Condition Variables 與虛假喚醒防禦

今天是 10 天特訓總複習的第七天，我們深入複習了 Linux MMU 虛擬記憶體與當機隔離機制、Process vs Thread 的記憶體共享權界、POSIX Condition Variables 原子級 `pthread_cond_wait` 三合一操作、`while` 迴圈防禦虛假喚醒與 `pthread_cond_broadcast` 全員喚醒。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【架構觀念題：Linux 多執行緒 vs FreeRTOS 任務】
* **原問題**：壞指標在 RTOS vs Linux 的下場？Linux 靠什麼單元防護？同 Process 的 Pthreads 記憶體是共享還是隔離？
* **解析與解答**：
  1. **MMU 虛擬記憶體防護**：裸機/RTOS 無記憶體隔離，壞指標引發 HardFault 全機死機。Linux 擁有 **MMU (Memory Management Unit)**，壞指標引發 Page Fault 後僅對該 Process 發送 `SIGSEGV` (Segmentation fault) 強制關閉，**核心與其他程式依然穩定運作**。
  2. **Thread 共享記憶體**：同一個 Process 內的 Pthreads **預設共享 Heap、Data 全域變數與檔案描述符 (FD)**，僅擁有獨立的 Stack。因此多執行緒存取全域資料必須使用 Mutex 保護。

### 題目 2【經典除錯題：Condition Variable 與 `while` 迴圈防禦】
* **原問題**：`pthread_cond_wait` 底層三動作？為何必須將 `if (g_data_count == 0)` 改為 `while`？
* **解析與解答**：
  1. **原子三合一操作**：`pthread_cond_wait` 一氣呵成執行：**解鎖 Mutex ➡️ 進入 Block 休眠 ➡️ 被喚醒瞬間自動重新搶鎖 (Lock Mutex)**。
  2. **`while` 防禦虛假喚醒與搶奪**：若用 `if`，當 2 個 Consumer 同時被廣播喚醒時，A 拿走資料變 0，B 醒來後會跳過判斷直接執行 `count--` 導致資料變 `-1` (Underflow 當機)。改用 `while` 醒來會**重新檢查條件**，發現資料被拿走即安全重新休眠。

### 題目 3【核心機制題：`pthread_cond_signal` vs `pthread_cond_broadcast`】
* **原問題**：`signal` 與 `broadcast` 物理差異？多任務等待緊急電壓超標應呼叫何者？
* **解析與解答**：
  1. **喚醒數量差異**：`pthread_cond_signal()` 僅喚醒**至少一個**等待中的 Thread (適合 1-to-1 生產者/消費者)；`pthread_cond_broadcast()` 則會喚醒**所有**休眠等待該條件的 Threads。
  2. **緊急廣播選用 `broadcast`**：多任務 (風扇、LED、Log) 等待緊急事件時，必須使用 `pthread_cond_broadcast()` 確保全家大小通通叫醒執行緊急處置。

---

## 🚀 專題特訓：Process vs Thread 記憶體地圖與 `while` 迴圈防禦時間線

### 1. Process vs Thread 記憶體邊界
* **Process (行程)** ➡️ **獨立隔離**：Process A 與 Process B 擁有獨立 MMU 頁表，彼此記憶體完全隔離（需使用 `shm_open` 共享記憶體進行 IPC）。
* **Thread (執行緒)** ➡️ **共享記憶體**：同 Process 內的 Threads 共享 **Heap、Data 全域變數與 FD**，僅擁有私有 Stack。多執行緒同時讀寫全域變數會引發 **Race Condition**，故必須使用 `pthread_mutex_t` 保護。

### 2. 虛假喚醒中 `if` 引發 `-1` Underflow 災難的時間線
當 `g_data_count = 0` 時，Consumer A 與 Consumer B 皆通過 `if` 檢查並睡在 `pthread_cond_wait` 內部：
1. Producer 產出 **1 個資料** (`g_data_count = 1`) 並發動廣播。
2. Consumer A 搶到 Mutex 先醒來，將資料拿走 (`g_data_count--` 變為 `0`) 後解鎖離開。
3. Consumer B 隨後搶到 Mutex 醒來：
   * **若使用 `if`**：Consumer B 已於睡前通過 `if` 檢查，醒來後**不重新檢查條件**，直接向下執行 `g_data_count--` ➡️ **`g_data_count` 變為 `-1` (Underflow 記憶體爆掉！)**。
   * **若使用 `while`**：Consumer B 醒來後**強制重飛回頂端重新檢查 `while (g_data_count == 0)`**，發現資料已遭 A 搶走，再次安全進入 `pthread_cond_wait` 繼續休眠！
