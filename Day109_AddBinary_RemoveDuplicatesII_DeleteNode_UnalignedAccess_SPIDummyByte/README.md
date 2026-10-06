# Day 109: 群聯 Phison ＋ 電子五哥 韌體實戰模式 (二進位全加器 × 原地去重 K 格不變量 × 鏈結替身覆寫 × 未對齊 UsageFault × SPI Dummy Byte 與 BSY 陷阱)

## 📑 今日訓練概覽
- **P1 (硬體全加器與大數字串加法)**: LeetCode #67: Add Binary (Easy) - `while (i >= 0 || j >= 0 || carry > 0)` 單一迴圈通殺長短字串與殘留進位 + 雙指標原地反轉 (0 ms, Beats 100.00%)
- **P2 (陣列原地雙指標與數學不變量)**: LeetCode #80: Remove Duplicates from Sorted Array II (Medium) - 往前檢查 K 格不變量 `nums[fast] != nums[slow - 2]`，嚴格 $O(1)$ 額外空間通用解 (Accepted)
- **P3 (鏈結串列指標與替身覆寫)**: LeetCode #237: Delete Node in a Linked List (Easy) - 無 `head` 前提下的「李代桃僵」數值覆寫 + `free(temp)` 杜絕記憶體外洩 (Accepted)
- **自走車專案實務科普 (第 109 講)**:
  - 超音波避障 HC-SR04 `pulseIn()` 阻塞死等 30ms 導致馬達 PID 控制週期腰斬與盲奔撞牆痛點
  - STM32 定時器輸入捕獲（Timer Input Capture）雙邊緣中斷非阻塞測距架構
- **0x10 韌體架構實戰情境題**:
  1. **非對齊記憶體存取（Unaligned Memory Access）引發 UsageFault**:
     - `__attribute__((packed))` 強制移除 Padding 導致 32-bit 資料落於奇數位址
     - 32-bit `LDR` 跨 Bus Transaction 觸發 ARM 硬體 Fault
     - `memcpy` 展開為 4 次 `LDRB` 與手動位元拼接安全讀取
  2. **SPI 匯流排全雙工與 Dummy Byte 陷阱**:
     - Slave 無時鐘源，Master 必須藉由發送 Dummy Byte 驅動 SCK 產生 8 個時脈
     - `TXE`（填裝區為空）vs `BSY`（砲管發射完畢），過早拉高 CS 導致封包尾端位元截斷（Truncation）
- **職涯與研究所戰略規劃**:
  - 累積題庫已突破 76 題，電子五哥達到碾壓級別，群聯/瑞昱進入白板穩定度與 0x10 情境深化階段
  - Jetson Nano ＋ STM32 異質架構高含金量論文題材剖析（Sim-to-Real RL 邊緣落地、動態 DVFS 能耗排程）

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Add Binary (#67 - Easy)
- **核心思維**:
  - 加法運算必自最低位（LSB，個位數）向最高位（MSB）推進。
  - 單一迴圈 `while (i >= 0 || j >= 0 || carry > 0)` 同時吸收不等長字串與終端進位，避免長短字串窮舉帶來的龐大 if-else 分支。
  - 填入結果後，以雙指標原地反轉。

### P2: Remove Duplicates from Sorted Array II (#80 - Medium)
- **核心思維**:
  - 在非遞減排序陣列中，若允許每個元素最多出現 $K$ 次（本題 $K = 2$）：
  - 候選元素 `nums[fast]` 是否合法的充要條件為：`nums[fast] != nums[slow - K]`！
  - 只要不等於已經驗證合格的倒數第 $K$ 個元素，該元素就絕對不可能成為第 $K + 1$ 個重複項。
  - 此數學不變量使程式碼壓縮至 5 行，並原生通用於任意 $K$ 值。

### P3: Delete Node in a Linked List (#237 - Easy)
- **核心思維**:
  - 缺乏 `head` 指針時，物理上不可能回溯尋找前驅節點 `prev`。
  - 採用「李代桃僵」數值替換法：將下一節點之值拷貝至當前節點（`node->val = node->next->val`），再跳過下一節點（`node->next = node->next->next`）。
  - 主動使用 `free(temp)` 釋放多餘節點，展現韌體工程師防範 Heap 洩漏的素養。

---

## 🚗 自走車實務科普：超音波輸入捕獲與非阻塞避障

### 痛點剖析
- `pulseIn()` 是 CPU 死迴圈阻塞函式，最長等待 30ms。
- 自走車底盤 PID 週期通常為 10ms~20ms，30ms 阻塞將使車輛進入失控盲奔狀態。

### 工業級解決方案
- 觸發：GPIO 輸出 10μs TRIG 脈衝，CPU 立即返回主任務。
- 捕獲：利用定時器 Input Capture 通道，自動於 ECHO 上升沿記錄 $T_1$、下降沿記錄 $T_2$。
- 計算：$\Delta T = T_2 - T_1$，$S = \frac{\Delta T \times 340}{2}$，全程 0% CPU 阻塞。

---

## ⚔️ 0x10 韌體架構實戰情境題深度複盤

### 題目一：非對齊記憶體存取與 UsageFault
- **成因**:
  - `__attribute__((packed))` 強制移除 Padding，導致 32-bit 變數落在非 4 倍數地址（如 0x20000001）。
  - 跨越兩次 32-bit Bus Transaction，低階 CPU（Cortex-M0）或啟用 `UNALIGN_TRP` 的高階架構直接觸發硬體 Fault。
- **安全防護**:
  - `memcpy(&val, ptr, 4)`：編譯器自動拆解為 4 次 `LDRB` 單 Byte 安全讀取與暫存器拼裝。
  - 手動移位拼接：`val = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)`。

### 題目二：SPI 匯流排全雙工與 Dummy Byte / BSY 陷阱
- **Dummy Byte 物理意義**:
  - Slave 為純被動設備無獨立時鐘源；Master 移位暫存器必須送出資料方能驅動 SCK 產生 8 個時脈脈衝供 Slave 回傳數據。
- **TXE vs BSY 翻車現場**:
  - `TXE == 1`：僅代表資料從 TDR 緩衝區掉入 Shift Register 砲管，線路上仍正在發送中。
  - 若此時拉高 CS，最後一個 Byte 將被硬生生腰斬截斷。
  - 正確 SOP：等待 `TXE == 1` 後，必須進一步等待 `BSY == 0`，方可安全拉高片選 CS。
