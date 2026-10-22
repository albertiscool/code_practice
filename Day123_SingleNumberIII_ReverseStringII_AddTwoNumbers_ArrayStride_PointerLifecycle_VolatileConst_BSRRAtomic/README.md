# Day 123 研發替代役 / 碩士預聘實戰訓練

## 📋 本日訓練大綱
* **Part 1 白板程式題（HackMD 考古題高頻變形，無提示純白板實戰）**
  * **P1**: LeetCode #260 - Single Number III（XOR 兩數差異、LSB 分組異或、有號整數防溢位）
  * **P2**: LeetCode #541 - Reverse String II（字串週期性區間原地翻轉、邊界條件處理）
  * **P3**: LeetCode #2 - Add Two Numbers（單向鏈結串列大數相加、維護 carry 進位、虛擬頭節點 dummy）
* **Part 2 面試經典致命陷阱題**
  * **Trap 1**: 陣列退化、跨距步長與指標位移（`array` vs `&array`、`&array + 1` 跨距、`*(ptr - 1)`、十六進位換算）
  * **Trap 2**: 傳值 (Call-by-Value) 與指標參數生命週期（`get_memory_A` 記憶體洩漏與空指標 Crash、`get_memory_B` 傳雙指標修改呼叫端變數）
* **Part 3 基本 0x10 韌體與作業系統觀念題**
  * **Q1**: `const` 與指標排列組合 ＆ 韌體中 `volatile const` 唯讀硬體狀態暫存器語意
  * **Q2**: 暫存器位元操作巨集 ＆ RMW (Read-Modify-Write) Race Condition 與 BSRR 原子性寫入硬體機制
* **自走車專案實務科普（系統廠 / IC 設計廠面試亮點）**
  * 超音波避障感測器的 CPU 致命詛咒（阻塞式 `pulseIn`）與硬體定時器輸入捕獲（Timer Input Capture）零阻塞架構

---

## 💻 Part 1 白板程式題總結

