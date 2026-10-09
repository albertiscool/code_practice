# Day 112: 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式 (XOR單數消去 × 有序串列合併 × 快慢指針測環 × const指標口訣 × 結構體Padding × UART非阻塞狀態機)

## 📑 今日訓練概覽
- **P1 (位元運算消去經典之王)**: LeetCode #136: Single Number (Easy) - XOR（$\oplus$）自反性 $a \oplus a = 0$ 與結合律，嚴格 $O(N)$ 線性時間、$O(1)$ 常數空間最優解 (0 ms, Beats 100.00%, Sub ID: 2167254428)
- **P2 (有序鏈結串列合併手術)**: LeetCode #21: Merge Two Sorted Lists (Easy) - 虛擬節點 Dummy Node 消除首節點特判 + 雙指標有序重接 + 尾端長鏈直接銜接 (0 ms, Beats 100.00%, Sub ID: 2167263006)
- **P3 (鏈結串列快慢指標測環)**: LeetCode #141: Linked List Cycle (Easy) - Floyd 龜兔賽跑演算法（Tortoise and Hare）嚴格 $O(1)$ 空間偵測環形結構，防禦空指標訪問 (Accepted, 29/29 Passed, Sub ID: 2167268620)
- **面試經典致命陷阱題**:
  - `const` 指標四種排列組合：「左定值，右定向」一秒辨識內容唯讀與指標唯讀
  - 結構體 Padding 與記憶體自然對齊：`struct S1`（12 Bytes）vs `struct S2`（8 Bytes）偏移量與補齊計算
- **基本 0x10 韌體核心觀念題**:
  - `static` 關鍵字三種作用域物理意義：區域變數（Data/BSS 延長生命週期）、全域變數與函式（Internal Linkage 模組內部私有化）
  - 大小端（Little Endian vs Big Endian）判定演算法與跨處理器通訊 Byte Order 轉換
