# Day 90: Maximum Subarray, Reverse Bits, 示波器 vs 邏輯分析儀, Macro vs Inline, UART 16x Oversampling

## 📚 學習大綱與實作總結

本日完成 2 道科技廠/IC 設計廠高頻白板程式題，並深入探討 3 道嵌入式韌體核心口試考題：訊號完整性除錯、C 語言展開副作用與 UART 接收端硬體取樣架構。

---

## 💻 白板程式題實作與檢討

### 題目 1：最大子陣列和 (Maximum Subarray - LeetCode #53)
- **檔案**：[`p1.c`](./p1.c)
- **難度**：🟡 Medium | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：Kadane 演算法（動態規劃 / 貪婪思維），時間複雜度 $O(N)$，空間複雜度 $O(1)$。
- **重要邊界陷阱**：
  - 若將 `max_sum` 初始化為 `0`，當陣列全是負數（如 `[-3, -2, -5, -1]`）時，任何累加和大於 0 的條件都不會成立，導致最終錯誤回傳 `0`。
  - **正解原則**：將 `max_sum` 初始設為 `nums[0]`，迴圈中每次累加 `curr_sum += nums[i]` 後立即更新 `max_sum`，只要 `curr_sum < 0` 就重置為 `0`（因為負數累加和對後續子陣列只會產生拖累）。

### 題目 2：顛倒二進位位元 (Reverse Bits - LeetCode #190)
- **檔案**：[`p2.c`](./p2.c)
- **難度**：🟢 Easy | **出題頻率**：🔥🔥🔥🔥🔥
- **核心操作**：32 位元位元反轉（Bitwise Manipulation）。
- **位移順序陷阱（Off-by-one Shift Trap）**：
  - 若寫法為「先累加 `(n & 1)` 再 `result <<= 1`」，在最後一次迴圈（$i=31$）時會將最後一個 bit 多左移 1 次，並將最初放進來的 bit 左移 32 次造成 32-bit 溢位（輸出為預期值的 2 倍或溢位為 0）。
  - **標準寫法**：先左移騰出空間，再填入當前最低位：
    ```c
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    ```

---

## 🎙️ 全真面試口試抽考精要

### 題 1：自走車與硬體訊號除錯實務 - 示波器 vs 邏輯分析儀 & I2C 訊號完整性

#### 1. 示波器 vs 邏輯分析儀的分工情境
* **示波器 (Oscilloscope)**：
  * **時機**：懷疑有**硬體物理層問題（Signal Integrity / 訊號完整性）**時使用。
  * **特徵**：還原真實「連續電壓波形」，量測上升/下降時間（Rise/Fall Time）、過衝/振鈴（Overshoot/Ringing）、電源漣波（Ripple）、高頻毛刺（Glitch）以及訊號是否跨過 $V_{IH}/V_{IL}$ 邏輯門檻。
* **邏輯分析儀 (Logic Analyzer)**：
  * **時機**：硬體物理層訊號正常，需要進行**多通道、長時間的軟體協議邏輯除錯**時使用。
  * **特徵**：取樣數位 0 與 1，具備 8~32 條以上通道與深層記憶體，自動解碼 I2C, SPI, UART, CAN, PCIe, NAND Flash 等協定封包。

#### 2. I2C 上升邊緣緩慢成因與上拉電阻調整
* **物理成因**：
  * I2C 為開漏極（Open-Drain）架構，拉高電位全靠外部上拉電阻 $R_p$ 對匯流排寄生電容 $C_b$（走線、接腳、晶片電容）進行 $RC$ 充電。
  * 充電時間常數 $\tau = R_p \times C_b$，$t_r \approx 0.8473 \times R_p \times C_b$。$R_p$ 過大或負載過多會使波形爬升過慢，無法在時脈週期內達到 $V_{IH}$（$0.7 V_{DD}$），造成通訊異常或 NACK。
