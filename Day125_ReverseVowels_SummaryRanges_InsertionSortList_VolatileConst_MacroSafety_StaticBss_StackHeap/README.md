# Day 125 研發替代役 / 碩士預聘實戰訓練

## 📋 本日訓練大綱
* **Part 1 白板程式題（取材自 HackMD 考古與面試題，無提示純白板實戰）**
  * **P1**: LeetCode #345 - Reverse Vowels of a String（雙指標母音字母反轉、邊界防禦與 static inline 最佳化）
  * **P2**: LeetCode #228 - Summary Ranges（HackMD「群聯三題」排序區間彙總、兩層 malloc 與 (long long) 防溢位）
  * **P3**: LeetCode #147 - Insertion Sort List（HackMD「linklist insert」鏈結串列插入排序、循環閉環修復與 dummy 表頭維護）
* **Part 2 面試經典致命陷阱題**
  * **Trap 1**: 記憶體絕對位址寫入與 const/volatile 修飾（宣告 A 可寫 vs 宣告 B 唯讀編譯報錯、暫存器 #define 巨集括號防護）
  * **Trap 2**: 巨集副作用、整數溢位與位移後綴（SECONDS_PER_YEAR 運算子優先級破壞與 16-bit 溢位、BIT_MASK(18) 缺少 UL 導致 16 位元系統位移清零）
* **Part 3 基本 0x10 韌體與作業系統觀念題**
  * **Q1**: static 關鍵字三重含義（區域延長生命週期、全域/函式內部鏈結）＆ .bss 段開機 Startup Code 清零機制
  * **Q2**: Stack vs Heap 物理架構差異（位址增長方向、分配主體、速度）＆ 韌體即時系統嚴禁 malloc/free 之致命缺陷（不可重入、非確定性延遲、外部碎片）
* **自走車專案實務科普（系統廠 / IC 設計廠面試亮點）**
  * 雙核心自走車 I2C 感測器（IMU 陀螺儀）匯流排死鎖（SDA 鎖死在 LOW）與「9 個 Clock 脈衝」硬體解鎖機制

---

## 💻 Part 1 白板程式題總結

