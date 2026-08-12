# Day 56: HackMD 韌體工程師 0x10 面試聖經 Part 5 (全 16 題大滿貫封頂結業)

今天是對照 HackMD 經典文章《韌體工程師的 0x10 個問題》特訓第五階段，我們完成了最後 4 題 (`malloc(0)` 行為與記憶體碎片、`typedef` vs `#define` 型態別名指標、未定義行為 `a++ + ++a` 序列點、以及硬體 Busy-Waiting 超時 Timeout 防禦與 Watchdog 機制)，正式達成 **HackMD 0x10 個問題 100% 全數通關！**

---

## 📝 實戰考題與詳細解答 (HackMD 0x10 Questions 13~16)

### HackMD 第 13 題【動態記憶體配置 malloc(0) 與記憶體碎片】
* **原問題**：`malloc(0)` 回傳什麼？嵌入式環境為何排斥 `malloc`/`free`？
* **解析與解答**：
  1. **`malloc(0)` 行為與 `free`**：C 標準規範為 Implementation-Defined。在 GCC/glibc 等多數標準庫下，會配置包含 Chunk Header 的最小空間並回傳**非 NULL 的有效指標**。因其非 NULL，使用後**必須呼叫 `free(ptr)`** 以防 Chunk Header 洩漏。
  2. **嵌入式記憶體碎片 (Fragmentation)**：單晶片 RAM 有限，頻繁動態配置釋放會產生無法連續利用的孔洞碎片。長期運作最終導致大區塊配置失敗 (回傳 NULL) 觸發系統當機。

### HackMD 第 14 題【typedef vs #define 型態別名大對決】
* **原問題**：`#define dps char *` 與 `typedef char * dscp;` 下 `dps p1, p2; dscp p3, p4;` 的型態？
* **解析與解答**：
  1. **變數型態**：`p1`, `p3`, `p4` 為 `char*` 指標；`p2` 為普通 `char`。
  2. **原理差異**：`#define` 為預處理純文字替換，`dps p1, p2` 被展開為 `char * p1, p2;`；`typedef` 為編譯器層級真正的型態別名。

### HackMD 第 15 題【未定義行為與順序點 Undefined Behavior】
* **原問題**：`int a = 5; int b = a++ + ++a;` 結果與問題？`x = a++; y = ++a;` 分開結果？
* **解析與解答**：
  1. **未定義行為 (UB)**：同一行在兩序列點間對同變數修改超逾一次屬於 UB，不同編譯器優化等級結果不一。
  2. **分開寫數值**：
     * `x = a++;` ➡️ 後加 (Post-increment)，`x` 取得舊值 **`5`**，`a` 增為 6。
     * `y = ++a;` ➡️ 前加 (Pre-increment)，`a` 增為 7，`y` 取得新值 **`7`**。

### HackMD 第 16 題【硬體 Busy-Waiting 與看門狗 Watchdog 災難】
* **原問題**：`while ((*status_reg & 0x80) == 0)` 死等硬體災難與工業級防線？
* **解析與解答**：
  1. **死鎖與 Watchdog 重啟災難**：硬體故障致使 Bit 7 恆為 0 時，無限制死等會導致 CPU 無限旋轉死鎖，最終引爆看門狗 (Watchdog Timer) 強制系統重啟。
  2. **工業級 Timeout 防禦**：
     ```c
     // ✅ 加上 Timeout Counter 防線
     uint32_t timeout = 10000;
     while ((*status_reg & 0x80) == 0) {
         if (--timeout == 0) return ERROR_TIMEOUT;
     }
     ```
     在 Linux 中亦可使用 `pthread_cond_timedwait()`，RTOS 中使用帶有 Tick 超時的 Semaphore 或 `vTaskDelay()`。

---

## 🏆 0x10 面試聖經全 16 題通關大滿貫報告

恭喜你！從第 1 題 `SECONDS_PER_YEAR` UL 溢位防禦，一路挑戰 `MIN` 巨集副作用、`#error` 預處理中斷、無窮迴圈 `for(;;)` 與低功耗 WFI、a~h 8 大指標宣告、`static` 3 大作用、`const` 5 大變體、`volatile` 3 大情境與二次讀取除錯、Bitwise `SET`/`CLEAR` 遮罩、絕對位址單行解引用 `*( (volatile int *)0x67a9 ) = 0xaa55;`、ISR 4 大致命錯誤、二補數 42 億算術、到最後的 `malloc(0)` 與 Busy-waiting 超時防禦！

你已將這份廣受各大 IC 設計廠傳閱的 16 道經典面試題 100% 徹底征服！🚀
