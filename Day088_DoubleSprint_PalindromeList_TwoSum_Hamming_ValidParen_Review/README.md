# Day 88: 雙倍戰力特訓 (迴文鏈結串列/兩數之和/位元操作/有效括號) 與微處理機核心硬體周邊 (I2C卡死救援/大小端探測/ADC採樣定理與輸入阻抗)

今天啟動「雙倍戰力補給包（Day 88 雙日特訓）」，維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」最高強度節奏。一口氣攻克了 4 大高頻白板題（迴文鏈結串列 [快慢指針找中點 + 原地反轉後半段 + 同步比對]、兩數之和 II [有序陣列首尾雙指針與 1-indexed 規範]、位元 1 的個數 [神級 Brian Kernighan's Algorithm `n &= (n - 1)`]、有效的括號 [Stack 堆疊模擬與預存預期右括號神技]），並在口試中深入拆解了 I2C 總線鎖死與 9-Clock 軟體救援 SOP、C 語言大小端（Endianness）記憶體探測與位元反轉巨集、以及 ADC 奈奎斯特採樣定理與採樣保持電容 $C_{\text{sample}}$ 阻抗匹配電路！

---

## 🏆 今日 4 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【迴文鏈結串列 (Palindrome Linked List - LeetCode #234)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠鏈結串列綜合考題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day88_DoubleSprint_PalindromeList_TwoSum_Hamming_ValidParen_Review/p1.c)
* **演算法**：快慢雙指針 ＋ 原地反轉後半段，時間 $O(N)$，空間 $O(1)$。
* **核心三步法**：
  1. 快慢指針找到鏈結串列中點：`while (fast != NULL && fast->next != NULL)`。
  2. 原地反轉後半段鏈結串列（從 `slow` 開始，`prev` 初始值為 `NULL`）。
  3. 前半段（`head`）與後半段（`prev`）同步走訪比對數值。
* **⚠️ 實作盲點複習**：
  * 快慢指針條件切勿誤寫成 `||`（或），否則當 `fast->next == NULL` 時會因為短路求值導致 `fast->next->next` 存取空指針觸發 Segfault！
  * 反轉時 `prev` 必須初始化為 `NULL`，若令 `prev = slow` 會導致前後互相指向造成鏈結串列成環（Cycle）。

---

### 2. 題目 2【兩數之和 II - 輸入有序陣列 (Two Sum II - LeetCode #167)】
* **難度**：🟡 **Medium** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠雙指針必考題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day88_DoubleSprint_PalindromeList_TwoSum_Hamming_ValidParen_Review/p2.c)
* **演算法**：首尾對撞雙指針（Two Pointers），時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 左指針 `head = 0`，右指針 `tail = numbersSize - 1`。
  * 若 `sum == target`：回傳 `[head + 1, tail + 1]`（注意題目要求 **1-indexed**）。
  * 若 `sum > target`：`tail--`（總和太大，右指針左移減小和）。
  * 若 `sum < target`：`head++`（總和太小，左指針右移增加和）。

---

### 3. 題目 3【位元 1 的個數 (Number of 1 Bits - LeetCode #191)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (IC 設計廠面試必考位元操作)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day88_DoubleSprint_PalindromeList_TwoSum_Hamming_ValidParen_Review/p3.c)
* **演算法**：Brian Kernighan's Algorithm，時間 $O(K)$（$K$ 為 1 的個數），空間 $O(1)$。
* **核心邏輯**：
  * 利用 `n &= (n - 1)` 可以在單次位元運算中**直接抹除二進制中最右邊的第一顆 '1'**！
  * 迴圈執行次數只等於 1 的數量，遠優於固定右移 32 次的暴力解。

---