- **自走車專案實務科普 (第 112 講)**:
  - 樹莓派（Linux）與微控制器（MCU）跨晶片 UART 通訊協議：二進位 Frame 標頭（`0xAA 0x55`）、長度、指令、Payload 與 CRC 校驗
  - 非阻塞事件驅動狀態機（Event-Driven FSM）逐 Byte 解析：杜絕傳統 `while` 阻塞死等導致馬達 PID 週期凍結與跑偏

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Single Number (#136 - Easy)
- **核心思維**:
  - 利用 XOR 互斥或的三大性質：
    1. 自反性：$a \oplus a = 0$
    2. 單位元素：$a \oplus 0 = a$
    3. 交換律與結合律：$a \oplus b \oplus a = (a \oplus a) \oplus b = 0 \oplus b = b$
  - 連續 XOR 陣列內所有元素，成對出現者必兩兩抵銷為 0，最終殘留者必為該唯一獨立數字。
- **面試加分點**:
  - 初始化 `int result = 0;` 從索引 0 遍歷至結尾，天然相容 `numsSize == 1`，無需任何分支判斷。

### P2: Merge Two Sorted Lists (#21 - Easy)
- **核心思維**:
  - 使用 Stack 上的虛擬頭節點 `struct ListNode dummy; dummy.next = NULL;`。
  - 維護指標 `struct ListNode* tail = &dummy;`。
  - 當 `list1` 與 `list2` 皆非空時，比較兩者之值，較小者掛在 `tail->next`，並將該串列指標前進一步（`list = list->next`）。
  - 迴圈結束後，將剩餘非空串列直接一筆掛上：`tail->next = (list1 != NULL) ? list1 : list2;`。
- **關鍵避坑點**:
  - **區域變數野指標防護**：`dummy` 未初始化時 `dummy.next` 為隨機垃圾值，若直接寫 `tail = dummy.next` 會導致訪問無效記憶體觸發 SEGV 當機！必須指向實體位址 `tail = &dummy`。
  - **指標前進推進**：每次串接後，來源串列指針必須前進，否則陷入死迴圈。

### P3: Linked List Cycle (#141 - Easy)
- **核心思維**:
  - 慢指標每次走 1 步，快指標每次走 2 步。
  - 若有環，慢指標進入環後，兩者相對距離每回合縮小 1 步，保證必在 1 圈內相遇。
- **邊界保護**:
  - `while (fast != NULL && fast->next != NULL)` 同時保護快指針與下一個節點，杜絕 `fast->next->next` 解引用空指標。

---

## 💣 面試經典致命陷阱題複盤

### 陷阱 1：`const` 指標四種組合判斷口訣
```c
const int *p1;       // const 在 * 左邊 -> 內容唯讀 (所指數值不能改)
int const *p2;       // const 在 * 左邊 -> 內容唯讀 (所指數值不能改)
int * const p3;       // const 在 * 右邊 -> 指標唯讀 (指標位址不能改)
const int * const p4; // * 左右皆有 const -> 內容與位址皆不能改
```
- **一秒辨識口訣**：**「以 `*` 為界，左定值（內容）、右定向（位址）」**！

### 陷阱 2：結構體 Padding 與自然對齊計算
```c
struct S1 {
    char a;  // 1B + 3B padding = 4B (offset 0~3)
    int b;   // 4B            = 4B (offset 4~7)
    short c; // 2B + 2B padding = 4B (offset 8~11)
}; // 總大小 = 12 Bytes

struct S2 {
    int b;   // 4B            = 4B (offset 0~3)
    short c; // 2B            = 2B (offset 4~5)
    char a;  // 1B + 1B padding = 2B (offset 6~7)
}; // 總大小 = 8 Bytes
```
- **核心規則**：
  1. 每個成員的起始位移（offset）必須是該成員型別大小的整數倍。
  2. 結構體的總大小必須是最大成員大小（此處為 `int` 4 Bytes）的整數倍。
  3. 成員依「由大到小」排列可大幅節省 Padding 填充空間（12B 壓縮至 8B）。

---

## ⚔️ 基本 0x10 韌體核心觀念題複盤

### 題目一：`static` 關鍵字在三種作用域的底層物理意義
1. **函式內部修飾區域變數**：
   - 儲存位置自 Stack 移至 **Data 區（已初始化）或 BSS 區（未初始化）**。
   - 生命週期自函式結束延長為**「整個程式運行期間」**，數值在多次呼叫間得以保留。
2. **檔案全域修飾全域變數**：
   - 符號表（Symbol Table）鏈結屬性自 External Linkage 限制為 **Internal Linkage**。
   - 該變數僅在當前 `.c` 檔案內部可見，其他檔案使用 `extern` 亦無法存取，杜絕命名衝突。
3. **修飾函式**：
   - 限制該函式為 **當前檔案私有函式（Private Helper）**，禁止外部檔案呼叫。
   - 在模組化韌體開發中，除了 Header 開放的 API 外，其餘函式全數應宣告為 `static`，促進編譯器進行 Inline 最佳化並達成封裝。

### 題目二：大小端（Little Endian vs Big Endian）判定與通訊轉換
1. **定義**:
   - **Little Endian（小端序）**：低位元組（LSB）存放在低記憶體位址（ARM Cortex-M 晶片、x86 預設皆為小端序）。
   - **Big Endian（大端序）**：高位元組（MSB）存放在低記憶體位址（TCP/IP 網路位元組序）。
2. **C 語言判定寫法（指標法 / Union 法）**:
   ```c
   int check_endian(void) {
       uint16_t test = 0x0001;
       uint8_t *p = (uint8_t*)&test;
       return (*p == 0x01); // 回傳 1 為小端序 (Little)，0 為大端序 (Big)
   }
   ```
3. **跨架構通訊翻車案**:
   - 若 MCU（小端序）將 16-bit 數據 `0x1234` 直接透過 UART 發送，記憶體排列為 `[0x34, 0x12]`。
   - 接收端若以大端序解析，會讀成 `0x3412`（數值由 4660 突變為 13330），造成馬達暴衝或數值失真。
   - 標準作法：通訊時統一規範字節序（如網路位元組序 Big-Endian），發送端與接收端呼叫 `htons()` / `ntohs()` 轉換。

---

## 🚗 自走車實務科普：跨處理器 UART 協議與非阻塞 FSM 狀態機

### 1. 傳統阻塞式讀取的致命缺陷
- 呼叫 `while(!Serial.available())` 死等封包，會將 MCU CPU 佔據數毫秒。
- 底盤馬達 PID 控制週期（10ms~20ms）被凍結，導致車輛頓挫或跑偏撞牆。
- 線路雜訊少收 1 Byte 會引發死鎖當機。

### 2. 非阻塞有限狀態機（FSM）逐 Byte 解析
- 函式 `parse_byte(uint8_t ch)` 本身無迴圈，全域變數 `current_state` 保有狀態記憶。
- 每接收 1 個 Byte 推進一次狀態（`WAIT_SOF1` $\to$ `WAIT_SOF2` $\to$ `WAIT_LEN` $\to$ `WAIT_CMD` $\to$ `WAIT_PAYLOAD` $\to$ `WAIT_CRC`）。
- 耗時不到 $0.1\mu s$ 立即返回，完全不干擾馬達控制；遇雜訊錯位瞬間重置狀態，具備自我修復容錯能力。
