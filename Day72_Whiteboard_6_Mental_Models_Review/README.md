# Day 72: 韌體白板 6 大核心心智模型 (Mental Models) 與 0x10 面試全攻略總複習

本篇為前 71 天高強度白板手寫刷題（20+ 經典題與 🟡 Medium 中等題）與 0x10 嵌入式 C 語言精華之「體系化心智模型總結」。將所有演算法降維歸納為 6 大底層原型，幫助在韌體與 IC 廠技術面試中達到「見題秒辨原型、直接寫出滿分架構」的直覺反應。

---

## 🧠 韌體白板 6 大核心心智模型 (Mental Models)

---

### 🎯 模型一：【雙指標體系 (Two Pointers)】—— 空間 $O(1)$ 的極致

雙指標是嵌入式系統中最關鍵的記憶體節約技巧，核心在於不依賴額外 Buffer，原地完成尋址與重組。

| 題目原型 | 典型題號與難度 | 核心套路與畫面 |
| :--- | :--- | :--- |
| **字串 / 陣列原地反轉** | LeetCode #344 🟢 Easy | 首尾雙指標 `head` 與 `tail` 向中間對撞，邊走邊 `swap`。 |
| **驗證迴文串** | LeetCode #125 🟢 Easy | 雙指標向內比對，遇到標點符號或空格自動跳過。 |
| **刪除重複項** | LeetCode #26 🟢 Easy | 快慢雙指標：快指標探路，慢指標維護「有效陣列邊界」，原地覆蓋。 |
| **鏈結串列相交點** | LeetCode #160 🟢/🟡 | 雙指標各走一遍 $A+B$（走完自己換走對方），**消除長度差**，必在相交點碰頭！ |
| **鏈結串列環入口點** | LeetCode #142 🟡 Medium | **Floyd 兩階段**：階段 1 快 2 慢 1 相遇；階段 2 慢指標回 Head，兩者各走 1 步，由 $x = nC - y$ 必定在**環入口點**相碰！ |
| **刪除倒數第 N 節點** | LeetCode #19 🟡 Medium | 快指標先走 $n+1$ 步，慢指標再一起走。搭配 **`dummy` 哨兵節點** 消除刪除 Head 的邊界例外！ |

---

### 🎯 模型二：【多重反轉神技 (Multi-Pass Reversals)】—— 零記憶體結構重組

禁止使用額外陣列時，透過「整體反轉 + 局部反轉」達成 $O(N)$ 原地重排：

