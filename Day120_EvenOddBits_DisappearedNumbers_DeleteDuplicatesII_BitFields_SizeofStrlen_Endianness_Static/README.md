# Day 120 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-19
- **訓練主軸**: 奇偶位元遮罩平行提取 (Number of Even and Odd Bits)、陣列原地正負號標記與缺失數字探測 (Find All Numbers Disappeared in an Array)、已排序鏈結串列重複節點全數整批拔除 (Remove Duplicates from Sorted List II)、結構體位元欄 (Bit-fields) 跨 Byte 佈局與大小端陷阱、`sizeof` vs `strlen` 字串結束符 `\0` 與函式參數指標退化、Little/Big Endian 判定語法與網路字節序轉換、`static` 關鍵字三大用途與符號表鏈接屬性 (Internal Linkage)、自走車 IMU 姿態感測器 I2C 暫存器自動遞增突發讀取 (Burst Read) 與陀螺儀零點漂移 (Gyro Bias) 校準演算法。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0~1 ms / Beats 90~100% 遠端驗證)

### P1: LeetCode #2595 - Number of Even and Odd Bits
- **考點對照**: 暫存器奇偶 Bit 遮罩操作 (Even Mask `0x55555555`, Odd Mask `0xAAAAAAAA`) 與位元平行分流消元。
- **核心思維**:
  - 利用 `0x55555555` 與 `0xAAAAAAAA` 單週期一次性分離所有奇偶索引的位元。
  - 結合 Brian Kernighan 演算法 `n = n & (n - 1)` 快速消除最右側的 1，常數時間計數。
  - 零提示純白板一次直接 AC！
