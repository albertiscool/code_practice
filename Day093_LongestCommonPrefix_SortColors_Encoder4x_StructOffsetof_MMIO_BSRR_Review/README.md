# Day 93: Longest Common Prefix, Sort Colors, 編碼器四倍頻, 結構體 offsetof, MMIO 與 BSRR 原子操作

## 📚 學習大綱與實作總結

本日完成 2 道科技廠/IC 設計廠高頻白板程式題，並依新配置進行 3 道全真口試實務抽考：光電編碼器正交 4 倍頻解碼與 STM32 硬體定時器解耦、結構體記憶體對齊與 `offsetof` 空指標魔法、MMIO 暫存器映射、RMW 並發更新遺失與硬體 BSRR 原子操作。

---

## 💻 白板程式題實作與檢討

### 題目 1：最長公共前綴 (Longest Common Prefix - LeetCode #14)
- **檔案**：[`p1.c`](./p1.c)
- **難度**：🟢 Easy | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：縱向掃描法（Vertical Scanning），時間複雜度 $O(S)$（$S$ 為所有字元總數），額外空間複雜度 $O(1)$。
- **演算法核心邏輯**：
  - 以 `strs[0]` 為基準，逐一檢查每一行字元是否在其他所有字串 `strs[j]` 中完全吻合。
  - 一旦發現 `strs[j][i] == '\0'`（某字串已到底）或 `strs[j][i] != c`（字元不匹配），立刻結算前綴長度 `i`。
- **重要 C 語言字串盲點檢討**：
  - `malloc(i + 1)` 只有一個參數，傳兩個參數為 `calloc`。
  - C 語言字串結尾務必手動補上 `'\0'`（`result[i] = '\0'`），否則會印出未定義記憶體亂碼。
  - 若所有字串完全吻合，迴圈正常結束後應回傳 `strs[0]` 的拷貝，不可回傳空字串。

### 題目 2：顏色分類 / 荷蘭國旗問題 (Sort Colors - LeetCode #75)
- **檔案**：[`p2.c`](./p2.c)
- **難度**：🟡 Medium | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：Dijkstra 荷蘭國旗三指標原地排序（Dutch National Flag），時間複雜度 $O(N)$（單次走訪 One-pass），額外空間複雜度 $O(1)$。
- **三指標維護區間**：
  - `[0 ... p0 - 1]`：全為 0（紅色區）
  - `[p0 ... curr - 1]`：全為 1（白色區）
  - `[curr ... p2]`：未處理元素
  - `[p2 + 1 ... n - 1]`：全為 2（藍色區）
- **關鍵交換原則**：
  - `nums[curr] == 0`：與 `nums[p0]` 交換，`p0++`，`curr++`。
  - `nums[curr] == 1`：已在白色區，直接 `curr++`。
  - `nums[curr] == 2`：與 `nums[p2]` 交換，`p2--`；**⚠️ 注意：此時 `curr` 必須停在原地！** 因為從後面換過來的新數字未經檢驗，必須留在原地下一輪重新判斷。
- **原始盲點檢討**：原地排序必須使用 `swap` 互相交換，若直接賦值 `nums[p2--] = nums[i]` 會將原始資料抹煞覆蓋。

---

## 🎙️ 全真面試口試抽考精要

### 題 1：自走車專案實務 - 光電編碼器正交相位差、4倍頻與硬體中斷負載

#### 1. 正交訊號與方向判定
- **正交特徵**：A 相與 B 相為相位差 $90^\circ$ 的方波。
- **方向判定**：當 A 相發生**上升邊緣（Rising Edge）**時讀取 B 相電位：
  - 若 `B == LOW`，表示 A 相領先 B 相 $\to$ **正轉（Forward）**，計數累加。
  - 若 `B == HIGH`，表示 B 相領先 A 相 $\to$ **反轉（Backward）**，計數遞減。

