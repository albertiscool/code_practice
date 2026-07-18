# Day 31: 系統架構師的視野 - FreeRTOS 與 Linux 底層對決

今天是鐵人特訓的最後一哩路，我們跳脫了單純的程式碼，以上帝視角俯瞰了微控制器 (MCU/FreeRTOS) 與微處理器 (MPU/Linux) 在物理架構上的終極差異。

## 1. 🛡️ 記憶體防護：裸奔 vs 五星級飯店 (MMU)
* **FreeRTOS (無 MMU)**：指標直接操作實體記憶體，執行效率極致。但毫無防護機制，一個 Task 的指標寫錯（如寫入 `0x00000000`），會直接踩爛系統核心，導致無人機當場墜毀。
* **Linux (具備 MMU)**：硬體 MMU (記憶體管理單元) 提供了「虛擬記憶體」的幻覺。每個行程 (Process) 都以為自己獨佔 4GB 空間，實則被物理隔離。若發生越界讀寫，MMU 會瞬間硬體攔截，Linux 核心只會單獨擊殺該行程 (`Segmentation fault`)，系統其餘控制單元依然完美運作。

## 2. ⏱️ 即時性之戰：硬即時 vs 軟即時 (Hard vs Soft Real-Time)
* **Linux 預設 (CFS 完全公平排程器)**：為了顧及所有程式的公平性與吞吐量，會適度妥協讓低優先權任務執行，導致高優先權任務產生毫秒級延遲，此為「軟即時 (Soft Real-Time)」。
* **Linux 硬即時方案 (`SCHED_FIFO`)**：若要將 Linux 應用於車用溫控或飛彈追蹤，必須繞過 CFS，將排程策略設定為 `SCHED_FIFO`。這賦予了任務絕對的搶佔權與獨佔權，保證微秒級的極速反應。

## 3. 💥 多核心的暴力美學與超級災難 (Parallelism)
* **單核心 (併發 Concurrency)**：依靠快速 Context Switch 輪流欺騙眼睛。透過 `taskENTER_CRITICAL()` 關閉中斷即可完美防止 Race Condition。
* **多核心 (真並行 True Parallelism)**：Jetson Nano 等多核處理器能在同一物理奈秒，讓 4 顆核心同時消化 4 倍的運算量。
* **架構黑暗面**：關閉單一核心的中斷，根本防不住其他 3 顆核心同時來修改同一塊記憶體。工程師被迫捨棄軟體鎖，改用硬體級防禦機制（如自旋鎖 Spinlock、原子指令 Atomic Instructions）來鎮壓跨核心的超級競態條件。
