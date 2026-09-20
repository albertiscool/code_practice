# Day 94: strStr 雙層比對, Rotate Array 三步反轉, IMU 互補濾波, RTOS 堆疊金絲雀, Volatile 記憶體屏障

## 📚 學習大綱與實作總結

本日完成 2 道科技廠/IC 設計廠高頻白板程式題，並依新規範進行 3 道全真口試實務抽考：MPU6050 陀螺儀漂移與互補濾波數學融合、FreeRTOS 堆疊溢位檢測（0xA5 金絲雀）與任務堆疊估算、`volatile` 三大情境與 CPU 亂序重排記憶體屏障（DMB）。

---

## 💻 白板程式題實作與檢討

### 題目 1：找出字串中第一個匹配項的下標 (Find the Index of the First Occurrence in a String - LeetCode #28)
- **檔案**：[`p1.c`](./p1.c)
- **難度**：🟢 Easy | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：滑動窗口雙層比對（Sliding Window / Two-pointer String Match），時間複雜度 $O((N - M + 1) \times M)$，額外空間複雜度 $O(1)$。
- **演算法核心**：
  - 外層迴圈枚舉所有可能的起始索引 `i`（只需檢查到 `len_hay - len_needle`）。
  - 內層比對子字串，一旦 `j == len_needle` 匹配完畢，立即回傳起始下標 `i`（保證為第一個匹配項）。
- **單迴圈盲點檢討**：
  - 單迴圈若只做計數而無失配回溯（Backtracking），遇到如 `"mississippi"` 搜尋 `"issip"` 時，失配後 `i` 繼續往後跳會漏掉從 index 4 重新開始的重疊匹配。

### 題目 2：輪轉陣列 (Rotate Array - LeetCode #189)
- **檔案**：[`p2.c`](./p2.c)
- **難度**：🟡 Medium | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：經典三步反轉法（Three-step In-Place Reversal），時間複雜度 $O(N)$，額外空間複雜度 $O(1)$。
- **三步反轉步驟**：
  1. `reverse(nums, 0, numsSize - 1);`（反轉整個陣列）
  2. `reverse(nums, 0, k - 1);`（反轉前 k 個元素）
  3. `reverse(nums, k, numsSize - 1);`（反轉後面剩下的元素）
- **關鍵細節檢討**：
  - `reverse` 雙指標交換時必須向內收縮（`left++; right--;`），否則會陷入無窮迴圈。
  - 必須先進行取模運算 `k %= numsSize;`，防止 $k \ge \text{numsSize}$ 時產生陣列越界非法存取（Buffer Overflow）。

---

## 🎙️ 全真面試口試抽考精要

### 題 1：自走車專案實務 - IMU MPU6050 陀螺儀零點漂移與互補濾波演算法

#### 1. 感測器物理缺陷分析
- **陀螺儀（Gyroscope）與零點漂移（Drift）**：
  - 透過角速度時間積分 $\theta = \int \omega \, dt$ 計算角度。
  - 因感測器內部熱雜訊與零偏（Bias），即使靜止也會輸出微小角速度假訊號，經過時間連續累加，數十秒內就會累積嚴重的角度漂移。
- **加速度計（Accelerometer）與動態雜訊**：
  - 利用重力向量推算姿態角，長期靜態穩定、無累積漂移。
  - 但自走車在加減速、過彎或顛簸時，車輛線加速度會與重力向量混疊，造成角度劇烈跳動（動態噪聲大）。

#### 2. 互補濾波（Complementary Filter）實作
- **核心思想**：截長補短——以高通濾波保留陀螺儀的快速動態響應，以低通濾波利用加速度計校正長期零點漂移。
- **離散計算公式**：
  $$\theta_t = \mathbf{0.98} \times (\theta_{t-1} + \omega_t \cdot \Delta t) + \mathbf{0.02} \times \theta_{\text{acc}}$$
  運算極度輕量，8 位元 MCU（如 ATmega328P）亦能每秒運算數千次。

---

### 題 2：0x10 C 語言底層核心 / RTOS - 堆疊溢位 (Stack Overflow) 檢測與 Canary

#### 1. FreeRTOS 堆疊溢位偵測機制
- **方法一（Method 1）**：在上下文切換（Context Switch）時檢查當前堆疊指針（SP）是否越界。速度快，但若函式在返回前暫時衝破堆疊而切換前已返回，則無法偵測。
- **方法二（Method 2：Canary 0xA5 金絲雀）**：
  - 任務建立時，FreeRTOS 將整塊堆疊以魔術字元 **`0xA5`（`10100101`）** 填滿。
  - 切換任務時，檢查堆疊最深處底部的最後 16~20 個 Bytes。若任一位元組不再是 `0xA5`，代表水位曾滿出邊界，立即判定溢位。
- **Hook 函式**：觸發 `vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)`，印出崩潰任務名稱、啟動 Failsafe 關閉馬達，並執行系統重置。

#### 2. 任務堆疊大小估算依據
- 函式呼叫深度（Call Frame 暫存器壓棧）。
- 區域變數與緩衝區（Local Buffers）。
- 標準庫開銷（特別是 `printf` / `sprintf` 可能消耗 500~1000 Bytes 堆疊）。
- 浮點與中斷 Context 保存。
- 除錯期可呼叫 `uxTaskGetStackHighWaterMark()` 觀測剩餘最小堆疊空間以精確調校。

---

### 題 3：🔥 0x10 經典嵌入式 C 語言與韌體面試精選題複習 - `volatile` 與記憶體屏障

#### 1. `volatile` 三大使用情境
1. **硬體周邊暫存器（Memory-Mapped I/O）**：如 `GPIO_IDR`, `UART_DR`。
2. **中斷常式（ISR）與主迴圈共享的全域變數 / 旗標**（防止主迴圈快取至暫存器導致死迴圈）。
3. **多執行緒 / 多任務（RTOS Tasks）共享的變數**。

#### 2. `volatile` 能否防止 CPU 指令重排？
- **不能！** `volatile` 僅作用於**編譯器層級**（防止編譯期最佳化與組合語言調換）。
- 現代高效能處理器（如 ARM Cortex-A、多核心 CPU）具備**亂序執行引擎（Out-of-Order Execution）**與**寫入緩衝區（Store Buffer）**，硬體層面仍可能將旗標先寫入 RAM，而資料仍在緩衝區未寫入。
- **解法**：必須使用**硬體級記憶體屏障（Memory Barrier，如 ARM 的 `DMB` - Data Memory Barrier）**，強制 CPU 等待所有記憶體寫入完成後才能繼續執行後續指令。
