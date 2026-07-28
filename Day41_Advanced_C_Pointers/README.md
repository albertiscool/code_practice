# Day 41: 總複習 Part 1 - 高階 C 語言指標與函式指標狀態機 (FSM)

今天是 10 天特訓總複習的第一天，我們深入探討了 C 語言高階指標操作、函式指標在 OOP 多型與無人機狀態機中的應用，以及端序 (Endianness) 與指標算術陷阱。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【觀念除錯題：記憶體修改陷阱】
* **原問題**：傳入單重指標 `char *ptr` 至 `allocate_buffer` 內部進行 `malloc`，在 `main` 執行完會如何？
* **解析與解答**：
  1. **會印出 `Failed`**：C 語言為 Pass by Value (傳值)，副程式內的 `ptr` 僅為 `my_ptr` 的區域拷貝，無法修改 `main` 中的 `my_ptr` 本體。
  2. **Memory Leak (記憶體洩漏)**：`malloc(1024)` 在 Heap 成功配置，但指標隨著 `allocate_buffer` 結束而銷毀，導致 Heap 記憶體無法被 `free()`。
  3. **正確修正**：必須改用雙重指標 `char **ptr` (Pass by Address)，於 `main` 中傳入 `&my_ptr`。

### 題目 2【架構設計題：函式指標與 C 語言 OOP 多型】
* **原問題**：Linux VFS 與驅動程式中使用函式指標實作了哪種 OOP 特性？為什麼比 `switch-case` 更易維護？
* **解析與解答**：
  1. **多型 (Polymorphism)**：同一個介面 (`file->f_op->read()`) 呼叫，根據綁定不同的物件/驅動 (SSD / SD卡 / Socket)，運行時展現出完全不同的實現行為。
  2. **開閉原則 (Open-Closed Principle, OCP)**：新增第 51 種新硬體時，不用改動核心框架的任何舊程式碼，直接註冊新驅動即可，實現「對擴充開放、對修改關閉」。

### 題目 3【硬體低階題：指標算術與 Endianness 端序】
* **原問題**：`uint32_t arr[3]`，`(arr + 1)` 往後移動幾 Bytes？`uint8_t *p = (uint8_t *)(arr + 1);` 在 Little-Endian 下 `*p` 印出何值？
* **解析與解答**：
  1. **位址偏移 4 Bytes**：`uint32_t` 佔 32 bits = 4 Bytes，指標 `+ 1` 偏移 $1 \times 4 = 4$ Bytes。
  2. **印出 `0xF0`**：
     * `p` 為 `uint8_t*`，`*p` 僅讀取 1 Byte。
     * Little-Endian 遵循「小（低位元組 LSB）在低位址」，`0x9ABCDEF0` 中的 `0xF0` 被保存在 lowest address，故 `*p` 讀出 `0xF0`。

---

## ⚙️ 核心語法複習：函式指標狀態機 (State Machine)

### 1. `typedef` 簡化語法
```c
typedef void (*StateFunc_t)(void); // 定義 StateFunc_t 為指向 void func(void) 的指標型態
```

### 2. 狀態表與一鍵執行
```c
static StateFunc_t g_state_table[STATE_MAX] = {
    [STATE_IDLE]    = do_state_idle,
    [STATE_TAKEOFF] = do_state_takeoff,
    [STATE_FLYING]  = do_state_flying,
    [STATE_LANDING] = do_state_landing
};

// 在事件迴圈中直接執行 (時間複雜度 O(1)，消滅 switch-case)
g_state_table[current_state]();
```
