# Day 99: 環形入口偵測、雙指針刪除節點、非單調峰值二分搜尋、UART DMA 空閒中斷、Circular Buffer 與 I2C 物理層除錯

今天來到 **Day 99（進階戰場模式倒數衝刺）**！演算法部分全數攻克鏈結串列快慢指針、雙指針間距滑動窗口與非單調陣列二分搜尋；自走車科普切入高頻通訊架構 UART DMA + IDLE Line；模擬面試則深入儲存/通訊靈魂結構 Circular Buffer 手刻驗證與 I2C 物理層 RC 上拉電阻除錯！

---

## 💻 一、 今日無提示純白板程式題（進階 3 題全數通過）

### 1. 環形鏈結串列入口點偵測 (LeetCode #142 - Linked List Cycle II) -> [`p1.c`](./p1.c)
- **難度標籤**：🟡 聯詠、瑞昱、聯發科 必考經典快慢指針終極追問題 | **複雜度**：時間 $O(N)$，空間 $O(1)$
- **核心數學原理**：
  - 相遇時快指針走的距離是慢指針的 2 倍：$2(L + X) = L + nC + X \implies L = nC - X$。
  - 將 `slow` 放回 `head`，雙方同步每次走 1 步，再次相遇處即為進入環的入口節點。
- **無環串列邊界盲點檢討（已存檔使用者原始程式碼）**：
  - 原始寫法在無環情況下產生「邏輯穿透（Fall-Through）」：
    1. 在單節點無環 `[1]`：第一個迴圈未進入，直接穿透至第二階段，因 `slow == fast == head` 跳過迴圈，誤將節點 1 當成入口回傳。
    2. 在多節點無環 `[1, 2, 3]`：走到底時 `fast` 已經是 `NULL`，穿透至第二個迴圈存取 `fast->next` 造成空指標解引用當機。
  - **標準解法**：將第二階段入口搜尋直接嵌套於 `if (slow == fast)` 判斷之內，外層正常走完自然回傳 `NULL`。

### 2. 刪除鏈結串列倒數第 N 個節點 (LeetCode #19 - Remove Nth Node From End of List) -> [`p2.c`](./p2.c)
- **難度標籤**：🟡 一線 IC 廠 & 系統廠 動態指標滑動窗口必考題 | **複雜度**：時間 $O(N)$，空間 $O(1)$
- **實作亮點（100 分滿分一次過）**：
  - 完美運用 **Stack 區域變數 Dummy Head (`struct ListNode dummy; dummy.next = head;`)**。
  - 雙指針維持固定距離 $n + 1$ 步（`slow = &dummy`, `fast` 先走 $n$ 步）。
  - 單次遍歷（One-pass）：當 `fast` 抵達末端 `NULL` 時，`slow` 不偏不倚剛好停在被刪除節點的前驅節點，刪除頭節點也無須額外 `if-else`。

### 3. 尋找峰值 (LeetCode #162 - Find Peak Element) -> [`p3.c`](./p3.c)
- **難度標籤**：🟡 聯發科、聯詠非單調二分搜尋經典高頻題 | **複雜度**：時間 $O(\log N)$，空間 $O(1)$
- **坡度二分核心概念（爬山法）**：
  - 題目條件保證邊界外為 $-\infty$ 且相鄰元素不相等。
  - **盲點檢討（已存檔使用者原始程式碼）**：原始實作中大小於符號相反（`nums[mid] > nums[mid+1]` 誤設 `left = mid + 1`），下坡向右走反而走入低窪處（尋找谷底而非峰值）。
  - **正確範式**：
    - `nums[mid] < nums[mid + 1]`：正處於「上坡」，順著上坡向右爬必有峰頂 $\implies left = mid + 1$。
    - `nums[mid] > nums[mid + 1]`：正處於「下坡」，左側必有峰頂或 `mid` 本身即是頂點 $\implies right = mid$。
    - 搭配 `while (left < right)` 收斂，嚴格達成 $O(\log N)$。

---

## 🚗 二、 自走車專案實務科普（面試高頻殺手題）

