# Day 108: 群聯 Phison ＋ 電子五哥 韌體實戰模式 (交替位元 Pattern 檢測 × 雙指標向內平方 × ASCII 雙向查表 × D-Cache 與 DMA 一致性 × 視窗看門狗 WWDG)

## 📑 今日訓練概覽
- **P1 (位元操作與暫存器 Pattern 檢測)**: LeetCode #693: Binary Number with Alternating Bits (Easy) - 滑動位元比對 + IC 設計廠 $O(1)$ 純硬體無迴圈 XOR 神技 `(a & (a + 1)) == 0` (0 ms, Beats 100.00%)
- **P2 (雙指標向內收斂與陣列填寫)**: LeetCode #977: Squares of a Sorted Array (Easy) - 頭尾雙指標向內對決 + 倒序填寫 + `left <= right` 邊界防護 (0 ms, Beats 100.00%)
- **P3 (字串映射與 ASCII 查表法)**: LeetCode #205: Isomorphic Strings (Easy) - ASCII 256 雙向查表（一夫一妻制）+ 首次見面登記與衝突攔截 (0 ms, Beats 100.00%)
- **自走車專案實務科普 (第 108 講)**:
  - MPU6050 姿態儀 Data-Ready 硬體外部中斷採樣（非阻塞架構，保全 PID 週期）
  - 馬達減速齒輪箱高頻振動抑制：一階低通濾波器（LPF: $y[n] = \alpha \cdot x[n] + (1 - \alpha) \cdot y[n-1]$）
  - 正交編碼器（Quadrature Encoder）硬體四倍頻（4x Counting）與增量式 PID（Anti-windup）
  - 馬達堵轉安全監控（Stall Detection Failsafe）：避免 H 橋燒毀與急煞暴衝
- **0x10 韌體架構實戰情境題**:
  1. **D-Cache 快取一致性與 DMA 存取陷阱**: Write-Back 快取架構、DMA 接收（RX）呼叫 `Invalidate`（作廢快取舊值逼 CPU 讀 SRAM）、CPU 發送（TX）呼叫 `Clean`（把快取新值強行寫回 SRAM 給 DMA 搬）。
  2. **看門狗 WDT 餵狗架構陷阱**: 為什麼禁止在定時器中斷（Timer ISR）餵狗（避免主程式卡死卻持續打卡的「殭屍系統」）、視窗看門狗（WWDG）為什麼太早餵（Early Feed）也要重啟（抓出程式跑飛 PC Jumper 與跳關死迴圈）。

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Binary Number with Alternating Bits (#693 - Easy)
- **核心思維**:
  1. 使用者一發入魂寫法：取出最低位 `(n & 1)` 與次低位 `((n >> 1) & 1)` 比對，相異則右移，相同則回傳 false（0 ms AC）。
  2. IC 設計廠 $O(1)$ 純硬體無迴圈解法：
     - 若 $n = 5$ (`101`)，右移一位為 `010`。
     - 兩者 XOR：$a = n \oplus (n \gg 1) = 111$（相鄰位元必相異，XOR 之後全翻為 1）。
     - 驗證全為 1：`unsigned int a = (unsigned int)n ^ (n >> 1); return (a & (a + 1)) == 0;`。

### P2: Squares of a Sorted Array (#977 - Easy)
- **核心思維**:
  - 原陣列已排序，平方後最大值必在**最左端（負數絕對值大）**或**最右端（正數大）**。
  - 配置 `result` 陣列，指標 `curr` 從尾端倒著往前填入。
- **關鍵陷阱**:
  - 迴圈條件必須是 `while (left <= right)`（帶有等號）！若寫成 `<`，當指標收斂至中間最後一個元素（`left == right`）時會提前結束，導致 `result[0]` 未初始化噴出記憶體亂碼。
- **In-place 深度探討**:
  - 本題無法在保持 $O(N)$ 線性時間下做到 In-place，因為尾端覆寫會破壞尚未比對的右端資料；若原地平方後歸併會退化至 $O(N^2)$ 或 $O(N \log N)$。開闢 $O(N)$ 輔助空間為最優 Trade-off。

### P3: Isomorphic Strings (#205 - Easy)
- **核心思維**:
  - 必須是**雙向一對一映射（Bijection）**，不同字元不能共搶同一個對象。
  - 採用 ASCII 256 大小固定陣列 `map_s[256]` 與 `map_t[256]`，單週期查表。
- **關鍵陷阱**:
  - 判斷式必須是 `if (map_s[c1] == 0 && map_t[c2] == 0)`（雙方皆尚未登記），不可誤寫為 `!= 0`。
  - 使用 `(unsigned char)` 轉型防止 ASCII > 127 時轉為負數造成陣列越界。

---

## 🚗 自走車實務科普：感測中斷、濾波與編碼器 PID

### 1. MPU6050 非阻塞中斷採樣
- 禁止在主迴圈輪詢（Polling）I2C，避免阻塞馬達 PID 週期。
- 開啟硬體 Data-Ready 外部中斷腳位，新數據到達才進中斷通知主程式。

### 2. 馬達機械震動濾波
- 減速齒輪箱高頻振動（50~200Hz）干擾加速度計。
- 採用一階低通濾波器：$y[n] = 0.15 \cdot x[n] + 0.85 \cdot y[n-1]$，平滑噪聲。

### 3. 正交編碼器四倍頻與增量式 PID
- AB 相 90 度相位角：A 上升沿時看 B 的電平即可瞬間辨識正反轉。
- 捕捉 A/B 相的上升沿與下降沿（Timer Encoder Mode），解析度提升 400% 且 CPU 0 負載。
- 馬達控速採用增量式 PID，天然杜絕積分飽和（Anti-windup）。
- 堵轉監控（Stall Detection）：給予 PWM 卻連續 1 秒測不到速度，立即強制 PWM 歸零並報警。

---

## ⚔️ 0x10 韌體架構實戰情境題深度複盤

### 題目一：D-Cache 快取一致性（Cache Coherency）與 DMA
- **痛點原因**:
  - Write-Back 模式下，DMA 繞過 CPU 直接寫入實體 SRAM，但 CPU 的 D-Cache 依然保留舊資料。CPU 讀取時命中 Cache，讀出舊資料。
- **RX 與 TX 操作規範**:
  - **RX（DMA 寫入，CPU 讀取）**: 呼叫 **`Invalidate`**（作廢快取舊資料，逼 CPU 去實體 SRAM 抓取新資料）。
  - **TX（CPU 寫入，DMA 搬移）**: 呼叫 **`Clean` / Flush**（將 CPU 留在 D-Cache 裡的最新資料強行寫回實體 SRAM，供 DMA 搬移；若誤用 Invalidate 會將剛寫好的新資料當垃圾銷毀）。

### 題目二：看門狗（WDT）餵狗架構陷阱
- **禁止在 Timer ISR 餵狗**:
  - 定時器中斷硬體觸發、優先權高。若主迴圈馬達控制死鎖，ISR 依然定時餵狗，系統變成「大腦腦死、心跳仍在跳」的**植物人殭屍系統**。
  - 正解：多任務健康監控旗標，所有任務皆健康時由專門 Task 餵狗。
- **視窗看門狗（WWDG）太早餵重啟原理**:
  - 規定合法時間視窗（例如 30ms ~ 60ms）。
  - 若在 1ms 內過早餵狗，代表外部雜訊造成程式計數器（PC）發瘋跑飛（PC Jumper）、偷跳關略過中途安全檢查，或陷入極端死迴圈。WWDG 立即強制重啟挽救系統。
