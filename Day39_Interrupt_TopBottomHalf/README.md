# Day 39: Linux 核心中斷處理哲學 - 上半部 (Top Half) 與下半部 (Bottom Half / Workqueue)

今天我們解鎖了 Linux 核心驅動開發中最關鍵的高硬核架構：**中斷處理的上下半部切割 (Top/Bottom Half Split)**，並實作了現代核心最熱門的下半部機制——**Workqueue (工作佇列)**。

## 1. ⚡ 為什麼中斷處理必須切成兩半？
* **硬體中斷的特權**：當 ISR (Top Half) 執行時，CPU 的中斷會被強制關閉，作業系統排程器也被凍結。
* **不切分的災難**：若在 ISR 裡面慢慢解析封包或讀取檔案，CPU 會長時間對外界耳聾眼瞎，導致系統無法回應鍵盤、滑鼠或高優先級事件，甚至引發 **Kernel Panic 死機**。
* **黃金切分法則**：
  * **上半部 (Top Half / ISR)**：極速 (< 1 $\mu s$) 清除硬體中斷標記 (ACK)，登記下半部排程後**立刻退出**。**絕對禁止沉睡 (Cannot Sleep)**。
  * **下半部 (Bottom Half)**：恢復中斷回應後，在背景慢慢執行耗時的封包解析與邏輯運算。

## 2. 🛠️ Linux 核心三大下半部工具
1. **Softirq (軟中斷)**：極速原子操作，專門服務 100Gbps 網卡與 SCSI 磁碟，不可沉睡。
2. **Tasklet**：封裝自 Softirq，簡單易用，不可沉睡。
3. **Workqueue (工作佇列)** 🌟 **現代最推薦**：
   * 在核心背景執行緒 (`kworker`) 中運作 (Thread Context)。
   * **允許沉睡 (Can Sleep)**！可安全呼叫 `msleep()`、等待 I/O 或存取 Mutex。

## 3. 📦 Workqueue 資料傳遞與 IPC 脈絡 (.data)
* **`void *data` 的裝箱拆箱**：背景 `kworker` 執行緒是全系統共用的，透過 `.data` 指針將硬體暫存器與封包記憶體位址傳遞給下半部。
* **極速退場機制**：上半部 ISR 內部**不需要**手動打包複雜資料，只需呼叫 `schedule_work()` (模擬碼中將 `is_scheduled = true`) 打開開關即可極速退場。
