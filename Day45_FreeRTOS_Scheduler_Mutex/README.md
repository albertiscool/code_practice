# Day 45: 總複習 Part 5 - FreeRTOS 搶佔式排程、優先權逆轉與 Task Notification

今天是 10 天特訓總複習的第五天，我們深入複習了 FreeRTOS 搶佔式排程器 (Preemptive Scheduler) 在 `vTaskDelay` 時的時間線切換、優先權逆轉 (Priority Inversion) 與互斥鎖 (Mutex) 的優先權繼承 (Priority Inheritance) 物理救命機制，以及 ISR 中斷使用 Task Notification 的省記憶體優勢。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【時間線推導題：FreeRTOS 搶佔式排程】
* **原問題**：`Task_High` (優先級 3) 執行 `vTaskDelay(100)` 後 CPU 切換給誰？100ms 時間到了喚醒時 CPU 發生什麼事？
* **解析與解答**：
  1. **切換給 `Task_Low`**：`Task_High` 進入 `Blocked` 休眠狀態，排程器立刻將 CPU 控制權切換給處於 `Ready` 狀態的 `Task_Low` (優先級 1) 執行。
  2. **強行搶佔 (Preemption)**：100ms SysTick 定時器觸發，將 `Task_High` 移回 `Ready` 狀態。因其優先級 (3) 高於 `Task_Low` (1)，排程器會發動**搶佔**，中斷 `Task_Low` 並立刻切換回 `Task_High` 執行。

### 題目 2【經典除錯題：Priority Inversion (優先權逆轉) 與解藥】
* **原問題**：Task A (高)、Task B (中)、Task C (低)。當 Task C 握鎖時 Task A 等待鎖，此時無鎖的 Task B 醒來會發生什麼？Mutex 如何解決？
* **解析與解答**：
  1. **慘案發生 (Task B 搶到 CPU)**：不需要鎖的 Task B (優先級 2) 會搶佔握有鎖的 Task C (優先級 1)。這導致 Task C 無法執行歸還鎖，進而讓最急的 Task A (優先級 3) 卡死等待，發生高優先級任務被中優先級任務卡死的 **優先權逆轉 (Priority Inversion)**。
  2. **解藥 (Mutex 優先權繼承)**：應使用 Mutex。當 Task A 嘗試獲取被 Task C 佔有的 Mutex 時，FreeRTOS 會觸發 **Priority Inheritance**，暫時將 Task C 的優先級提升至 3，防止 Task B 搶佔。Task C 快速歸還鎖後恢復原優先級，Task A 瞬間拿到鎖執行。

### 題目 3【架構選型題：Mutex vs Semaphore vs Task Notification】
* **原問題**：為什麼 ISR 中絕對不能呼叫 `xSemaphoreTake(Mutex)`？在中斷單向通知 Task 場景下官方最推薦哪種機制？
* **解析與解答**：
  1. **ISR 嚴禁 Block/Sleep**：若 Mutex 已被佔用，`xSemaphoreTake` 會嘗試讓調用者進入休眠 (Block) 讓出 CPU。然而 **ISR 屬於中斷上下文，沒有 TCB 亦絕不允許休眠**！在 ISR 拿鎖會引發核心 Assertion 崩潰或系統死鎖。且 Mutex 的優先權繼承對沒有任務優先級的 ISR 完全無效。
  2. **Task Notification (任務通知)**：在 ISR 1-to-1 單向通知場景下，官方最推薦 `vTaskNotifyGiveFromISR()`。因為它直接修改目標 Task TCB 內部的 32-bit 變數，不需要配置 Semaphore 佇列結構體，**節省高達 ~45% RAM 且切換速度更快**！
