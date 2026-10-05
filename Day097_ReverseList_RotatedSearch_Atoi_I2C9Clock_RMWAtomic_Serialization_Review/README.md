# Day 97: 一線 IC 設計廠與系統廠進階戰場模式啟動、自走車 I2C 救援、0x10 戰場情境題

今天正式升級進入 **「Day 97+ 一線 IC 設計廠（聯詠、瑞昱、聯發科）與系統廠進階戰場模式」**！全面攻克 LeetCode Easy~Medium 邊界高頻演算法、深入自走車 I2C 匯流排死鎖之硬體級 9-Clock 救援術，並首度解鎖 0x10 戰場一級實戰連環情境題與跨晶片通訊序列化架構！

---

## 💻 一、 今日白板程式題（進階 3 題全數通過）

### 1. 反轉單向鏈結串列 (Reverse Linked List - LeetCode #206) -> [`p1.c`](./p1.c)
- **難度標籤**：🟡 一線 IC 設計廠 & 系統廠動態指標第一考點 | **複雜度**：時間 $O(N)$，空間 $O(1)$
- **實作亮點**：
  - 三指針滑動（`pre`, `curr`, `next_node`）原地修改。
  - 在變更 `curr->next` 前先以 `next_node` 抓住後繼節點，徹底避免斷鏈引發之 Memory Leak / Segfault。
  - 邊界防禦無懈可擊，一次通過空串列與單節點測試，獲評 **100 分滿分**！

### 2. 搜尋旋轉排序陣列 (Search in Rotated Sorted Array - LeetCode #33) -> [`p2.c`](./p2.c)
- **難度標籤**：🟡 IC 設計廠二分搜尋變形魔王題 | **複雜度**：時間 $O(\log N)$，空間 $O(1)$
- **核心思維**：在旋轉陣列中，以 `mid` 切分後，左半部與右半部**「至少有一半保證是單調遞增（Sorted）的」**。
- **盲點檢討（已存檔使用者原始程式碼）**：
  - 原始寫法僅判斷 `nums[mid] < target` 即草率收縮邊界；但事實上即使左半遞增且 `nums[mid] > target`，`target` 若小於 `nums[left]` 則根本不在左半區間（例如 `[4,5,6,7,0,1,2]`, `target=0`）！
  - **正確解法（雙邊閉區間夾擊）**：
    - 若左半有序：嚴格檢查 `nums[left] <= target && target < nums[mid]`，成立才往左找，否則往右找。
    - 若右半有序：嚴格檢查 `nums[mid] < target && target <= nums[right]`，成立才往右找，否則往左找。

### 3. 字串轉整數 / 簡化版 atoi (String to Integer - LeetCode #8) -> [`p3.c`](./p3.c)
- **難度標籤**：🟡 韌體 CLI 命令、驅動與通訊協定解析必備題 | **複雜度**：時間 $O(N)$，空間 $O(1)$
- **四大經典盲點檢討（已存檔使用者原始程式碼）**：
  1. **ASCII 轉換**：字元 `'0' ~ '9'` 轉數值必須減去 `'0'`（`s[i] - '0'`），否則會取得其 ASCII 編碼（如 `'4'` 是 52）。
  2. **正負號賦值**：最後回傳必須乘上正負符號 `(int)(sign * result)`。
  3. **狀態機終止條件**：依規範，開頭前導空白後若遇非數字非符號字元（如 `"words and 987"`），必須**立即終止回傳 0**，不能無條件遍歷。
  4. **整數溢位反向防禦**：Windows MSVC 平台下 `long` 與 `int` 皆為 32 位元，`result * 10` 在運算前就可能先溢位；必須在乘 10 之前使用反向除法防禦：`if (result > (INT_MAX - digit) / 10)`，或使用 64 位元 `long long` 暫存並截斷至 `INT_MAX` / `INT_MIN`。

---

## 🚗 二、 自走車專案實務科普（系統廠與 IC 廠除錯魔王題）

### 主題：I2C 匯流排死鎖——SDA 永久被拉在 0V（Bus Hang）與 9-Clock 救援術
- **物理死鎖成因**：
  - I2C 為開漏極（Open-Drain）架構，當主機正在讀取 Slave（如陀螺儀 MPU6050），Slave 正在向 SDA 輸出 `0`（將 SDA 拉低到 GND）。
  - 若此時 MCU 發生電源突波重啟（Reset），MCU 暫存器被清空並將 SCL 停在 HIGH；
  - Slave 以為傳輸尚未結束，持續死命拉低 SDA 等待 SCL 時鐘；而重啟後的 MCU 偵測到 SDA 為 LOW，誤判為「Bus Busy 匯流排被佔用」，雙方陷入**永久死鎖（Deadlock）**。