#### 2. 4 倍頻解碼與 STM32 硬體定時器優化
- **4 倍頻原理**：將 A 相與 B 相皆設定為雙邊緣觸發中斷（`CHANGE`），每個電氣週期捕捉「A上升、B上升、A下降、B下降」共 4 個跳變邊緣，使 500 PPR 編碼器免費提升為每圈 2000 個脈衝解析度。
- **CPU 負載危機與 STM32 解方**：
  - 8 位元 MCU（ATmega328P @ 16MHz）在高速旋轉時每秒需處理數萬至十幾萬次中斷，造成 CPU 100% 飽和卡死。
  - **STM32 編碼器介面模式（Encoder Mode）**：通用定時器（TIM2/TIM3/TIM4）硬體內部具備正交解碼邏輯，直接由硬體自動計數至暫存器 `TIMx->CNT`，**0% CPU 佔用、零中斷**，CPU 僅需於控制週期讀取暫存器即可。

---

### 題 2：0x10 C 語言底層核心 - 結構體 Memory Alignment、Padding 與 `offsetof` 巨集

#### 1. 結構體記憶體對齊計算（32-bit 系統）
```c
struct DataA { char a; int b; short c; };
// a: 1 byte + 3 bytes padding (對齊 4)
// b: 4 bytes (offset 4)
// c: 2 bytes (offset 8) + 2 bytes tail padding (補齊整體最大成員 4 的倍數)
// sizeof(struct DataA) = 1 + 3 + 4 + 2 + 2 = 12 Bytes

struct DataB { int b; short c; char a; };
// b: 4 bytes (offset 0)
// c: 2 bytes (offset 4)
// a: 1 byte  (offset 6) + 1 byte tail padding (補齊整體最大成員 4 的倍數)
// sizeof(struct DataB) = 4 + 2 + 1 + 1 = 8 Bytes
```

#### 2. `offsetof` 巨集的底層實作原理
```c
#define offsetof(TYPE, MEMBER) ((size_t)&(((TYPE *)0)->MEMBER))
```
- **原理**：將整數 `0` 強制轉型為指向結構體的指標 `(TYPE *)0`，並存取成員取其位址。
- **為什麼不會 HardFault？** 因為外層加上了 `&` 取位址，`&` 與 `->` 的解參考互相抵消，編譯器在編譯期（Compile-time）只依據型別藍圖換算位移量，根本沒有在執行期存取位址 0 的記憶體。
- **延伸**：Linux 核心以此實作 `container_of` 巨集，利用成員位址反推父結構體位址。

---

### 題 3：🔥 0x10 經典嵌入式 C 語言與韌體面試精選題複習 - MMIO 暫存器映射與 RMW 競態條件

#### 1. MMIO 巨集定義與三要素
```c
#define UART_CR1 (*(volatile uint32_t *)0x40021000)
```
- `(uint32_t *)`：將常數轉為 32 位元記憶體位址指標。
- `volatile`：禁止編譯器快取暫存器數值或刪除指令，強制每次讀寫均走硬體匯流排。
- 最外層 `*`：解參考為左值（L-value），便於直接賦值與讀取。

#### 2. 位元清除操作
```c
UART_CR1 &= ~(1 << 4); // 清除 Bit 4，其餘位元保持不變
```

#### 3. Read-Modify-Write (RMW) 競態條件與 BSRR 解決方案
- **更新遺失災難（Lost Update）**：
  - 主迴圈執行 `UART_CR1 |= (1 << 3);` 拆解為 Read(讀取舊值) $\to$ Modify(改Bit 3) $\to$ Write(寫回)。
  - 若在 Read 之後剛好發生中斷，中斷中設定了 `Bit 6 = 1` 並寫回記憶體；中斷返回後，主迴圈將手裡過時的舊值寫回記憶體，硬生生把中斷寫入的 `Bit 6` 覆蓋抹殺為 `0`！
- **STM32 BSRR 硬體原子性根除方案**：
  - BSRR 為 32 位元只寫（Write-Only）暫存器，低 16 位寫 1 代表置位（Set），高 16 位寫 1 代表復位（Reset），寫 0 代表「無動作」。
  - 軟體只需一條 `STR` 單週期指令寫入對應位元（如 `GPIOA->BSRR = (1 << 3);`），無需 Read 也無需 Modify，從硬體層面徹底消滅 RMW 競態。