- **LeetCode 成績**:
  - **狀態**: Accepted (1003/1003 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 10.1 MB
  - **Submission ID**: `2175978888`

---

### P2: LeetCode #448 - Find All Numbers Disappeared in an Array
- **考點對照**: 「群聯三題」缺失數字問題進階版；SSD 韌體 L2P 查表常用之**原地標記法 (In-Place Index Sign Negation)**。
- **核心思維**:
  - 將數值 $x$ 映射至索引 $\text{idx} = |x| - 1$。
  - 若 `nums[idx] > 0` 則翻轉為負數，代表該數字已經出現過；加判斷條件防止重複元素二次翻轉。
  - 第一趟掃描利用 `count--` 直接算出缺失數字精準總數，使 `malloc` 一次到位（0 記憶體浪費）；第二趟掃描將未被標記為負數的索引（`nums[j] > 0`）收集為答案 `j + 1`。
  - 時間複雜度 $O(N)$，額外空間複雜度 $O(1)$。
- **LeetCode 成績**:
  - **狀態**: Accepted (35/35 Passed)
  - **執行時間**: 1 ms (Beats 98%+)
  - **記憶體**: 35.9 MB
  - **Submission ID**: `2176010223`

---

### P3: LeetCode #82 - Remove Duplicates from Sorted List II
- **考點對照**: 鏈結串列指標走訪與全重複節點「整批刪除」、Dummy Node 邊界防禦與主動 `free()` 記憶體管理。
- **核心思維**:
  - **使用者實作版 (快慢前瞻雙指針)**：建立 Stack 虛擬頭節點 `dummy`，快慢指標探測連續重複項，內部迴圈整批拔除並主動 `free` 節點釋放記憶體，修復 `fast != NULL` 邊界防禦後順利 Accepted。
  - **面試標準極簡版 (單指針前瞻探測法)**：
    - `cur` 停在最後一個已確認唯一節點（起點為 `&dummy`），直接看未來的兩步 `cur->next` 與 `cur->next->next`。
    - 若數值相同，記錄 `dup_val`，以 `while (cur->next && cur->next->val == dup_val)` 一路吃掉重複節點並釋放，程式碼緊湊優雅且 100% 免疫空指標。
- **LeetCode 成績**:
  - **狀態**: Accepted (168/168 Passed)
  - **執行時間**: 1 ms (Beats 87%+)
  - **記憶體**: 11.3 MB
  - **Submission ID**: `2176025549`

---

## Part 2: 面試經典致命陷阱題

### Trap 1: 結構體位元欄 (Bit-fields) 跨 Byte 佈局與大小端陷阱
- **問題分析**:
  `struct PacketHeader { uint8_t version : 4; uint8_t type : 4; };`，收到一個 Byte `0x31` (`0011 0001`)。
- **解答與深度推導**:
  1. **數值判定**：在 Little-Endian 架構下，先宣告的 `version` 佔低位元（LSB, Bit 0~3 = `0001` = 1），後宣告的 `type` 佔高位元（MSB, Bit 4~7 = `0011` = 3）。
  2. **禁忌與工業界標準**：
     - ISO C 標準將 bit-field 的排列順序定義為 **Implementation-Defined（由編譯器決定）**，跨編譯器或跨大小端架構時順序極易顛倒。
     - 工業界標準：一律使用 `uint8_t buffer[]` 原始位元組陣列，透過位元移位與遮罩巨集（`&` 和 `>>`）手動解包，保證跨平台行為絕對一致。

---

### Trap 2: `sizeof` vs `strlen` 字串結束符與陣列退化陷阱
- **問題分析**:
  `char str1[] = "hello"; char *str2 = "hello"; char str3[10] = "hello"; void func(char s[10]);`
- **解答與底層剖析**:
  1. `sizeof(str1) = 6`（包含字串結尾隱式空字元 `'\0'`）；`sizeof(str2) = 8`（64-bit 指標變數大小）；`sizeof(str3) = 10`（固定長度字元陣列）。
  2. 呼叫 `func(str3)` 時，印出 `sizeof(s) = 8, strlen(s) = 5`。
     - 原因：**陣列作為函式參數時會退化為指標（Array-to-pointer decay）**，方括號內的 `10` 會被編譯器忽略，因此 `sizeof(s)` 印出的是指標大小（8 bytes）。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: Little-Endian vs Big-Endian 的判定與網路字節序轉換
1. **動態判定標準實作 (C 語言指標轉型法)**：
   ```c
   uint32_t a = 1;
   uint8_t *b = (uint8_t *)&a;
   if (*b == 1) {
       // Little-Endian (低位元 0x01 存放在最低記憶體位址)
   } else {
       // Big-Endian
   }
   ```
2. **網路字節序 vs 主機字節序本質**：
   - 網路協定（TCP/IP）使用 Big-Endian（高位元組在前），符合人類直覺閱讀順序；
   - 嵌入式 CPU（ARM / x86）採用 Little-Endian，便於 ALU 從 LSB 開始單週期累加進位，且型別降轉時起始位址無需偏移。
   - 若未轉換（`htonl`/`ntohl`），多位元組資料直接反轉錯亂（如 `0x12345678` 變 `0x78563412`），導致封包長度與協議校驗和徹底崩潰。

---

### Q2: `static` 關鍵字在 C 語言的三大用途與符號表鏈接屬性
1. **函式內區域變數 (Local Static)**：
   - **生命週期**：靜態生命週期，貫穿整個程式執行期間（直到關機或程式終止）。
   - **預設初始值與區段**：未初始化時由編譯器預設清零為 0，存放於 **`.bss` 區段**；若賦予非零初始值則存放於 **`.data` 區段**（絕非 Stack）。
2. **全域變數與內部函式 (Global / Function Static)**：
   - **鏈接屬性 (Internal Linkage)**：將符號限制在當前編譯單元（`.c` 檔案）內部，不輸出至全域符號表，外部檔案無法透過 `extern` 引用。
   - **大型韌體開發三大價值**：
     - **資訊隱藏與封裝性**：避免外部模組隨意調用內部未受保護的底層驅動函式。
     - **杜絕重名衝突**：不同模組可各自定義內部輔助函式（如 `static void init(void)`），避免 Linker 報符號重複定義（Multiple Definition Error）。
     - **激進編譯最佳化**：編譯器確信該函式無外部調用者，可大膽將其就地展開為內聯（Inline），節省函式調用開銷。

---

## 🚗 自走車專案實務科普

### 今日主題：自走車 IMU 姿態感測器「I2C 暫存器自動遞增突發讀取 (Burst Read)」與「陀螺儀零點漂移 (Gyro Bias) 校準」

#### 1. 面試痛點場景（廣達、和碩、台積電 IMC 面試常客）
面試官提問：
> *「在你的智慧車專案中，使用 IMU（如 MPU-6050）感測車體旋轉角速度與加速度。為什麼車子在平地上靜止不動時，讀出來的角度依然會慢慢自己偏轉（Drift）？此外，6 軸數據（$X, Y, Z$ 加速度與角速度）共有 14 個暫存器位元組，你的底層 I2C 驅動是如何高效且無時差讀取它們的？」*

#### 2. 底層 I2C 暫存器自動遞增突發讀取 (Burst Read / Auto-increment)
* **初學者寫法的致命缺陷**：
  - 分開呼叫 14 次單字節讀取函式，每次都需要送出：`START` $\to$ `Device Addr (W)` $\to$ `Reg Addr` $\to$ `START` $\to$ `Device Addr (R)` $\to$ `Read Byte` $\to$ `STOP`。
  - 總通訊時間高達數毫秒，浪費 70% 以上的 I2C 頻寬；
  - **時序不同步（Data Skew）**：讀取 $X$ 軸與 $Z$ 軸之間相差了數個毫秒，在高速運動下姿態計算會嚴重失真。
* **工業級解決方案：I2C 暫存器指針自動遞增 (Auto-increment Burst Read)**：
  - MPU-6050 內部設計有硬體指針自動遞增邏輯。
  - 只需要發送一次起始暫存器位址 `0x3B`（`ACCEL_XOUT_H`），接著主機連續讀取 14 個 bytes，每讀完一個 byte 送出 `ACK`，感測器內部指針自動加 1：
    ```c
    uint8_t imu_raw_buf[14];
    // 單次 I2C DMA 突發讀取 14 個位元組（0x3B ~ 0x48）
    HAL_I2C_Mem_Read_DMA(&hi2c1, MPU6050_ADDR, 0x3B, I2C_MEMADD_SIZE_8BIT, imu_raw_buf, 14);
    ```
  - **大小端拼接（呼應今日 Part 3 考點！）**：
    MPU-6050 暫存器高位元在低位址（Big-Endian），讀進 STM32（Little-Endian）後必須透過位元移位合成 16-bit 帶符號整數：
    ```c
    int16_t accel_x = (int16_t)((imu_raw_buf[0] << 8) | imu_raw_buf[1]);
    int16_t gyro_z  = (int16_t)((imu_raw_buf[12] << 8) | imu_raw_buf[13]);
    ```

#### 3. 陀螺儀零點漂移（Zero-rate Drift / Bias）校準演算法
* **為什麼會自發漂移？**
  - MEMS 陀螺儀內部利用微機械梳齒振動結構偵測科氏力，受製程機械不對稱、封裝殘餘應力與環境溫度影響，靜止時輸出永遠不會是完美的 0，而是帶有一個直流偏置（DC Offset Bias）。
  - 對角速度做時間積分 $\theta = \int \omega \, dt$ 時，微小的常數偏移會隨時間線性放大，造成車輛定位發散。
* **開機靜態校準標準程序 (Power-on Calibration SOP)**：
  1. 自走車開機時，在系統啟動的前 2 秒維持車體完全靜止。
  2. 透過定時器採樣 1000 筆陀螺儀原始數據累加求平均：
     $$\text{Bias}_Z = \frac{1}{1000} \sum_{i=1}^{1000} \text{gyro\_z}_i$$
  3. 進入主控制迴圈後，每次採樣即時扣除靜態偏置：
     $$\omega_{\text{actual}} = (\text{gyro\_z} - \text{Bias}_Z) \times \text{Sensitivity}$$
  4. 經由此靜態校準，可消除 98% 以上的累積零漂，保證自走車循跡走直線穩定不偏移！
