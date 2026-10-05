# Day 51: 韌體工程師 0x10 面試精選特訓 Part 2 (巨集括號陷阱、Bitwise 位元操作、Stack vs Heap 記憶體防護與端序對調)

今天是 0x10 面試經典題目的第二階段特訓，我們深入複習了 C 語言巨集展開括號陷阱、單晶片暫存器 Bitwise 設值與安全判斷寫法、Stack 堆疊溢位與 Heap 記憶體洩漏機制、以及 16-bit Byte Swap 端序轉換與 POSIX 網路名稱縮寫。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 4【巨集與位元運算：Macro Trap & Bit Operations】
* **原問題**：`#define SQUARE(x) (x * x)` 傳入 `3 + 1` 結果為何？Bit 4 設為 1 與檢查 Bit 4 之單行寫法？
* **解析與解答**：
  1. **巨集純文字展開陷阱**：`SQUARE(3 + 1)` 會展開為 `3 + 1 * 3 + 1`，依先乘除後加減算得 `7`。安全寫法為每一處參數皆加括號：`#define SQUARE(x) ((x) * (x))`。
  2. **Bit Set 設為 1**：`reg |= (1 << 4);` （利用 Bitwise OR 設定 Bit 4 為 1）。
  3. **Bit Check 安全檢查**：
     * ❌ 誤寫 `(reg & (1 << 4)) == 1` 會因 `0x10 (16) != 1` 導致永遠回傳 0。
     * ✅ 正確寫法 A (位移至第 0 位)：`((reg >> 4) & 1)`
     * ✅ 正確寫法 B (不等於 0 檢查)：`((reg & (1 << 4)) != 0)`

### 題目 5【記憶體配置與崩潰：Stack Overflow & Local Pointer】
* **原問題**：回傳區域陣列指標下場？函式內宣告 1MB 區域陣列下場？Heap 配置未釋放會怎樣？
* **解析與解答**：
  1. **Dangling Pointer 懸空指標**：區域陣列位於 Stack 堆疊區，函式 return 後其 Stack Frame 即遭釋放。回傳該位址會存取到隨時被覆寫的垃圾資料甚至死機。
  2. **Stack Overflow 堆疊溢位**：MCU/RTOS Task 堆疊極小 (1KB~8KB)。宣告 1MB 區域陣列會導致堆疊指標打破邊界踩壞周遭變數，觸發當機。應呼叫 `malloc(1024 * 1024)` 宣告於 Heap 記憶體。
  3. **Heap 記憶體洩漏 (Memory Leak)**：MCU/RTOS 無 OS 進程關閉回收機制，未呼叫 `free()` 的 Heap 記憶體會永遠標記為使用中，長期累積最終導致 `malloc()` 回傳 NULL 系統崩潰。

### 題目 6【位元組轉換與端序：Byte Swapping】
* **原問題**：16-bit 數據 `0x1234` 轉 `0x3412` 的單行寫法？`htons` / `ntohs` 4 個縮寫字母含意？
* **解析與解答**：
  1. **16-bit Byte Swap 寫法**：
     `val = ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);`
  2. **POSIX 縮寫含意**：
     * `h` ➡️ **Host** (主機端序，本地 CPU 小端序)
     * `n` ➡️ **Network** (網路端序，TCP/IP 統一規範 **大端序 Big-Endian**)
     * `to` ➡️ **to** (轉換為)
     * `s` / `l` ➡️ **short** (16-bit 短整數) / **long** (32-bit 長整數)
     * `htons()` 即 **Host to Network Short** (主機端序轉網路大端序 16-bit)。