1. **旋轉陣列 Rotate Array** (LeetCode #189 🟡 Medium)：
   * 題目：將陣列向右旋轉 $k$ 步。
   * **三步反轉法**：
     1. 整體反轉 `[0, N-1]`
     2. 前 $k$ 個反轉 `[0, k-1]`
     3. 剩餘部分反轉 `[k, N-1]`
2. **反轉單字 Reverse Words in a String** (LeetCode #151 🟢/🟡)：
   * 題目：`"the sky is blue"` ➔ `"blue is sky the"`。
   * **兩步反轉法**：
     1. 整體反轉（`"eulb si yks eht"`）
     2. 逐一走訪，遇到空格 `' '` 或結尾 `'\0'` 時將每個單字內部反轉（`"blue is sky the"`）！

---

### 🎯 模型三：【位元運算與暫存器操作 (Bitwise Magic)】—— IC 廠的最愛

暫存器操作與位元運算是微控制器底層的核心語言：

1. **暫存器位元操作大四喜**：
   * **Set Bit**：`val |= (1U << n)`
   * **Clear Bit**：`val &= ~(1U << n)`
   * **Toggle Bit**：`val ^= (1U << n)`
   * **Read Bit**：`(val >> n) & 1U`
   * *(專業加分點：永遠使用 `1U` 避免第 31 位 signed int 溢位之未定義行為 UB！)*
2. **Single Number I** (LeetCode #136 🟢 Easy)：
   * 成對數字利用 **異或 `a ^ a = 0`** 全部自動消滅，剩下的就是單身數字。
3. **Single Number II** (LeetCode #137 🟡 Medium)：
   * 其餘數字出現 3 次：利用 **32-Bit 直式加法** 統計每一縱欄 bit 為 1 的總數，對 3 取餘數 (`sum % 3`)，再用 `result |= (1U << i)` 還原出獨一無二的數字！

---

### 🎯 模型四：【降維與滾動優化 (Dimension Reduction & State Roll)】

MCU 記憶體極其珍貴，如何把 $O(N)$ 空間壓縮到 $O(1)$：

1. **爬樓梯 / 費氏數列** (LeetCode #70 🟢 Easy)：
   * 捨棄遞迴與大陣列，使用 `first`, `second`, `third` **3 個滾動變數**，$O(1)$ 空間防止 MCU 爆棧！
2. **買賣股票的最佳時機 II** (LeetCode #122 🟡 Medium)：
   * **貪心演算法 (Greedy)**：把長途波段拆解為每日短波動（$(3-1)+(5-3) = 5-1$）。只要明天比今天高就累加正差額，下跌不操作，單次走訪 $O(N)$！
3. **字串第一個唯一字元** (LeetCode #387 🟢 Easy)：
   * **頻率統計陣列**：開一個長度 26 的 `count[26]` 取代 Hash Map，兩次走訪 $O(N)$ 時間、$O(1)$ 空間解決！
4. **最大子陣列和 (Kadane 演算法)** (LeetCode #53 🟡 Medium)：
   * 走訪陣列，只要前面的累積和 $< 0$，就立刻斷捨離「重開新起點」！

---

### 🎯 模型五：【二元樹的兩大思維 (Tree Paradigms)】

1. **廣度優先走訪 (BFS / 層序走訪)** (LeetCode #102 🟢/🟡)：
   * 利用 **Queue (先進先出 FIFO)**：爸爸出隊時，把左右小孩推進隊尾，自然實現「一層一層由上到下」走訪。純 C 語言中以指標陣列 `queue[]` 配合 `head` 與 `tail` 模擬。
2. **二元搜尋樹驗證 (BST Validation)** (LeetCode #98 🟡 Medium)：
   * 避開「只看父子關係」的陷阱！將祖先的約束打包成 **開區間 `(min, max)` 傳遞**，往左走更新上限，往右走更新下限，搭配 `LLONG_MIN/MAX` 避免溢位！

---

### 🎯 模型六：【純 C 手寫底層資料結構 (Pure C Infrastructure)】

1. **環形緩衝區 Ring Buffer (Circular Queue)** (LeetCode #622 🟡 Medium)：
   * 掌握 `head`, `tail`, `count` 與模運算 `(tail + 1) % CAPACITY`，解決 UART 串口與 FreeRTOS 佇列通訊。
2. **純 C 陣列模擬 Stack** (LeetCode #20 🟢 Easy)：
   * 用 `top` 索引實作括號匹配與遞迴扁平化。

---

## 📚 0x10 嵌入式 C 語言核心命題回顧

1. **`volatile` 的本質**：僅保證「禁止編譯器快取與指令刪除」，**絕不保證原子性、記憶體屏障與執行緒安全**。
2. **`volatile` 指標解讀**：
   * `volatile int *p;` ➔ `*p` 數值為 volatile（如周邊硬體暫存器 `GPIOA->IDR`）。
   * `int * volatile p;` ➔ `p` 指標變數位址為 volatile（如中斷雙緩衝區 Ping-Pong Buffer 切換）。
3. **ARM Cortex-M 雙堆疊指標**：
   * `PSP` (Process Stack Pointer) 供使用者 Task 使用。
   * `MSP` (Main Stack Pointer) 供 OS Kernel 核心與 ISR 中斷共用，防止 Task 爆棧踩死系統。
4. **快取一致性 (Cache Coherency) 與 DMA**：
   * TX 發送前呼叫 **`SCB_CleanDCache()`**（快取 Flush 寫回實體 RAM）。
   * RX 接收前呼叫 **`SCB_InvalidateDCache()`**（快取作廢失效，強制自 RAM 抓最新資料）。
5. **`memcpy` vs `memmove`**：
   * `dest > src` 記憶體重疊時，`memmove` 自動改由「從後往前拷貝」以防範來源資料踩踏。

---

## 🚗 智慧自駕小車專案核心問答速查

* **Linux GPOS vs MCU Hard Real-Time**：樹莓派負責吃資源的 YOLOv8 AI 視覺與 ROS 2；MCU 負責微秒級 PWM 脈衝、馬達差速與硬即時煞車。
* **POSIX `mmap` 零拷貝**：Camera Frame Buffer 直接映射至 User Space，避免三次 `memcpy`，大幅提升 FPS。
* **超音波常數推導**：聲速 $340 \text{ m/s} = 0.03434 \text{ cm/}\mu\text{s} \to 29.1 \mu\text{s/cm}$，單程公式為 $\frac{\text{duration}}{2} \div 29.1$。
* **L298N H 橋煞車機制**：`PWM = 0` 為慣性滑行 (Coasting)；`IN1 = LOW, IN2 = LOW, PWM = 255` 形成閉合短路回路，利用反電動勢 (Back-EMF) 達成瞬間主動動態煞車 (Dynamic Active Braking)。
