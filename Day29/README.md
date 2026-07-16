# Day 29: FreeRTOS 多重事件同步 (Event Groups) 與任務通知限制

今天我們探討了 RTOS 中用來處理「多重條件」與「極速喚醒」的兩大神器：**Event Groups (事件標誌群組)** 與 **Task Notification (任務通知)**，並深入分析了它們在系統架構設計上的絕對優勢與致命限制。

## 1. 🎯 Event Group：消滅瞎忙的事件驅動架構
* **痛點**：在裸機或簡單架構中，任務必須使用 `while(1)` 加上一堆 `if-else` 去不斷輪詢 (Polling) 硬體狀態，極度浪費 CPU 算力且程式碼雜亂。
* **解法**：Event Group 在底層只用一個 32-bit 的整數，每個 Bit 代表一個獨立事件。任務可以優雅地休眠 (`BLOCKED`)，直到中斷或驅動程式將特定的 Bit 設為 1，RTOS 才會把任務搖醒。
* **AND/OR 邏輯**：透過 `wait_for_all` 參數，可以輕鬆實作「Camera Ready **AND** Temp Safe (全部滿足才喚醒)」或「Button Pressed **OR** Network Arrived (滿足其一就喚醒)」。

## 2. 🛡️ 自動清零的原子防禦 (Auto-Clear)
* **無限迴圈 Bug**：如果任務被喚醒後，沒有將等待的 Event Bit 清零，任務會在下一次迴圈時因為「條件依然滿足」而立刻被喚醒，導致無限重算舊資料，吃光 GPU 算力。
* **`xClearOnExit = pdTRUE`**：設定此參數後，FreeRTOS 會在喚醒任務的那個物理瞬間，以「不可被打斷的原子操作 (Atomic Read-and-Clear)」將對應的 Bit 清零。保證絕無 Race Condition 與重複執行的 Bug。

## 3. ⚔️ Task Notification vs Queue/Event Group 的架構對決
Task Notification 是 FreeRTOS 中速度最快、最省 RAM 的機制，但它有三大物理硬傷，決定了它不能取代 Queue：
1. **破壞解耦 (Coupling)**：發送通知時必須硬編碼「收件人 (Task Handle)」。生產者必須知道消費者是誰，程式碼綁死。
2. **無法廣播 (1 對 N)**：無法像 Event Group 一樣，設定一次 Bit 就同時喚醒多個在等待的任務。
3. **無排隊機制 (深度僅有 1)**：底層只是一個 32-bit 變數。若消費者還沒醒來，生產者又連發通知，新資料會直接無情覆蓋舊資料，造成遺失。

**架構師結論**：
* 簡單的 1對1 極速中斷喚醒 $\rightarrow$ **Task Notification**。
* 多對多、需要解耦排隊 $\rightarrow$ **Queue**。
* 複雜的 AND/OR 多條件同步 $\rightarrow$ **Event Group**。
