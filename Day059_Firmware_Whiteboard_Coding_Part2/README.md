# Day 59: 韌體工程師白板題特訓 Part 2 (大小端 Endianness 檢查、32-bit Swap Endianness、Union 記憶體共用與指標優先順序陷阱)

今天是韌體工程師白板手寫程式碼 (Whiteboard Coding) 特訓第二期。我們著重於硬體記憶體架構 (Endianness)、位元組反轉 (Byte Swap)、`union` 實體記憶體共享特性，以及指標運算優先順序陷阱特訓。

---

## 📝 實戰白板題詳細解析與 Code Review

### 題目 3【大小端 (Endianness) 檢查與 32 位元 Byte 反轉】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day59_Firmware_Whiteboard_Coding_Part2/p3.c)

#### 任務 1：檢查當前系統是 Little-Endian 還是 Big-Endian
* **指標強制轉型解法**：
  ```c
  bool is_little_endian(void) {
      uint32_t x = 0x0001;
      uint8_t *y = (uint8_t *)&x; // 必須強制轉型為 (uint8_t*)，才能只讀取最低位址的第 1 個 Byte!
      return (*y == 1);
  }
  ```
  * **細節解析**：若寫成 `int *y = &x; uint8_t z = *y;`，`*y` 會讀出完整的 32-bit 數字 `1`，賦值給 `uint8_t` 時會進行數值截斷，不論大小端 `z` 都會是 `1`。故必須強制轉型成 `(uint8_t*)` 才能唯讀第一個位址。
* **Union (共用體) 加分解法**：
  ```c
  bool is_little_endian_union(void) {
      union {
          uint32_t val;
          uint8_t bytes[4];
      } u;
      u.val = 1;
      return (u.bytes[0] == 1); // u.val 與 u.bytes[0] 共享實體 RAM 的第一個 Byte!
  }
  ```

#### 任務 2：32-bit Byte 反轉 (`0x12345678` ➡️ `0x78563412`)
* **經典位元遮罩與位移解法 (最推薦寫法)**：
  ```c
  uint32_t swap_endian(uint32_t val) {
      return ((val & 0x000000FF) << 24) | // 抓取 0x78 左移 24 位 ➡️ 0x78000000
             ((val & 0x0000FF00) << 8)  | // 抓取 0x56 左移 8 位  ➡️ 0x00560000
             ((val & 0x00FF0000) >> 8)  | // 抓取 0x34 右移 8 位  ➡️ 0x00003400
             ((val & 0xFF000000) >> 24);  // 抓取 0x12 右移 24 位 ➡️ 0x00000012
  }
  ```
* **Union 注意事項與檢討**：
  1. `union` 內部定義為型態藍圖，成員宣告需指定型態（如 `uint32_t num;`），不能直接寫傳入參數名 `val;`。
  2. 讀取 `u.bytes` 前必須先寫入資料 `u.num = val;`。
  3. 位元組合必須使用按位或 `|`，不能使用邏輯或 `||` (後者會變成 bool `1` 或 `0`)。

---

## 🎯 指標與記憶體 4 大特訓總結

1. **指標步長 (Step Size)**：`p + 1` 位址移動 `1 * sizeof(型態)`。`(uint8_t*)&x` 能精確讀取單一 Byte。
2. **絕對記憶體/暫存器寫入 (MMIO)**：`*( (volatile uint32_t *) 0x40001004 ) = 0xFF;`
3. **`*` 與 `++` 優先順序比較 (`int a[2] = {10, 20}; int *p = a;`)**：
   * `val = *p++` ➡️ 取舊值 `10`，指標 `p` 往前移 1 格 (指向 `a[1]`)。
   * `val = (*p)++` ➡️ 取舊值 `10`，記憶體內容 `a[0]` 變成 `11`，指標位置不變。
   * `val = *++p` ➡️ 指標 `p` 先往前移 1 格，取新值 `20`。
4. **`const` 指標黃金口訣**：
   * `const int *p` ➡️ `const` 在 `*` 左邊 ➡️ **內容唯讀 (Data Read-Only)**。
   * `int * const p` ➡️ `const` 在 `*` 右邊 ➡️ **位址唯讀 (Address Read-Only)**。
