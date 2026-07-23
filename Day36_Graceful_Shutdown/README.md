# Day 36: Linux 優雅關機 (Graceful Shutdown) 與訊號攔截 (Signal Handling)

今天我們學習了 Linux 伺服器與邊緣運算裝置 (如 Jetson Nano) 必須具備的「守護行程 (Daemon) 防禦骨架」。確保系統在遭受 `Ctrl+C` 甚至重啟訊號時，能夠完美釋放資源，絕不留下共享記憶體垃圾。

## 1. 🛡️ 攔截作業系統核彈 (`sigaction`)
當使用者按下 `Ctrl+C`，OS 會發送 `SIGINT` 訊號強制將程式爆頭。
透過 `sigaction` API，我們可以向核心宣告：「請不要殺我，將訊號轉交給我的 `Signal_Handler` 副程式處理」。這賦予了我們自定義關機流程的能力。

## 2. 🛑 優雅關機 (Graceful Shutdown) 的標準骨架
這是一套防止 Memory Leak 的黃金架構：
1. **攔截訊號**：Handler 內部不做任何耗時動作，僅將全域旗標 `g_keep_running` 設為 `0`。
2. **自然終結**：主迴圈 `while(g_keep_running)` 偵測到旗標改變，自然打破迴圈往下走。
3. **安全清理**：順利執行迴圈下方的 `shm_unlink` (清除記憶體) 與 `close` (關閉檔案)，完美下車。

## 3. ⚛️ 變數防護盾：`volatile sig_atomic_t`
要作為訊號與主程式溝通的橋樑，變數宣告必須極度嚴謹：
* **`volatile`**：警告編譯器「不要快取這個變數」，確保主迴圈每次都會去主記憶體讀取最新狀態。
* **`sig_atomic_t`**：保證對此變數的修改在 CPU 底層為「一氣呵成 (Atomic)」的單一指令，徹底消滅訊號插斷造成的資料撕裂。

## 4. 💀 面試必考地雷：Handler 中的 `printf` 死鎖
絕對不可以在 Signal Handler 中呼叫 `printf`！
`printf` 底層具有互斥鎖 (Mutex)。若主程式在 `printf` 執行一半時被訊號打斷，而 Handler 內部又呼叫 `printf` 去搶奪同一把鎖，程式將當場引發 **死鎖 (Deadlock)** 永遠凍結。
**正解**：在 Handler 內部，只能嚴格使用 POSIX 認證的 Async-Signal-Safe (非同步訊號安全) API，例如直接呼叫底層的 `write()`。
