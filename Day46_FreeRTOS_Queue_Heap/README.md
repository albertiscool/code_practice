# Day 46: 總複習 Part 6 - FreeRTOS 佇列、Event Groups 與記憶體管理 (heap_1~5)

今天是 10 天特訓總複習的第六天，我們深入複習了 FreeRTOS 工具選型決策樹 (Queue vs Event Group vs Task Notification)、`heap_1` 至 `heap_5` 動態記憶體管理的物理區別，以及 Queue 預設 Pass-by-Value 與滿佇列超時防禦。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【架構選型題：Queue vs Event Group vs Task Notification】
* **原問題**：分析 3 個嵌入式場景 (多條件AND解鎖、128B 數據帶 Payload 傳輸、1-to-1 極速中斷通知) 適合選用何種工具。
* **解析與解答**：
  1. **場景 A (多條件AND) ➡️ `Event Group`**：`xEventGroupWaitBits()` 支持 `xWaitForAllBits = pdTRUE` 多位元廣播與邏輯 AND 同步解鎖。
  2. **場景 B (帶 Payload 傳輸) ➡️ `Queue`**：Queue 提供執行緒安全 (Thread-Safe) 的 FIFO 緩衝區與長度對齊，適合解耦傳輸數據結構體。
  3. **場景 C (極速 1-to-1 通知) ➡️ `Task Notification`**：中斷通知特定任務，直接修改 TCB 內部數值，節省 ~45% RAM 且速度最快。

### 題目 2【記憶體管理除錯題：FreeRTOS `heap_1` ~ `heap_5` 大對決】
* **原問題**：頻繁建立刪除 Task 為何不能用 `heap_1`？`heap_4` 如何解決碎片？外掛不連續 SDRAM 應選哪一個？
* **解析與解答**：
  1. **`heap_1` 無 `vPortFree`**：`heap_1` 僅允許配置不可釋放，頻繁動態建立/刪除 Task 會導致記憶體瞬間枯竭。`heap_4` 具備 **Coalescing (記憶體對齊合併算法)**，能自動將相鄰空閒區塊黏合為大區塊，消滅碎片。
  2. **`heap_5` 支援不連續記憶體**：唯一支援 `vPortDefineHeapRegions()`，能將位址不連續的內建 SRAM 與外掛 SDRAM 串接為統一 Heap。

### 題目 3【Queue 佇列機制與死鎖防禦】
* **原問題**：`xQueueSend` 預設是 Pass by Value 還是 Reference？Queue 滿時執行 Send 逾時會發生什麼事？回傳值為何？
* **解析與解答**：
  1. **預設 Pass by Value (複製值)**：FreeRTOS 預設使用 `memcpy` 將整份結構體內容複製進 Queue 緩衝區（防止發送端局部變數銷毀導致接收端讀到髒資料）。若結構體極大，工程師會手動建立指標 Queue 傳送位址。
  2. **休眠等待與 `errQUEUE_FULL`**：Task A 會進入 `Blocked` 狀態休眠等待 1000ms。若 1000ms 後仍滿，`xQueueSend` 會喚醒 Task A 並回傳 **`errQUEUE_FULL` (`pdFAIL` / 0)**。
