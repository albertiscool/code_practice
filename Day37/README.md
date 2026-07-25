# Day 37: Linux 核心字元裝置驅動程式 (Char Device Driver) 架構

今天我們跨越了從應用層 (User Space) 到核心層 (Kernel Space) 的最後一道牆，徹底剖析了 Linux 字元裝置驅動程式 (Char Device Driver) 的核心結構、記憶體邊界防護與硬體控制 API。

## 1. 📂 系統呼叫路由與 `file_operations`
在 Linux「一切皆檔案」的哲學下，應用層對 `/dev/my_sensor` 執行的系統呼叫，會被 VFS 檔案系統路由到驅動程式對應的 Function Pointer：
* `open()` ➡️ `my_driver_open()` （獨佔性檢查）
* `write()` ➡️ `my_driver_write()` （搬移寫入資料）
* `read()` ➡️ `my_driver_read()` （搬移讀取資料）
* `close()` ➡️ `my_driver_release()` （釋放硬體資源）

## 2. 🛡️ 核心防衛天條：`copy_from_user` / `copy_to_user`
在 Driver 內部**絕對不能使用 `memcpy` 直接存取 User 傳入的指標**！
* **致命原因**：若 User 指標為野指標或未對應記憶體，用 `memcpy` 會引發 Kernel Page Fault，直接導致 **Kernel Panic (藍屏死機重啟)**，或觸發現代 CPU (ARM PAN / x86 SMAP) 的硬體阻擋異常。
* **安全 API 救援**：`copy_from_user` 與 `copy_to_user` 提供：
  1. **範圍檢查 (Range Check)**：防範越界存取 Kernel 機密記憶體。
  2. **Exception Fixup Table**：遇到壞指標時優雅攔截異常，回傳未搬移 Bytes 數，絕不導致死機。

## 3. 🔢 主次設備號 (Major / Minor Device Numbers)
在 Linux 中，設備號統一封裝在 32-bit 的 `dev_t` 型態中：
* **Major Number (主設備號)**：代表「這是哪一個驅動程式 (Which Driver)」。核心靠它決定要喚醒哪一份 `file_operations`。
* **Minor Number (次設備號)**：代表「該驅動管轄下的第幾個硬體實體 (Which Instance)」。例如 `/dev/ttyUSB0` 與 `/dev/ttyUSB1` 主設備號同為 `188`，但次設備號分別為 `0` 與 `1`。

## 4. 🛠️ 硬體控制瑞士軍刀：`ioctl`
* **`read()` / `write()` ➡️ 資料通道 (Data Path)**：負責傳送感測器資料串流。
* **`ioctl()` ➡️ 控制通道 (Control Path)**：負責傳送非資料類的硬體控制指令（如設定 UART 波特率、晶片 Reset、GPIO 腳位切換）。驅動內部使用 `switch(cmd)` 與核心巨集 (`_IOW`, `_IOR`) 來精準捕捉並控制暫存器。