- **軟體 9-Clock 救援 SOP**：
  1. 初始化時若偵測到 SDA 為 LOW，暫不啟動硬體 I2C，將 **SCL 腳位切換為一般 GPIO 輸出模式**。
  2. 軟體手動控制 SCL 連續打出 **9 個 Clock 脈衝**（1 Byte = 8 bits + 1 ACK，最多 9 次 Slave 必釋放 SDA）。
  3. Slave 送完未竟資料後主動釋放 SDA（被上拉電阻拉回 HIGH）。
  4. 主機手動發送合法的 **STOP 訊號**（SCL 為 HIGH 時將 SDA 由 LOW 轉為 HIGH）。
  5. 將腳位切回硬體 I2C 控制器，匯流排在毫秒內毫髮無傷地自動復原！

---

## 🎙️ 三、 0x10 戰場一級・實戰情境連環題 -> [`0x10.c`](./0x10.c)

### 情境 1：中斷與暫存器讀改寫 (RMW) Race Condition 與 ARM 硬體原子操作
- **RMW 組合語言競爭危害**：
  - `PORTA |= (1U << 3);` 在組合語言中為 3 步：`LDR R0, [PORTA]` -> `ORR R0, #(1<<3)` -> `STR R0, [PORTA]`。
  - 若主迴圈在 `LDR` 後被中斷搶佔（Preemption），ISR 執行 `PORTA &= ~(1U << 5)` 關閉蜂鳴器；當 ISR 返回後，主迴圈使用**舊的暫存器快照**執行 `STR`，當場把 ISR 關閉的蜂鳴器再次覆蓋寫回 `1`！
- **ARM Cortex-M BSRR 硬體原子解決方案**：
  - BSRR（Bit Set/Reset Register）為**寫 1 有效（Write-1-effective）**之唯寫暫存器。
  - 低 16 位寫 1 設 HIGH，高 16 位寫 1 設 LOW。
  - 僅需一條組合語言 `STR` 即可生效，不需經歷 `LDR` 讀取，在匯流排硬體週期達到 **100% 原子操作**，無須關閉中斷！

### 情境 2：跨晶片通訊結構體直接 memcpy 的三大雷區與序列化標準
- **三大雷區**：
  1. **跨架構 Padding 對齊失真**：8-bit AVR（如 Arduino）為 1-byte 對齊（封包 7 Bytes），32/64-bit Linux（如樹莓派）強制 4-byte 自然對齊（封包 12 Bytes），直接 memcpy 導致欄位位址全面錯位變成亂碼！
  2. **大小端序（Endianness）顛倒**：Big-Endian 與 Little-Endian 晶片互傳時多位元組資料高低位顛倒。
  3. **原生型別長度不一**：裸寫 `int`/`long`/`enum` 會因編譯器與平台 ABI 產生長度歧異。
- **工業級標準解法**：
  - **解法 A（`#pragma pack(1)`）**：緊湊打包消除 Padding，但需注意在嚴格 RISC/ARM 架構上可能觸發**非對齊存取 HardFault**。
  - **解法 B（手動序列化 / Deserialization - 黃金標準）**：不依賴結構體記憶體布局，手動以固定位移（Big-Endian）打平成 Byte Array 傳輸，兩端以對稱位移還原，100% 免疫 Padding、大小端與 HardFault！
  - **防禦規範**：全面採用 C99 `<stdint.h>` 固定寬度型別、封包內部以 `uint8_t` 取代 `enum`、並加入 `_Static_assert` 於編譯時期強制檢驗大小。

---

## 💼 四、 求職自薦信與職業發展諮詢精華

1. **慧榮科技 (Silicon Motion, SMI) 韌體設計工程師**：
   - 打造針對 NAND Flash 控制晶片、ARM 架構、C 語言指標、助教經歷與 TOEIC 930 之口語化自薦信。
2. **研華科技 (Advantech) FAE 職缺適配性評估**：
   - 完整對照 JD 需求（ARM/Linux 邊緣運算、Edge AI 專案落地、售前 Demo、技術移轉與兩年助教經歷）。
3. **FAE vs AE 角色本質深度對比**：
   - **AE（應用工程師）**：總部實驗室寫公板（Reference Design）、SDK 與 App Note，偏向研發端（Code 佔 70%）。
   - **FAE（現場應用工程師）**：站在前線陪業務面對客戶、做售前 Demo 與現場排查救火，兼具技術與高溝通商業價值（轉型 PM/海外業務跳板）。