### 主題：樹莓派與 MCU 雙晶片通訊——UART DMA + IDLE Line（空閒中斷）架構
1. **傳統 Byte-by-byte 中斷的致命缺陷**：
   - 在 115200 鮑率下每 $87\ \mu\text{s}$ 進一次中斷；921600 鮑率下每 $10\ \mu\text{s}$ 進一次。
   - ARM Cortex-M 每次中斷壓棧/出棧耗費大量週期，頻繁 Context Switch 榨乾 CPU，導致馬達控制時序抖動（Jitter）。
2. **UART DMA + IDLE 空閒中斷工作原理**：
   - **DMA 靜默搬運**：UART 每收 1 Byte 由 DMA 自動搬移至 RAM 緩衝區，全程 0% CPU 介入。
   - **IDLE Line 判定不定長封包**：當 RX 腳位維持 HIGH 超過 1 個 Frame 時間（空閒），觸發一次 `UART_IT_IDLE`。
   - **長度計算**：CPU 在單一中斷中讀取 `DMA_CNDTR` 計算長度（$\text{Length} = \text{BufferSize} - \text{DMA\_CNDTR}$），以極低負載實現不定長封包解析。

---

## 🎙️ 三、 0x10 一線科技業模擬面試實戰

### 1. 儲存核心資料結構：手刻 Circular Buffer -> [`ringbuffer.c`](./ringbuffer.c)
- **滿與空歧義消除**：
  - 犧牲一格法（Waste 1 Slot）：判斷滿為 `(head + 1) % SIZE == tail`，實體容量折損 1 Byte。
  - 計數器法（Count Flag）：維護 `count` 變數，達成 100% 空間利用（`count == SIZE` 為滿，`count == 0` 為空）。
  - 自然溢位法（Linux `kfifo`）：利用整數自然溢位與遮罩，實現 Lock-free 聖經架構。
- **除法器與運算週期優化**：
  - 限制容量為 2 的冪次方，以 **`& (BUFFER_SIZE - 1)`** 取代 `%`。
  - 在缺乏硬體除法器的架構（ARM Cortex-M0/M0+）上，將數十週期的軟體除法降為 1 週期 `ANDS` 指令。
- **程式碼盲點檢討（已存檔使用者原始程式碼）**：
  - 修正函式標頭分號語法錯誤（C2449）。
  - 修正函式指針呼叫陷阱：`if (ring_buffer_is_full)` 為函式位址判定（永遠為 true），修正為帶引數呼叫 `if (ring_buffer_is_full(rb))`。
  - 調正 `head`（寫入 / Push）與 `tail`（讀取 / Pop）之語義慣例，通過 8 大步驟完整單元測試。

### 2. 系統廠整合與硬體電路除錯：I2C 物理層上拉電阻與示波器圓角排查
- **Open-Drain 物理特性**：晶片內部僅有下臂接地 NMOS，高電位全靠外部上拉電阻 $R_p$ 對寄生電容 $C_b$ 充電。
- **電阻選用 Trade-off**：
  - **$R_p$ 過大（> 10 kΩ）**：RC 時間常數過大，上升時間 $t_r$ 超出規範（> 300ns），高電位被削成圓弧形，未達 $V_{IH}$（2.31V）即被拉低，引發 Setup Time 違規與 NACK 逾時。
  - **$R_p$ 過小（< 1 kΩ）**：沉降電流 $I_{OL} = \frac{V_{DD}}{R_p}$ 暴增，墊高低電平 $V_{OL}$ 削弱抗雜訊容限，並大幅增加功耗甚至燒毀 NMOS。
- **示波器波形圓角之韌體 2 大急救大招**：
  1. **開啟 MCU 內部弱上拉（Internal Pull-up）**：與外部電阻並聯（$R_{eq} = R_{ext} \parallel R_{int}$），等效電阻降低約 20%，瞬間拉陡上升斜率。
  2. **韌體主動降速（Fallback to 100 kHz）**：時鐘週期拉長 4 倍，規範放寬至 1000ns，使圓弧波形有充裕時間達到 $V_{IH}$ 穩定取樣。
