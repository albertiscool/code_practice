# Day 113: 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式 (高斯求和/XOR缺失數 × 雙指標原地移動零 × 快慢指針中點 × 指標運算優先級 × define與typedef別名 × 通訊中斷Failsafe)

## 📑 今日訓練概覽
- **P1 (尋找遺失的數字)**: LeetCode #268: Missing Number (Easy) - 高斯等差數列求和 $\frac{n(n+1)}{2} - \sum \text{nums}$ 與 XOR（$\oplus$）對偶消去法，嚴格 $O(N)$ 線性時間、$O(1)$ 常數空間零溢位最優解 (0 ms, Beats 100.00%, Sub ID: 2169054152)
- **P2 (陣列原地滾動移動零)**: LeetCode #283: Move Zeroes (Easy) - 快慢雙指標（Two Pointers）原地覆寫不變量維護 + 尾端批次補零，嚴格 $O(1)$ 空間 (0 ms, Beats 100.00%, Sub ID: 2169088125)
- **P3 (鏈結串列尋找中間節點)**: LeetCode #876: Middle of the Linked List (Easy) - 經典快慢指針（快2慢1）一次遍歷精準命中中間節點，完美相容奇數/偶數長度與單節點 (0 ms, Beats 100.00%, Sub ID: 2169094225)
- **面試經典致命陷阱題**:
  - 指標運算優先級（`*p++` vs `(*p)++` vs `*++p` vs `++*p`）後置/前置遞增與解引用單目運算子結合順序
  - `#define` vs `typedef` 指標別名陷阱：前處理文字替換（`int *a, b;` 導致 `b` 退化為純整數）vs 編譯器真正型別定義
- **基本 0x10 韌體核心觀念題**:
  - C 語言程式記憶體佈局（Memory Layout）五大分區：Text、Data（有初值全域/靜態）、BSS（未初值全域/靜態）、Stack（區域變數/函式呼叫棧）、Heap（動態分配）
  - Startup Code 開機初始化流程：搬移 Flash 到 RAM（Data 區）與清零（BSS 區）
  - 暫存器位元操作四大標準巨集（Set, Clear, Toggle, Test）與讀改寫（RMW）硬體安全防護
