# Day 38: Linux 核心驅動程式 ioctl 控制介面實作

今天我們深入探討了 Linux 驅動程式的「控制通道 (Control Path)」—— **`ioctl` (Input/Output Control)** 系統呼叫介面，學習了如何設計安全的硬體控制命令與錯誤處理機制。

## 1. 🛠️ 資料通道 vs 控制通道
* **`read()` / `write()` ➡️ 資料通道 (Data Path)**：負責傳送感測器大量串流資料。
* **`ioctl()` ➡️ 控制通道 (Control Path)**：負責下達非資料類的硬體設定指令（如波特率設定、硬體 Reset、GPIO 切換）。
* 在驅動內部對應 `long (*unlocked_ioctl) (struct file *, unsigned int cmd, unsigned long arg)`。

## 2. 🔀 三大控制傳參型態
在驅動內部的 `switch (cmd)` 處理中，`arg` 參數有三種常見用法：
1. **值傳遞 (Value Passing)**：直接將 `arg` 強制轉型為數字（如 `uint32_t baud = (uint32_t)arg`）。
2. **指標傳遞 (Pointer Passing)**：將 `arg` 當作 User 記憶體位址解讀，回傳複雜結構（如 Chip ID `0x32F4`）。
3. **無參數動作 (Action Trigger)**：不傳送任何資料，單純觸發晶片重置 (Reset)。

## 3. 🎯 命令編碼巨集 (ioctl Command Macros)
以 **User Space 視角** 定義資料流向，巨集會將資料型態長度自動編入 32-bit `cmd` 命令碼中，提供核心安全檢查：
* **`_IOR(type, nr, datatype)`**：從 Driver 讀資料給 User (Read)。
* **`_IOW(type, nr, datatype)`**：從 User 寫資料給 Driver (Write)。
* **`_IOWR(type, nr, datatype)`**：雙向資料交換 (Read & Write)。
* **`_IO(type, nr)`**：純動作觸發，不傳送資料 (No Data)。

## 4. ⚠️ 核心錯誤碼機制 (-EINVAL)
* 當應用層傳入無效命令 (如 `0xDEAD`) 時，驅動程式內部回傳負數 **`-22` (`-EINVAL`)**。
* `glibc` 接收到負數錯誤碼後，會自動將應用層 API 的回傳值改為 `-1`，並設置全域變數 **`errno = EINVAL`** (Invalid argument)。