* **電阻選用限制**：
  * **調小電阻**：一般 $3.3\text{V}$ 系統下推薦選用 **$2.2\text{k}\Omega \sim 4.7\text{k}\Omega$** 以加快上升時間。
  * **不能過小（$< 1\text{k}\Omega$）原因**：
    1. **$V_{OL}$ 失效與燒毀風險**：I2C 規定晶片下拉時的最大沉入電流（Sink Current）通常為 $3\text{mA}$。若電阻過小，$I = V_{DD}/R_p$ 過大導致晶片內部 NMOS 無法將電壓拉低至 $V_{OL} \le 0.4\text{V}$ 門檻，甚至可能過熱燒毀內部開關管。
    2. **靜態功耗暴增**：訊號為 Low 期間會持續消耗過多靜態電流。

---

### 題 2：0x10 C 語言底層核心 - `#define` 巨集 vs `static inline` 函式

| 比較維度 | `#define` 巨集 (Macro) | `static inline` 內聯函式 |
| :--- | :--- | :--- |
| **處理階段** | **前處理器 (Preprocessor)**，純字串文字代換 | **編譯器 (Compiler)**，完整語意解析與最佳化 |
| **型別檢查** | ❌ 無型別安全（Type Unsafe） |  強型別檢查（Type Safe） |
| **除錯難度** | ❌ GDB 無法下斷點、無法 Step-in |  保留除錯符號，可正常設斷點與追蹤 Call Stack |
| **展開決策** | ❌ 無條件強制展開，易造成 Code Bloat |  編譯器智慧評估，過大時自動轉為普通呼叫 |
| **執行效能** | 零函式呼叫開銷 | `-O2/-O3` 最佳化後完全消除函式呼叫開銷 |

#### 展開副作用分析
```c
int a = 3;
int res1 = SQUARE(a++); // 展開為 ((a++) * (a++)) -> a 遞增兩次變 5，res1 = 12 (UB)

int b = 3;
int res2 = square(b++); // 傳入 3，b 遞增一次變 4，函式內算 3*3 -> res2 = 9
```
* **結論**：現代嵌入式開發規範（如 MISRA C）強烈建議以 `static inline` 取代具運算邏輯的 Function-like Macro。

---

### 題 3：微處理機核心硬體周邊 - UART Baud Rate Generator 與 16x Oversampling

#### 1. 波特率分頻係數計算（$f_{\text{PCLK}} = 16\text{ MHz}$, Baud Rate $= 115200\text{ bps}$, 16x Oversampling）
* **公式**：
  $$\text{USARTDIV} = \frac{f_{\text{PCLK}}}{16 \times \text{Baud Rate}} = \frac{16,000,000}{16 \times 115200} \approx \mathbf{8.680555...}$$
* **暫存器分頻設定（以 STM32 `USART_BRR` 為例）**：
  * **Mantissa（整數部）**：$\lfloor 8.680555 \rfloor = \mathbf{8} = \text{0x8}$
  * **Fraction（4-bit 小數部）**：$\text{round}(0.680555 \times 16) = \text{round}(10.8888) = \mathbf{11} = \text{0xB}$
  * **暫存器寫入值**：`USART_BRR = (8 << 4) | 11 = 0x8B`（十進位 $139$）
  * **實際 Baud Rate 與誤差**：$115107.9\text{ bps}$，誤差率約 **$0.08\%$**（遠小於 $\pm 2\%$ 容許門檻）。

#### 2. 16x Oversampling 與 7, 8, 9 點多數決投票（Majority Voting）機制
* **採樣點選在 7, 8, 9 點的原因**：
  * 避開 Bit 開頭（0~2 點）與結尾（14~15 點）的邊緣訊號過渡期（Rise/Fall Time）。
  * 位於訊號最穩定的中心點，擁有最大的**眼圖張開度（Max Eye Opening）**與時序裕度（Timing Margin），最大化容忍雙方時脈漂移（Clock Drift / Jitter）。
* **多數決投票（3-sample Majority Voting）抗雜訊機制**：
  * 取樣 7, 8, 9 點若為 $\{1, 0, 1\}$，以 2 比 1 多數決判定為 `1`，並觸發硬體 Noise Flag（NE 旗標）。
  * 能有效濾除馬達換向或電源干擾造成的單一瞬態毛刺（Glitch），大幅提升通訊強韌度。