- **自走車專案實務科普 (第 113 講)**:
  - 雙核心（樹莓派 + MCU）UART 通訊中斷急停防暴衝架構（Failsafe Watchdog）
  - 心跳包（Heartbeat）與超時時間戳判定，馬達 H 橋 PWM 立即置零與短路煞車
  - MCU 復位開機引腳浮空（High-Z Floating）突波硬體下拉電阻（10kΩ Pull-down）防護

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Missing Number (#268 - Easy)
- **核心思維**:
  - 高斯求和公式：$\text{sum} = \frac{n(n + 1)}{2} - \sum \text{nums}[i]$。
  - 時間複雜度 $O(N)$，額外空間 $O(1)$。
- **面試加分點（XOR 零溢位解）**:
  - 當 $n$ 極大時，`n * (n + 1)` 有 32-bit 整數溢位風險。
  - 利用 $a \oplus a = 0$ 與 $a \oplus 0 = a$：將 $[0, n]$ 的所有索引與陣列元素全部連做 XOR，成對出現者自然消去，最終殘留者即為缺失數字！

### P2: Move Zeroes (#283 - Easy)
- **核心思維**:
  - 兩階段雙指標法：快指標 `fast` 負責掃描非零元素並覆寫至 `nums[slow++]`；第二階段 `slow` 往後跑完全程並補 `0`。
- **進階追問（單次遍歷 Swap）**:
  - 只要 `fast` 遇到非零項，且 `slow != fast`，直接將 `nums[slow]` 與 `nums[fast]` 對調，`0` 會自然往後滾動，一次迴圈搞定！

### P3: Middle of the Linked List (#876 - Easy)
- **核心思維**:
  - 慢指標每次走 1 步，快指標每次走 2 步。
  - 奇數長度（如 5 顆）：`fast` 停在尾節點（`fast->next == NULL`），`slow` 剛好停在第 3 顆。
  - 偶數長度（如 6 顆）：`fast` 走過尾節點停在 `NULL`，`slow` 停在第 4 顆（第二個中點）。
  - 邊界防禦：`while (fast != NULL && fast->next != NULL)` 完美杜絕空指標訪問。

---

## 💣 面試經典致命陷阱題複盤

### 陷阱 1：指標運算優先級（`*p++` vs `(*p)++` vs `*++p` vs `++*p`）
```c
int arr[] = {10, 20, 30};
int *p = arr;
int v1 = *p++;   // v1 = 10, p 移到 arr[1], arr 未變
int v2 = (*p)++; // v2 = 20, p 停在 arr[1], arr[1] 變成 21
int v3 = *++p;   // v3 = 30, p 先移到 arr[2] 再取值
int v4 = ++*p;   // v4 = 31, p 停在 arr[2], arr[2] 變成 31
```
- **執行後狀態**:
  - `v1 = 10, v2 = 20, v3 = 30, v4 = 31`
  - 陣列內容變為 `{10, 21, 31}`
  - 指標 `p` 指向 `arr[2]`

### 陷阱 2：`#define` vs `typedef` 指標別名
```c
#define PINT_MACRO int*
typedef int* PINT_TYPEDEF;

PINT_MACRO a, b;   // 展開為: int* a, b;   -> a 是 int*, b 是 int！
PINT_TYPEDEF c, d; // 編譯器型別別名: c 是 int*, d 是 int*！
```
- **核心盲點**：`#define` 只是單純的文字替換，在 C 語言中 `*` 是結合在變數名稱上的（`int *a, b;`），導致 `b` 缺少 `*` 而變成純整數！
- **工業級標準**：在 C/C++ 中定義型別別名時，**唯一推薦使用 `typedef` 或 `using`**，嚴禁使用 `#define`！

---

## ⚔️ 基本 0x10 韌體核心觀念題複盤

### 題目一：Memory Layout 五大分區
1. **Text 區（程式碼區，唯讀，位於 Flash）**：存放 CPU 機械碼與常數（如字串字面量）。
2. **Data 區（已初始化資料區，位於 RAM）**：存放「初始值不為 0」的全域變數與靜態變數（如 `g_var1 = 100`, `s_var = 50`）。
3. **BSS 區（未初始化資料區，位於 RAM）**：存放「未初始化」或「初值為 0」的全域變數與靜態變數（如 `g_var2`, `g_var3 = 0`）。
4. **Stack 區（堆疊區，位於 RAM）**：存放函式呼叫的局部變數、參數、返回位址（如 `local_var`, 指標變數 `ptr` 本身）。由高位址向低位址生長。
5. **Heap 區（堆積區，位於 RAM）**：由 `malloc()` / `free()` 動態配置的記憶體（如 `malloc(128)` 配置的那 128 Bytes）。由低位址向高位址生長。
- **Startup Code 職責**：
  - 微控制器開機時，啟動檔（`startup.s`）負責將 Data 區的初始值從 Flash 拷貝到 RAM。
  - 負責將整個 BSS 區段的 RAM 記憶體全數抹零（Zero Initialization）。

### 題目二：暫存器位元操作四大標準巨集
```c
#define SET_BIT(REG, n)    ((REG) |= (1U << (n)))
#define CLEAR_BIT(REG, n)  ((REG) &= ~(1U << (n)))
#define TOGGLE_BIT(REG, n) ((REG) ^= (1U << (n)))
#define TEST_BIT(REG, n)   (((REG) & (1U << (n))) != 0)
```
- **禁止直接賦值 `REG = (1 << n)` 的災難後果**：
  暫存器中往往包含數十個硬體控制位（如中斷使能、時脈開關、分頻係數）。直接用等號會將除了第 $n$ bit 以外的所有暫存器配置全部強制洗成 0，破壞其他周邊運作！必須採用 **讀改寫（Read-Modify-Write, RMW）** 模式。

---

## 🚗 自走車實務科普：通訊中斷急停 Failsafe 與引腳浮空防暴衝

### 1. 通訊斷線看門狗（Failsafe Timeout）
- 樹莓派因演算法崩潰或線路鬆脫停止傳送指令時，MCU 透過記錄 `last_rx_tick` 時間戳。
- 若超時（如 > 200ms）未收到合法封包，主動將馬達 PWM 強制歸零並施加短路煞車，避免自走車盲奔撞牆。

### 2. MCU 開機重啟 GPIO 浮空（High-Z）防暴衝
- MCU 在 Reset 期間，GPIO 處於高阻抗浮空狀態，外界電磁噪聲易被馬達驅動晶片誤判為高電位觸發馬達抽搐。
- 硬體防禦：在 PWM 控制線外接 10kΩ 下拉電阻至 GND，開機期間強制箝位在 0V 低電位。
