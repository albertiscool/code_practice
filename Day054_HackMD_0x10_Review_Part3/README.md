# Day 54: HackMD 韌體工程師 0x10 面試聖經 Part 3 (const 5大變體, volatile 3大例子與二次讀取 Bug, Bitwise 位元巨集)

今天是對照 HackMD 經典文章《韌體工程師的 0x10 個問題》特訓第三階段，我們精準複習了指標宣告快問快答 (指標陣列、陣列指標、函式指標陣列)、`const` 5 大語法變體、`volatile` 禁止暫存器快取的物理含意、3 大必考實戰情境、`square(volatile int *ptr)` 二次讀取除錯，以及 Bitwise 位元設定與清除巨集 (`SET_BIT` / `CLEAR_BIT`)。

---

## 📝 實戰考題與詳細解答 (HackMD 0x10 Questions 7~9)

### HackMD 第 7 題【關鍵字 const 的 5 大宣告變體】
* **原問題**：說明 `const` 5 種宣告物理含意。
* **解析與解答**：
  1. `const int a;` 與 `int const a;`：**完全無區別**，均代表 `a` 為唯讀整數。
  2. `const int *a;`：`const` 位於 `*` 左邊，代表 **內容 `*a` 唯讀不能改** (位址 `a` 可改)。
  3. `int * const a;`：`const` 位於 `*` 右邊，代表 **位址 `a` 唯讀不能改** (內容 `*a` 可改)。
  4. `const int * const a;`：`*` 兩側皆有 `const`，代表 **內容 `*a` 與位址 `a` 皆唯讀不能改**。

### HackMD 第 8 題【關鍵字 volatile 核心含意、3 大例子與 square() Bug】
* **原問題**：`volatile` 物理含意、3 個例子與 `square(volatile int *ptr)` 除錯？
* **解析與解答**：
  1. **物理含意**：告知編譯器該變數會被非預期地改變，**禁止編譯器做 CPU 暫存器快取優化**，每次讀寫皆強制親自存取實體 RAM 位址。
  2. **面試必考 3 大例子**：
     * **硬體記憶體映射暫存器 (Hardware Registers)**：如 GPIO 狀態暫存器 `IDR`，按鈕按下時數值隨時改變。
     * **ISR 中斷服務函式修改的全域變數**：ISR 異步修改變數，主迴圈 `main()` 讀取時須防範暫存器快取死鎖。
     * **多執行緒 / RTOS 多任務共享的變數**：Task A 與 Task B 共享的旗標變數。
  3. **`square()` 二次讀取 Bug 修正**：
     ```c
     // ❌ 錯誤：*ptr * *ptr 會至 RAM 讀取兩次，期間數值被硬體改變會算錯平方
     // int square(volatile int *ptr) { return *ptr * *ptr; }

     // ✅ 正確寫法：先暫存至區域變數，鎖定只讀取 RAM 一次
     int square(volatile int *ptr) {
         int a = *ptr;
         return a * a;
     }
     ```

### HackMD 第 9 題【位元操作：設定 Set 與清除 Clear Bit 3】
* **原問題**：寫出 Bit 3 設為 1 與清為 0 之單行寫法與通用巨集。
* **解析與解答**：
  1. **Bit 3 設為 1**：`a |= (1 << 3);`
  2. **Bit 3 清為 0**：`a &= ~(1 << 3);`
  3. **通用型位元巨集**：
     ```c
     #define SET_BIT(p, n)   ((p) |= (1 << (n)))
     #define CLEAR_BIT(p, n) ((p) &= ~(1 << (n)))
     ```