### 1. P1: LeetCode #260 - Single Number III
* **檔案路徑**：[Day123/p1.c](file:///c:/Users/a0907/Desktop/程式訓練/Day123/p1.c)
* **核心觀念**：
  * 第一次遍歷：所有元素全部 XOR，成對元素消除，得到 $x = a \oplus b$。
  * 提取 LSB 遮罩：$a \neq b \implies x \neq 0$，利用 `unsigned int diff = (unsigned int)x & -(unsigned int)x` 抓出相異的最低位。
  * 第二次遍歷：依據 `num & diff` 分成兩組各自 XOR，成對元素自動在同組消掉，分離出 $a$ 與 $b$。
  * 韌體防禦亮點：強制轉型為 `unsigned int`，防止 `INT_MIN` 取負號觸發 Signed Overflow 未定義行為。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 35/35 測試案例。

### 2. P2: LeetCode #541 - Reverse String II
* **檔案路徑**：[Day123/p2.c](file:///c:/Users/a0907/Desktop/程式訓練/Day123/p2.c)
* **核心觀念**：
  * 每 $2k$ 個字元翻轉前 $k$ 個字元。
  * 雙指標向中間收斂翻轉，注意指標移動 `pre++` 與 `slow--` 避免無窮迴圈。
  * 邊界計算：以本區間起點 `pre` 計算下個區間起點 `fast = pre + 2 * k`，末端剩餘少於 $k$ 個時右界為 `pre + len - 1`。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 60/60 測試案例。

### 3. P3: LeetCode #2 - Add Two Numbers
* **檔案路徑**：[Day123/p3.c](file:///c:/Users/a0907/Desktop/程式訓練/Day123/p3.c)
* **核心觀念**：
  * 禁忌：不能轉成 `int` 相加！節點數高達 100 位會引發嚴重大數溢位（Integer Overflow）。
  * 題目輸入為逆序存儲（Head 即為個位數），直接從頭遍歷模擬大數加法，維持 `carry` 進位。
  * 迴圈條件 `l1 != NULL || l2 != NULL || carry != 0`，優雅處理不等長串列與最高位進位。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 1569/1569 測試案例。

---

## 🎯 Part 2 面試陷阱題解析

### Trap 1: 陣列退化、跨距步長與指標位移
```c
int array[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
int *ptr = (int *)(&array + 1);
```
1. **型別與位址**：
   * `array` 退化為 `int*`（步長為 4 Bytes）。
   * `&array` 型別為 `int (*)[10]`（指向長度為 10 之陣列的指標，步長為 $10 \times 4 = 40$ Bytes）。
   * 兩者數值相同，皆為 `0x2000`。
2. **`&array + 1` 記憶體位址**：
   * 向前跨越 40 Bytes ($0x28$)，位址為 `0x2000 + 0x28 = ` **`0x2028`**（非 0x2040）。
3. **`*(ptr - 1)`**：
   * `ptr` 強制轉回 `int*`，`-1` 倒退 4 Bytes 回到 `array[9]`，解引用得到 **`100`**。
4. **`*array + 1` 與 `*(array + 1)`**：
   * `*array + 1 = 10 + 1 = 11`。
   * `*(array + 1) = array[1] = 20`。

### Trap 2: 傳值 (Call-by-Value) 與指標生命週期
1. **`get_memory_A` 致命錯誤**：
   * C 語言傳遞指標仍為傳值（Pass by Value），`p` 是 Stack Frame 上的複本。
   * `malloc` 位址賦值給區域變數 `p`，函式返回後 `p` 銷毀，造成 **Memory Leak**。
   * 主呼叫端 `strA` 仍為 `NULL`，執行 `strcpy(NULL, "Hello")` 觸發 **Segmentation Fault / HardFault 崩潰**。
2. **`get_memory_B` 成功本質**：
   * 傳入雙指標 `&strB`（主呼叫端變數本身的位址）。
   * 函式內透過解引用 `*p = malloc(...)`，直接改寫主呼叫端 Stack 上的變數值。

---

## 🧠 Part 3 基本 0x10 韌體與作業系統觀念題

### Q1: `const` 與指標 ＆ 韌體中的 `volatile const`
1. **指標修飾**：
   * `const int *p1`, `int const *p2`：指向常數的指標，內容不可改。
   * `int * const p3`：常數指標，位址不可改。
   * `const int * const p4`：指向常數的常數指標，位址與內容皆不可改。
2. **`volatile const` 奇特組合**：
   * **`volatile`**：告訴編譯器硬體外部隨時會修改此暫存器（如狀態暫存器、計時器），每次讀取禁止暫存器快取，必須從硬體實體位址重新 Fetch。
   * **`const`**：告訴編譯器軟體程式只有唯讀權限，禁止寫入（寫入直接報編譯錯誤）。
   * 合體即為：**硬體唯讀狀態暫存器（Hardware Read-Only Status Register）**。

### Q2: 暫存器位元操作 ＆ RMW 競爭條件 (Race Condition) 與 BSRR
1. **Bit 操作標準寫法**：
   ```c
   reg |= (1 << 5);     // 第 5 個 bit 設為 1
   reg &= ~(1 << 18);   // 第 18 個 bit 清為 0
   ```
2. **RMW 危機與 BSRR 硬體原子性**：
   * `reg |= (1 << 5)` 在組合語言中拆為：`LDR`（讀取）$\to$ `ORR`（修改）$\to$ `STR`（寫入）。
   * 若剛讀出後被 ISR 中斷，ISR 修改了 `reg` 的其他 bit，中斷返回後主程式將舊值寫回，會**完全抹殺 ISR 的修改**！
   * **BSRR（Bit Set/Reset Register）硬體解法**：BSRR 為純寫入暫存器，寫 0 無作用，寫 1 由晶片內部電路單週期原子觸發設置/清除，完全不需要讀取舊值，徹底根除 Race Condition。

---

## 🚗 自走車專案實務科普：超音波避障阻塞詛咒與硬體定時器輸入捕獲

### 1. 阻塞式 `pulseIn` 的致命災難
* **現象**：
  許多學生專案直接呼叫 Arduino `pulseIn(ECHO_PIN, HIGH)`。
  音速約 340 m/s，若障礙物較遠或反射波遺失，`pulseIn` 預設超時高達 **30 ~ 50 ms**！
* **後果**：
  在這 30~50 ms 內，CPU 完全死鎖（Busy Waiting）。自走車 100 Hz (10 ms) 的 PID 馬達速度閉迴路控制直接跳過 3~5 次運算，輪子劇烈抽蓄失控，甚至直接撞牆！

### 2. 工業級解法：硬體定時器輸入捕獲（Timer Input Capture）
* **硬體架構**：
  * 將 Echo 訊號接至 MCU 定時器具備輸入捕獲（Input Capture）功能的 GPIO。
  * 上升沿觸發：硬體計數器自動鎖存當前計數值至 `CCR1`。
  * 下降沿觸發：硬體計數器自動鎖存當前計數值至 `CCR2`。
* **優勢**：
  * $\Delta t = \text{CCR2} - \text{CCR1}$ 完全由硬體底層電路量測，**CPU 開銷為 0%，零阻塞**！
  * 馬達控制迴圈與通訊執行緒可 100% 準時維持定時週期。
