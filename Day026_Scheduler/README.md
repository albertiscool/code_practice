# Day 26: FreeRTOS 排程器 (Scheduler) 核心演算法與硬體機制

今天我們深入解剖了 RTOS 的大腦 —— **排程器 (Scheduler)**。
從 O(1) 的極速硬體演算法，到無情的搶占式階級制度，以及隱藏在最底層的 Idle 任務省電奧秘！

## 1. ⚡ O(1) 排程演算法：硬體暴力美學
為了達到絕對的「即時性 (Real-Time)」，排程器找尋下一個任務的時間必須是常數 $O(1)$，絕不允許使用會隨任務數量變慢的 $O(N)$ `for` 迴圈。
* **位元地圖 (Bitmap)**：使用一個 32-bit 整數，每個 Bit 代表一個優先級（0~31）。只要該優先級有任務 Ready，就把對應 Bit 設為 1。
* **`CLZ` 指令 (Count Leading Zeros)**：呼叫 ARM 晶片專屬的硬體組合語言指令 `CLZ`，在 1 個時鐘週期內瞬間算出前面有幾個 `0`。透過公式 `31 - CLZ數量`，瞬間定位出當前全系統最高優先級！

## 2. ⚔️ 搶占式排程 (Preemptive Scheduling)
RTOS 的鐵血法則：**「CPU 永遠只屬於當前狀態為 READY 且優先權最高的人。」**
* **搶占 (Preemption)**：如果低優先權任務 (如 YOLO 運算) 正在執行，這時外部中斷 (ISR) 喚醒了高優先權任務 (如緊急感測器)，排程器會**瞬間強制剝奪**低優先權的 CPU 執行權，雙手奉上給高優先權任務，保證系統極致的安全與響應速度。

## 3. ⏱️ 輪轉排程與時間切片 (Round-Robin & Time Slicing)
當系統中有多個任務處於 **「相同最高優先級」** 時：
* **時間切片**：排程器會讓他們「輪流跑」。每經過一個 SysTick (通常 1 毫秒)，就強制 Context Switch 換下一個人，產生同時執行的幻覺。
* **致命陷阱**：如果 `FreeRTOSConfig.h` 裡的 `configUSE_TIME_SLICING` 被設為 0，拿到 CPU 的任務就會死佔著不放，導致其他同級任務被「餓死 (Starvation)」。

## 4. 🛌 空閒任務 (Idle Task) 的硬體本質
系統啟動時，會強制創建一個優先權為 0 (最低階級) 的 Idle Task。
* **存在的物理意義**：因為 CPU 物理上無法「暫停執行指令」。當所有工作任務都進入 Blocked (休眠) 態時，CPU 必須有個空迴圈可以跑。
* **省電核彈 (`WFI`)**：工程師會在 Idle Task 裡塞入 ARM 專屬硬體指令 `WFI (Wait For Interrupt)`。CPU 讀到此指令會直接切斷核心時鐘，進入微安培級的深度睡眠，直到下一次中斷喚醒。
* **收屍人 (Garbage Collection)**：當使用 `vTaskDelete()` 刪除任務時，被刪除的任務無法自己釋放記憶體。這個釋放 TCB 與 Stack (防堵 Memory Leak) 的工作，就是在背景由 Idle Task 默默完成的。
