# Day 42: 總複習 Part 2 - 記憶體對齊 (Struct Padding)、Volatile 防護與 Bitwise 操作

今天是 10 天特訓總複習的第二天，我們深入探討了 64 位元系統下 Struct Padding 記憶體對齊算術、GCC 編譯器 `-O2` 最佳化對未加 `volatile` 指針造成的死迴圈陷阱，以及硬體暫存器層級的 Bitwise 遮罩控制。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【觀念計算題：Struct Padding 與記憶體對齊】
* **原問題**：比較 `SensorDataA` 與 `SensorDataB` 於 64 位元 Linux 下的 `sizeof` 大小。
* **解析與解答**：
  1. **`sizeof(SensorDataA)` = 24 Bytes**：`flag` (1B) 後補 7B Padding 以讓 `timestamp` 對齊 8B 邊界，結尾 `id` (2B) 後再補 6B Padding 以滿足整體為最大成員 8 的倍數 ($1+7+8+2+6=24$)。
  2. **`sizeof(SensorDataB)` = 16 Bytes (優化)**：`flag` (1B) 後補 1B Padding 讓 `id` (2B) 對齊偶數位址，隨後補 4B Padding 讓 `timestamp` (8B) 對齊 8 的倍數 ($1+1+2+4+8=16$)。
  3. **省記憶體原理**：成員宣告由小排到大能大幅減少中間為對齊自然邊界產生的 Padding 盲區。

### 題目 2【硬體陷阱題：`volatile` 與編譯器最佳化】
* **原問題**：`while ((*status_reg & 0x01) == 0)` 未加 `volatile` 時，GCC `-O2` 最佳化會引發什麼 Bug？
* **解析與解答**：
  1. **死迴圈災難**：編譯器認定迴圈內部無人修改 `*status_reg`，會將記憶體值僅讀取一次快取至 CPU Register (`R0`) 中。後續 `while` 僅重複檢查 Register `R0`，當實體硬體/RAM 更新該位址時，CPU 根本讀不到最新值，當場卡死。
  2. **修正宣告**：`volatile uint32_t *status_reg = (volatile uint32_t *)0x40011000;` 強迫 CPU 每次循環皆重新去 RAM/MMIO 抓取最新資料。

### 題目 3【硬體低階題：Bitwise 遮罩寫入】
* **原問題**：針對 32-bit 控制暫存器 `CR_REG` 執行置位 (Bit 4)、清零 (Bit 7) 與翻轉 (Bit 2)。
* **解析與解答**：
  1. **置位 (Set Bit 4)**：`CR_REG |= (1 << 4);` （利用 Bitwise OR 設 1）
  2. **清零 (Clear Bit 7)**：`CR_REG &= ~(1 << 7);` （利用 Bitwise AND 搭配的反轉 NOT 設 0）
  3. **翻轉 (Toggle Bit 2)**：`CR_REG ^= (1 << 2);` （利用 Bitwise XOR 進行 0/1 翻轉）

---

## ⚙️ 核心語法複習：Bitwise 暫存器三套路

```c
// 1. 置位 (Set Bit n to 1)
REG |= (1 << n);

// 2. 清零 (Clear Bit n to 0)
REG &= ~(1 << n);

// 3. 翻轉 (Toggle Bit n)
REG ^= (1 << n);
```
