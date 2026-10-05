# Day 34: Linux 高效能伺服器底層奧義 - Epoll 與 IPC 零拷貝

今天我們進入了 Linux 系統程式設計的最深處，探討了支撐全球網路巨頭 (如 Nginx, Redis) 高併發運作的核心底層技術。

## 1. 🗂️ 一切皆檔案與 I/O 多路復用 (epoll)
* **File Descriptor (FD)**：Linux 將所有硬體感測器、網路 Socket 抽象化為一個整數編號 (FD)，統一管理介面。
* **`select` 的 $O(N)$ 悲劇**：傳統方法在喚醒時，程式必須巡邏所有的 FD 來檢查誰有資料，連線數破萬時效能會雪崩。
* **`epoll` 的 $O(1)$ 神蹟**：利用核心的「紅黑樹」註冊與硬體中斷驅動的「就緒鏈結串列 (Ready List)」。當 `epoll_wait` 喚醒時，系統會直接把「有資料的 FD 清單」交給你，不管註冊了十萬個還是百萬個連線，處理時間永遠是常數級別。

## 2. ⚡ 觸發模式對決：LT (水平觸發) vs ET (邊緣觸發)
* **LT (Level-Triggered)**：預設的「碎念型郵差」。只要 Buffer 裡還有資料沒讀完，`epoll_wait` 就會不斷彈醒你。極度安全，不會遺漏。
* **ET (Edge-Triggered)**：高效能的「高冷型郵差」。只在資料到達的「那一瞬間」喚醒一次。若沒有一次把資料讀乾淨，剩餘資料將永遠卡死在記憶體裡 (Deadlock)。

## 3. 🏃‍♂️ ET 模式的保命符：Non-blocking I/O (非阻塞)
為了防止在 ET 模式下讀取資料時被卡死，**必須強制將 FD 設為非阻塞模式**。
在 `while` 迴圈中不斷 `read()`，直到信箱空了，系統不會讓你休眠，而是瞬間打臉回傳 `-1` 並附帶 `EAGAIN` 錯誤碼。收到 `EAGAIN` 才代表信箱徹底乾淨，可以安心回去呼叫 `epoll_wait`。

## 4. 🚀 IPC 物理極限：Shared Memory 與 Zero-copy (零拷貝)
* 當兩個獨立的 Process (如 YOLO 與 UI) 需要高速傳遞大量 4K 影像時，傳統 Pipe 會在 User / Kernel Space 產生巨量的拷貝負擔。
* **MMU 魔法**：透過 Shared Memory (`shm_open` / `mmap`)，Linux 核心會命令 MMU 將兩個 Process 的虛擬地址強制映射到「同一塊實體 RAM 晶片」。
* **零拷貝 (Zero-copy)**：寫入即讀出，資料傳輸延遲物理歸零。但因為是裸露的記憶體，必須搭配「跨行程的 POSIX Mutex/Semaphore」來防禦 Race Condition 競態條件。
