# Day 55: HackMD 韌體工程師 0x10 面試聖經 Part 4 (絕對位址解引用單行寫法, ISR 4大致命錯誤, 隱式轉型與二補數42億算術)

今天是對照 HackMD 經典文章《韌體工程師的 0x10 個問題》特訓第四階段，我們精準複習了絕對記憶體位址解引用單行寫法 `*( (volatile int *)0x67a9 ) = 0xaa55;`、ISR 4 大致命錯誤 (無傳參、無回傳、嚴禁非重入 `printf` 與浮點數)、以及 `unsigned` 隱式型態提升 (Usual Arithmetic Conversions) 與二補數 ($2^{32} - 20 = 4,294,967,276$) 物理原理。

---

## 📝 實戰考題與詳細解答 (HackMD 0x10 Questions 10~12)

### HackMD 第 10 題【存取特定記憶體位址 Absolute Memory Access】
* **原問題**：絕對位址 `0x67a9` 寫入整數數值 `0xaa55`。
* **解析與解答**：
  1. **型態轉型與 `volatile`**：暫存器位址必須強制轉型為 `(volatile int *)`，防範編譯器將讀寫暫存器的動作做快取優化。
  2. **單行解引用寫法 (One-liner)**：
     ```c
     *( (volatile int *)0x67a9 ) = 0xaa55;
     ```
     將數字 `0x67a9` 強制轉型為指標，外面加解引用符號 `*` 代表前往該記憶體位址寫入 `0xaa55`。

### HackMD 第 11 題【ISR 程式碼除錯】
* **原問題**：`__interrupt double compute_area(double radius)` 程式碼審查與除錯？
* **解析與解答**：
  1. ❌ **回傳值錯誤**：`double` ➡️ ISR 必須是 `void` 回傳 (無 C 程式碼呼叫者可接收回傳值)。
  2. ❌ **傳參錯誤**：`(double radius)` ➡️ 硬體非同步觸發 ISR，無法由軟體傳參，參數必須為 `(void)`。
  3. ❌ **非重入與阻塞**：`printf()` ➡️ 非重入 (Non-reentrant) 且帶有 stdout I/O 鎖，在中斷上下文呼叫會引發系統死鎖崩潰。
  4. ❌ **雙精度浮點數**：`double` 運算極耗時且需 PUSH/POP FPU 暫存器，破壞 ISR 快速離開原則。

### HackMD 第 12 題【型態轉換與 Unsigned 陷阱】
* **原問題**：`unsigned int a = 6; int b = -20; (a + b > 6) ? puts("> 6") : puts("<= 6");` 輸出？
* **解析與解答**：
  1. **印出結果**：螢幕會印出 **`> 6`**。
  2. **隱式型態轉換與二補數 (Two's Complement)**：
     * 當 `unsigned int` 與 `int` 混合運算時，`int` 被強制隱式提升為 `unsigned int`。
     * `-20` 的 32-bit 二補數記憶體二進位為 `0xFFFFFFEC`。被當成無號數解釋時，其數值變成 $2^{32} - 20 = \mathbf{4,294,967,276}$。
     * $6 + 4,294,967,276$ 為超巨大正整數，遠大於 $6$。
