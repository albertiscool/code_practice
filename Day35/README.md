# Day 35: Linux 跨行程零拷貝 (Shared Memory) 與系統中斷防禦 (EINTR)

今天我們達成了 Linux 系統程式開發的兩大里程碑：實作了效能達到物理極限的「跨行程共享記憶體 (Zero-copy)」，並學會了如何撰寫出不畏懼任何系統訊號干擾的「工業級優雅關機防禦」。

## 1. 🚀 IPC 效能極限：Shared Memory (共享記憶體)
傳統的 Pipe 需要在核心層與應用層之間來回拷貝資料。為了傳輸極大量的資料 (如 AI 影像辨識、無人機點雲)，我們必須使用 Shared Memory。
* **`shm_open`**：在純 RAM 空間 (`/dev/shm`) 開啟一個虛擬的共享物件。
* **`mmap` 的 MMU 魔法**：將這個虛擬物件，直接對應為我們 C 程式裡的標準指標。
* **Zero-copy (零拷貝)**：透過指標賦值 (`p_telemetry->temp = 78;`)，數值會以光速直接寫入實體 RAM，另一個行程 (Process) 瞬間就能看見。完全不經過 Kernel 拷貝。

## 2. 🛡️ 跨行程的安全鎖與「資料撕裂 (Data Tearing)」
共享記憶體在沒有保護的情況下，若寫入端寫到一半被中斷，讀取端就會讀到「新舊參半」的髒資料 (資料撕裂)。
**防禦法**：必須使用跨行程的具名號誌 **`sem_open("/my_lock")`** 或是共享的 Mutex，確保在寫入與讀取時擁有獨佔權。

## 3. 💥 系統中斷 (`EINTR`) 的完美脫逃魔術
當伺服器正在 `epoll_wait` 沉睡時，任何 OS 訊號（如調整視窗或 Ctrl+C）都會無情將其踹醒，並拋出 `EINTR` (中斷) 錯誤。
* **直覺陷阱**：直接當作 Error 關閉程式，會導致伺服器頻繁且無故崩潰。
* **架構師解法 (`continue` 傳送門)**：
  ```c
  if (errno == EINTR) { continue; }
  ```
  當捕捉到 `EINTR` 時，利用 `continue` 瞬間放棄本次迴圈後面的所有耗時程式碼，直接飛回迴圈頂端重新檢查 `while` 條件。

## 4. 🛑 Graceful Shutdown (優雅關機)
透過註冊 Signal Handler，當接收到 `Ctrl+C (SIGINT)` 時，只修改全域變數 `g_keep_running = 0`。
配合 `continue` 魔術飛回頂端後，迴圈因條件不成立而終止，程式順利走到最後的清除區域執行 `shm_unlink` 與 `close`，不留下任何記憶體垃圾，這就是所謂的 **優雅關機 (Graceful Shutdown)**。
