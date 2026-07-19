# Day 32: 踏入 Linux 殿堂 - POSIX Threads (pthreads) 與多核心鎖設計

今天我們正式跨越了單晶片的界線，進入到 Linux (微處理器如 Jetson Nano) 的應用層世界。我們探討了 Linux 系統程式設計的核心：多執行緒 (Multi-threading) 以及多核心環境下的防禦與效能調校。

## 1. 語法對接：FreeRTOS ➡️ Linux Pthreads
在 Linux 世界中，一切皆遵循 POSIX 標準。我們將 RTOS 的概念完美映射過來：
* `xTaskCreate()` ➡️ `pthread_create()` (建立執行緒)
* `SemaphoreHandle_t` ➡️ `pthread_mutex_t` (互斥鎖)
* `xSemaphoreTake()` ➡️ `pthread_mutex_lock()`
* `vTaskDelay()` ➡️ `usleep()` (微秒級休眠)

## 2. 📦 `void*` 的裝箱與拆箱魔法 (Typecasting)
Linux 的 `pthread_create` 規定任務函式只能接收 `void*` (萬用不透明紙箱)。
* **裝箱 (Boxing)**：將包含各種參數的結構體 (Struct) 地址，強制轉型成 `(void*)` 交給執行緒。
* **拆箱 (Unboxing)**：執行緒收到紙箱後，立即將其強制轉型回原本的 `Struct*`，就像戴上了解碼眼鏡，精準讀取出裡面的複合資料。這提供了極致的參數傳遞彈性。

## 3. 🛡️ 拔插頭防禦：`pthread_join` 的物理意義
在 Linux 中，執行緒是寄生在行程 (Process) 底下的。
若 `main()` 主執行緒提早跑到 `return 0;` 結束，Linux 核心會開著推土機摧毀整個 Process，連帶無情砍殺所有還在背景運作的子執行緒（導致資料毀損）。
**解法**：主執行緒必須呼叫 `pthread_join()` 坐在大門口進入阻塞狀態 (Blocking)，確實等待所有子執行緒完工下班後，才能安全關閉系統。

## 4. 🚀 多核心的效能陷阱：粗粒度鎖 vs 細粒度鎖
在擁有 4 核心的 Jetson Nano 上：
* **粗粒度鎖 (Coarse-Grained) 💀**：把耗時 100ms 的運算包在 Mutex 裡。這會導致嚴重的「鎖競爭 (Lock Contention)」，其他 3 顆核心全被餓死發呆，多核處理器瞬間「序列化」退化成單核效能。
* **細粒度鎖 (Fine-Grained) 🏆**：架構師的標準寫法。耗時運算全部放在鎖外，利用區域變數各自「真並行」運算。只有在最後 1 微秒要「寫入全域共享變數」的瞬間才上鎖，將效能壓榨到極致！
