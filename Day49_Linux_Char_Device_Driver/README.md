# Day 49: 總複習 Part 9 - Linux Kernel 驅動程式 (Char Device 字符設備、copy_from_user 防禦與 ioctl 巨集)

今天是 10 天特訓總複習的第九天，我們深入複習了 Linux 字符驅動程式 (Char Device Driver) 的核心架構、`file_operations` 函數指標掛載表、Major/Minor 設備號、`copy_from_user` 核心安全防禦與 `ioctl` 巨集編碼與 `unlocked_ioctl` 歷史演進。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【核心與使用者隔離題：copy_from_user / copy_to_user】
* **原問題**：為何不能用 `memcpy`？`copy_from_user` 做了哪些防禦？回傳值 0 與大於 0 的物理意義？
* **解析與解答**：
  1. **SMAP 硬體防線與 Exception Table 防禦**：直用 `memcpy` 會觸發 CPU 硬體 SMAP/PAN 安全防線。若 User 傳入壞位址在核心空間踩到 Page Fault，普通 `memcpy` 會引發 **Kernel Panic / OOPS 死機**。`copy_from_user` 會暫時解開 SMAP 並具備 **Exception Table (例外修復表)**，防範核心當機。
  2. **`access_ok` 檢查與回傳值**：會呼叫 `access_ok` 確保位址真位於 User Space。**回傳 0 代表 100% 成功**；**回傳 N > 0 代表還有 N 個 Bytes 沒能複製成功**。

### 題目 2【驅動結構與介面題：file_operations 結構體與 VFS】
* **原問題**：`struct file_operations` 充當什麼角色？Major Number 與 Minor Number 代表什麼？
* **解析與解答**：
  1. **VFS 函數指標表 (Function Pointer Table)**：充當 User Space 系統呼叫與 Kernel 驅動間的橋樑。當 User 呼叫 `open()`/`read()`/`write()` 時，Linux VFS 順著該結構體掛載的函數指標跳轉執行驅動內對應的 `my_dev_write()`。
  2. **主次設備號**：**Major Number (主設備號)** 代表驅動程式類型（告訴 VFS 該找哪一個 Driver）；**Minor Number (次設備號)** 代表同驅動下的具體第幾號個體 node (如 `/dev/ttyUSB0` vs `/dev/ttyUSB1`)。

### 題目 3【控制介面題：ioctl 控制與巨集】
* **原問題**：`ioctl` 為什麼不能寫死數字？`_IOW`/`_IOR` 包裝了什麼？`.unlocked_ioctl` 為何加上 `unlocked`？
* **解析與解答**：
  1. **全球獨一無二指令編碼**：寫死數字易引發不同驅動間指令衝突誤搶。`_IOW(type, nr, size)` 包裝了 **魔術數字 (Magic Number)、指令序號、傳輸方向與資料結構大小**，組成 32-bit 全球唯一的安全指令。
  2. **取消 Big Kernel Lock (BKL)**：舊版 Linux 的 `ioctl` 會自動幫全系統上核心大鎖 (BKL)，嚴重拖慢多核效能。現代改為 `.unlocked_ioctl`，代表**核心不再自動幫驅動上鎖**，若有競態條件 (Race Condition) 需由驅動開發者自行使用 Mutex 保護。

---

## 🎯 專題加碼：《韌體工程師 0x10 個問題》高頻考題抽考總整理

### 1. `const` 與 `*` 指標四大組合終極決策樹
以星號 `*` 為核心分界線：
* **`const int * p` 或 `int const * p`** ➡️ `const` 在 `*` 左邊：**內容唯讀 (`*p` 不能修改，但 `p = &b` 位址可改)**。
* **`int * const p`** ➡️ `const` 在 `*` 右邊：**位址唯讀 (`p` 不能修改，但 `*p = 30` 內容可改)**。
* **`const int * const p`** ➡️ `*` 兩邊皆有 `const`：**內容與位址皆為唯讀**。

### 2. Struct Padding 與 Endianness
* **對齊算術**：`struct SensorA { uint8_t; uint32_t; uint8_t; }` 經 4B 對齊與結尾填補後大小為 **12 Bytes**；調整順序為 `struct SensorB { uint8_t; uint8_t; uint32_t; }` 可將同型態縮緊，大小縮減為 **8 Bytes** (節省 33% 記憶體)。
* **端序 (Little-Endian)**：`count = 0x12345678` 在小端序 (ARM/x86) 中遵循「小位元組 (LSB) 存於小位址」，故記憶體 `[0]` 存放低位元組 **`0x78`**。

### 3. FSM 狀態機與 C99 指定初始化語法
* **時間複雜度**：傳統 `switch-case` 隨著狀態增加複雜度為 $O(N)$；使用 **`fsm_table[state]()` 函式指標陣列** 可發動 ARM 間接跳轉指令，達成 **$O(1)$** 秒殺執行。
* **C99 Designated Initializer 語法**：
  ```c
  typedef void (*state_func_t)(void); // ANSI C typedef 函式指標型態
  
  static const state_func_t fsm_table[STATE_MAX] = {
      [STATE_IDLE]    = handle_idle,   // [0] 格子存放 handle_idle 函式進入點位址
      [STATE_TAKEOFF] = handle_takeoff // [1] 格子存放 handle_takeoff 函式進入點位址
  };
  ```
  `[STATE_IDLE]` 係利用 Enum 數值作陣列索引，將對應之函式記憶體位址存入該格子中，免除傳統順序錯亂風險。