### 4. 題目 4【有效的括號 (Valid Parentheses - LeetCode #20)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大系統廠與 IC 廠 Stack 必考題)
* **原始碼檔案**：[`p4.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day88_DoubleSprint_PalindromeList_TwoSum_Hamming_ValidParen_Review/p4.c)
* **演算法**：Stack 堆疊模擬，時間 $O(N)$，空間 $O(N)$。
* **核心神技（預存預期右括號）**：
  * 遇到 `(` 入棧 `)`；遇到 `[` 入棧 `]`；遇到 `{` 入棧 `}`。
  * 遇到任何右括號，直接統一比對：`if (p == 0 || stack[--p] != s[i]) return false;`。
  * **終止清空驗證**：結尾必須驗證 `return p == 0;`，防止未閉合括號（如 `"(("`）被誤判為合法。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自走車專案實務：I2C 總線卡死 (Bus Lockup) 與 9-Clock 軟體救援】
* **卡死原因**：Master 在通訊途中遇雜訊復位，Slave（MPU6050）仍停在傳送中且恰好送出 0，將 SDA 死死拉在低電位。Master 硬體 I2C 因偵測到 Bus Busy 拒絕動作。
* **標準 4 步脫困 SOP**：
  1. 停用硬體 I2C，將 SCL 與 SDA 引腳切換為一般 GPIO（Open-Drain 模式）。
  2. 軟體模擬（Bit-Banging）連送 9 個 SCL 時脈脈衝，讓從機把剩餘 bits 吐完並釋放 SDA。
  3. 主機手動補送一個標準 I2C STOP 條件（SCL 高時 SDA 由低變高），強制復位 Slave 狀態機。
  4. 重新致能硬體 I2C 外設（`I2C_Init()`），總線重啟。

---

### 🗣️ 題 2【0x10 C / 底層記憶體：大小端 (Endianness) 探測與 32-bit 位元反轉巨集】
* **原始碼檔案**：[`endian_check.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day88_DoubleSprint_PalindromeList_TwoSum_Hamming_ValidParen_Review/endian_check.c)
* **Runtime 大小端探測（利用 `union`）**：
  * 小端序（Little-Endian）：低位元組（LSB）放在低位址（Offset 0）。
  * 若令 `A.b = 0x10;`，當 `A.c == 0x10` 則為小端序；若 `A.c == 0x00` 則為大端序。
* **32 位元大小端反轉巨集（Bitwise Shifts & Masks）**：
  ```c
  #define SWAP32(n) ( (((uint32_t)(n) & 0x000000FFU) << 24) | \
                      (((uint32_t)(n) & 0x0000FF00U) <<  8) | \
                      (((uint32_t)(n) & 0x00FF0000U) >>  8) | \
                      (((uint32_t)(n) & 0xFF000000U) >> 24) )
  ```
  * 注意：必須使用按位或 `|`（非邏輯或 `||`），並在常數後加 `U` 防止符號擴展算術右移。

---

### 🗣️ 題 3【🎯 微處理機核心硬體周邊：ADC 採樣定理、阻抗匹配與採樣電容】
* **Nyquist 採樣定理**：取樣頻率至少要是訊號最高頻率的 2 倍（$f_s \ge 2 f_{\max}$）。若採樣率不足，高頻訊號會錯誤折疊成虛假的低頻訊號，引發**混疊現象（Aliasing）**（如電影車輪倒轉效應）。前端需加抗混疊低通濾波器（AAF）。
* **採樣保持電容（$C_{\text{sample}}$）與輸入阻抗**：
  * ADC 入口有開關與採樣電容（$5 \sim 10\text{ pF}$），需在開關閉合的微秒級時間內透過外部電阻 $R$ 充飽（$\tau = RC$）。
  * 若外部阻抗過大（如 $100\,\text{k}\Omega$ 分壓），充電太慢導致電容未充飽開關即斷開，量測電壓大幅縮水失真。
  * **硬體解法**：在 ADC 引腳對地並聯外部濾波電容提供瞬間電荷，或加入**電壓隨耦器（Op-Amp Buffer）**提供近乎 $0\,\Omega$ 的超低輸出阻抗。