### 1. P1: LeetCode #345 - Reverse Vowels of a String
* **檔案路徑**：[Day125/p1.c](file:///c:/Users/a0907/Desktop/程式訓練/Day125/p1.c)
* **核心觀念**：
  * 雙指標向中間收斂，分別跳過非母音字元進行原地交換。
  * 語法陷阱：字串比對嚴禁使用單引號 `'aeiouAEIOU'`（會被視為多字元整數常數），必須使用雙引號或 `static inline bool isVowel(char c)`。
  * 邊界防禦：內部 `while` 必須包含 `left < right` 條件，防止在完全無母音的字串中指標越界溢位。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 481/481 測試案例。

### 2. P2: LeetCode #228 - Summary Ranges
* **檔案路徑**：[Day125/p2.c](file:///c:/Users/a0907/Desktop/程式訓練/Day125/p2.c)
* **核心觀念**：
  * 雙指標滑動區間：外層鎖定 `start`，內層推進探查連續遞增元素，鎖定 `end`。
  * 兩層記憶體配置：外層配置 `char**` 指標陣列，內層配置各區間字串（32 bytes 足以容納最大 24 bytes 的極端數值）。
  * 韌體防溢位：連續判定寫成 `(long long)nums[i + 1] - nums[i] == 1`，防止 `INT_MAX + 1` 觸發有號整數溢位 UB。
* **LeetCode 成績**：**0 ms (Beats 100%)**，通過 34/34 測試案例。

### 3. P3: LeetCode #147 - Insertion Sort List
* **檔案路徑**：[Day125/p3.c](file:///c:/Users/a0907/Desktop/程式訓練/Day125/p3.c)
* **核心觀念**：
  * 鏈結串列插入排序核心思維：維護已排序尾端 `tail` 與待插入節點 `p`。
  * 死迴圈避坑：插入節點後**必須立即 `break`**，否則指標結構被改動後繼續走訪會形成互相指向的閉環（Cycle 死鎖）。
  * 表頭失效防禦：尋找插入點必須從當前真正的表頭 `dummy.next` 出發，不可使用舊的 `head` 指標。
* **LeetCode 成績**：**Accepted (19/19 Passed)**。

---

## 🎯 Part 2 面試陷阱題解析

### Trap 1: 記憶體絕對位址寫入與 const/volatile 修飾
```c
int volatile * const p_regA = (int *) 0x1234;  // 宣告 A
const volatile int * p_regB = (int *) 0x1234;  // 宣告 B
```
1. **宣告 A 通過，宣告 B 報錯**：
   * 宣告 A 的 `const` 在 `*` 右邊，修飾指標本身不能改位址，但指向的記憶體內容為可寫 `int`，寫入順利完成。
   * 宣告 B 的 `const` 在 `*` 左邊，修飾 `int`，表示該記憶體位址為唯讀（Read-Only），賦值會觸發編譯錯誤。
2. **暫存器 #define 工業級寫法**：
   ```c
   #define REG_1234   (*((volatile uint32_t *)(0x1234)))
   ```
   嚴禁在巨集結尾加上分號 `;`，且外層必須加括號避免運算子優先級被破壞。

### Trap 2: 巨集副作用、整數溢位與位移後綴
```c
#define SECONDS_PER_YEAR 60*60*24*365
#define BIT_MASK(n) (1 << n)
```
1. **`SECONDS_PER_YEAR` 兩大錯誤**：
   * 未加括號：運算子優先級被破壞（例如 `x / SECONDS_PER_YEAR` 變成 `x / 60 * 60...`）。
   * 整數溢位：一年為 31,536,000 秒，在 16 位元系統（最大 32,767）直接溢位。
   * 正確寫法：`(60UL * 60UL * 24UL * 365UL)`。
2. **`BIT_MASK(18)` 缺少 `UL` 的致命問題**：
   * 常數 `1` 預設為 signed `int`，在 16 位元系統中執行 `1 << 18` 屬於未定義行為（UB），結果通常變為 0，硬體暫存器完全未被寫入。
   * 正確寫法：`#define BIT_MASK(n) (1UL << (n))`。

---

## 🧠 Part 3 基本 0x10 韌體與作業系統觀念題

### Q1: static 關鍵字三重含義 ＆ 嵌入式啟動流程
1. **static 的三重威力**：
   * **區域變數**：生命週期由 Stack 暫存延長為整個程式執行期間（儲存於 `.data` 或 `.bss`），函式退出數值不消失，但作用域仍限於函式內。
   * **全域變數**：限制為內部鏈結（Internal Linkage），其他檔案無法透過 `extern` 引用，避免全域符號衝突。
   * **函式**：限制為本檔案內部函式，編譯器能更積極執行 `inline` 內聯最佳化。
2. **儲存區段與開機清零**：
   * 未初始化的靜態變數存放在 **`.bss`** 段。
   * MCU 上電復位時，由存放於 Flash（`.text`）中的 **`Reset_Handler` 啟動代碼（Startup Code）** 執行清零迴圈，將 SRAM 的 `.bss` 段全數填 0。

### Q2: Stack vs Heap 硬體架構 ＆ 韌體即時系統禁忌
1. **物理架構差異**：
   * **增長方向**：Stack 由高位址往低位址增長（向下增長）；Heap 由低位址往高位址增長（向上增長）。
   * **管理主體**：Stack 由 CPU 硬體 SP 指針與編譯器呼叫慣例自動分配釋放；Heap 需工程師手動呼叫 `malloc`/`free`。
   * **速度**：Stack 僅需單一指令加減 SP（如 `SUB SP, #offset`），連續物理空間，速度極快；Heap 需走訪空閒鏈結表或系統呼叫，速度慢且時間不可預測。
2. **即時系統嚴禁 malloc/free 的三大缺陷**：
   * **不可重入（Non-reentrant）**：`malloc` 維護內部鏈結表，若在中斷 ISR 中重複呼叫會破壞內部中繼資料，引發系統崩潰。
   * **非確定性延遲（Non-deterministic）**：搜尋空閒區塊的時間無法保證上限，破壞 Hard Real-Time 截止時間要求。
   * **外部記憶體碎片（External Fragmentation）**：長期動態分配釋放會使記憶體零碎化，最終導致大區塊配置失敗（OOM）。

---

## 🚗 自走車專案實務科普：I2C 匯流排死鎖與「9 個 Clock 脈衝」解鎖機制

### 1. 災難現象：SDA 鎖死在 LOW（I2C Bus Lockup）
* **現象**：
  在雙核心自走車中，樹莓派或 MCU 透過 I2C 讀取 MPU6050 陀螺儀加速度計。
  車輛行駛中，馬達震動或電壓驟降觸發微控制器暫態重開機。
* **致命死鎖**：
  * 重開機前，若從端感測器（Slave）剛好正在發送資料或 ACK（**將 SDA 電位拉低為 LOW**），主機突然重置中斷。
  * 主機重新開機後，試圖在 I2C 產生 START 條件（SCL 為 HIGH 時將 SDA 由 HIGH 拉至 LOW）。
  * 但此時 SDA 已經被 Slave 拉在 LOW，主機無法產生 START，而 Slave 正在苦苦等待 Master 給 SCL Clock 來送出剩餘 bit。
  * **雙方永久死鎖！SDA 永遠卡在 LOW，整個 I2C 匯流排徹底癱瘓！**

### 2. 工業級解法：I2C 9-Clock 匯流排強制解鎖機制
在硬體 I2C 控制器初始化之前，先用 GPIO 軟體手動執行「匯流排清理流程（Bus Clear Routine）」：

```
SCL:  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐      ┌──────
      └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘(9)   │
                                                └─── STOP 條件
SDA:  (Slave 咬住 LOW) ───────> (Slave 釋放) ───┐   ┌──
                                              └───┘
```

1. **GPIO 模式切換**：將 SCL 設為推挽輸出（Output），SDA 設為浮空輸入（Input）。
2. **檢查 SDA 電位**：若 `SDA == LOW`，代表 Slave 咬死匯流排。
3. **發送 9 個 Clock 脈衝**：
   * 主機手動翻轉 SCL 9 次（High-Low-High）。
   * 前 8 個 Clock 讓 Slave 把手上的 Byte 送完；第 9 個 Clock 主機不回 ACK，Slave 內部狀態機便會判定傳輸結束，**主動放開 SDA（SDA 被外部上拉電阻拉回 HIGH）**！
4. **補發 STOP 條件**：在 SCL 為 HIGH 時，手動將 SDA 從 LOW 拉至 HIGH，將匯流排重設為 IDLE 閒置狀態。
5. **重設完成**：將 GPIO 重新映射給硬體 I2C 模組正常初始化。
