# Day 98: 一線 IC 設計廠演算法通關、馬達 H 橋死區防護、暫存器極致防禦與 FTL 儲存架構深度科普

今天延續 **Day 97+ 一線 IC 設計廠與系統廠進階戰場模式**，同時解鎖了針對群聯（Phison）、聯詠（Novatek）、慧榮（SMI）模擬技術面試的第一道核心硬實力題，並完整建立了儲存控制晶片（SSD Controller）的 FTL（Flash Translation Layer）全域架構！

---

## 💻 一、 今日白板程式題（進階 3 題全數通過）

### 1. 合併兩個已排序串列 (Merge Two Sorted Lists - LeetCode #21) -> [`p1.c`](./p1.c)
- **難度標籤**：🟡 一線 IC 設計廠 & 系統廠 鏈結串列第一高頻題 | **複雜度**：時間 $O(M+N)$，空間 $O(1)$
- **實作亮點（100 分滿分一次過）**：
  - 完美運用 **Stack 區域變數 Dummy Head (`struct ListNode dummy;`)**，零 `malloc`、零 Memory Leak 風險。
  - 統一了第一個節點與後續節點的穿針引線邏輯，極致簡化邊界條件。

### 2. 環形鏈結串列偵測 (Linked List Cycle - LeetCode #141) -> [`p2.c`](./p2.c)
- **難度標籤**：🟡 聯詠、瑞昱、聯發科 必考經典快慢指針題 | **複雜度**：時間 $O(N)$，空間 $O(1)$
- **實作亮點（100 分滿分一次過）**：
  - 採用 **Floyd's Tortoise and Hare（龜兔賽跑演算法）**，快慢指針相對速度為 1，數學上保證必定相遇、絕不跳過。
  - 防禦嚴密：`while (fast != NULL && fast->next != NULL)` 運用 C 語言短路求值（Short-circuit evaluation），徹底杜絕存取 `fast->next->next` 引發的 Null Pointer HardFault。
  - **進階面試追問（LeetCode #142 環起點）**：相遇時將一指針放回 `head`，兩者皆每次步進 1 步，再次相遇處即為「環的起點」（公式證明：$L = nC - X$）。

### 3. 尋找旋轉排序陣列中的最小值 (Find Minimum in Rotated Sorted Array - LeetCode #153) -> [`p3.c`](./p3.c)
- **難度標籤**：🟡 聯發科、聯詠二分搜尋變形高頻魔王題 | **複雜度**：時間 $O(\log N)$，空間 $O(1)$
- **三大核心盲點檢討（已存檔使用者原始程式碼）**：
  1. **變數 Scope 生命週期**：`mid` 宣告在 `while` 迴圈區塊內，跳出迴圈後 `return nums[mid];` 會導致 MSVC 編譯錯誤（C2065）。
  2. **二分基準錯誤（關鍵）**：以 `nums[left]` 為基準無法區分 `[11, 13, 15, 17]`（未旋轉，min在左）與 `[3, 4, 5, 1, 2]`（旋轉，min在右）。**必須改以「右端點 `nums[right]`」為基準**：
     - 若 `nums[mid] > nums[right]`：證明斷層在右半部，`left = mid + 1`。
     - 若 `nums[mid] <= nums[right]`：證明右半部單調遞增，`nums[mid]` 本身可能是最小值，故 `right = mid`（不能寫 `mid - 1`）。
  3. **收斂終止條件**：配合 `right = mid`，迴圈條件須為 `while (left < right)`，收斂至單一元素即為全域最小值 `nums[left]`。

---

## 🚗 二、 自走車專案實務科普（系統廠 / IC 廠除錯亮點）

### 主題：L298N H 橋馬達驅動器、PWM 頻率選擇 Trade-off 與直通短路（Shoot-Through）死區保護
1. **PWM 頻率的工程權衡**：
   - **太低（< 1 kHz）**：馬達矽鋼片在人耳聽覺區間（$20\text{Hz}\sim20\text{kHz}$）劇烈震盪，產生刺耳高頻嘯叫；且感抗限制導致電流鏈波過大，低速劇烈抖動。
   - **太高（> 20 kHz）**：L298N 為 BJT 達靈頓對架構，切換延遲（$1\sim2\ \mu\text{s}$）過長，晶體頻繁卡在線性放大區，切換熱損耗（$P_{sw} = f \times E_{loss}$）暴增，數分鐘內燒毀晶片。
   - **實務工程選擇**：L298N 折衷於 $8\text{kHz}\sim10\text{kHz}$；若升級 MOSFET 晶片（如 TB6612FNG）可直上 $20\text{kHz}\sim50\text{kHz}$ 完全靜音。
2. **直通短路（Shoot-Through / Cross-Conduction）**：
   - 換向瞬間，因電晶體關閉比開啟慢，若上臂未完全關閉下臂即開啟，將造成 $V_{CC}$ 直接對地短路，瞬間產生數十安培電流，拉垮電源或燒穿 H 橋。
3. **死區時間（Dead-Time）防護**：
   - 在關閉上臂與開啟下臂之間強制插入微秒級延遲（Dead-Time）。STM32 高級定時器（TIM1/TIM8）硬體整合了死區產生器暫存器（`TIMx_BDTR`），免除軟體抖動風險。

---

## 🎙️ 三、 一線 IC 模擬面試（第 1 題實戰）-> [`0x10.c`](./0x10.c)

### 小題 (A)：指標運算優先權與位移量判定（100 分滿分通過）
- `*p++`：後置 `++` 先解引用取得 `0x11223344`，隨後指標位移 4 bytes。
- `(*p)++`：解引用取得 `0x11223344` 賦值，隨後記憶體內部數值加 1（變 `0x11223345`），指標位移 0 bytes。
- `*++p`：指標先步進 4 bytes 指向下一筆，隨後解引用取得 `0x55667788`。
- `++*p`：先將記憶體內部數值加 1（變 `0x11223345`），再將新值賦出，指標位移 0 bytes。

### 小題 (B)：暫存器位元欄位防禦（MISRA-C & 單次原子寫回）
- **致命避坑點**：
  - 巨集名稱與括號間**嚴禁留空格**：`#define REG(val)`，留空格會變成物件巨集引發語法錯誤。
  - C 語言中賦值運算式為右值，禁止串接 `(REG &= mask) |= val`。
  - **硬體雙重寫入毛刺（Glitch Trap）**：不可在匯流排上先寫 `0` 再寫新值；必須在 CPU 暫存器算好後**「單次原子寫回」**。
  - 邊界截斷防護：傳入值必須做 `& 0x0FUL` 遮罩，防止溢位踩爛相鄰暫存器。

---

## 💾 四、 儲存主控核心架構科普：FTL（Flash Translation Layer）

### 1. NAND Flash 三大物理限制
- **Erase-before-Write**：只能將 `1` 寫成 `0`（Program），要變回 `1` 必須抹除（Erase）。
- **粒度不對稱**：讀寫以 **Page**（4KB~16KB）為單位，抹除以 **Block**（2MB~8MB）為單位，無法單獨抹除單一 Page。
- **有限 P/E 壽命**：TLC/QLC 僅有數百至數千次抹寫壽命。

### 2. FTL 四大核心支柱
- **L2P 位址對映（Mapping）與異地更新（Out-of-Place Update）**：
  - 主機發送 LBA，FTL 映射至實體 PBA。資料修改時寫入全新空白 Page，舊 Page 標記為 Invalid。
- **垃圾回收（Garbage Collection, GC）與寫入放大（WAF）**：
  - 挑選 Invalid Page 最多的 Victim Block，將剩餘少數 Valid Page 搬遷至新 Block，再整塊抹除。
  - 搬遷引發額外寫入量，定義寫入放大率：$\text{WAF} = \frac{\text{寫入 NAND 資料量}}{\text{Host 寫入資料量}} \ge 1$。
- **磨損均衡（Wear Leveling, WL）**：
  - **動態 WL**：新資料寫入擦除次數少的年輕 Block。
  - **靜態 WL（魔王級）**：主動將長期不改動的「冷資料（Cold Data）」搬移到老舊 Block，釋放年輕 Block 承受高頻擦寫。
- **TRIM 指令**：作業系統刪除檔案時通知 FTL 直接標記 Invalid，避免 GC 徒勞無功搬運無效資料。

### 3. 面試追問亮點：DRAM-less SSD 的 L2P 管理
- 1TB SSD 的 L2P Table 約需 1GB DRAM。無外掛 DRAM 時：
  - **Cache-based FTL**：完整 Table 放 NAND，控制器小容量 SRAM 僅利用 LRU 快取熱門項目。
  - **HMB（Host Memory Buffer）**：透過 NVMe 協定直接向主機的 Host DRAM 借用 64MB 存放 Table，兼具成本與效能。

---

## 🎥 五、 YouTube 精選推薦 FTL 科普影音

1. **[What's Actually Happening Inside Your SSD?](https://www.youtube.com/watch?v=kYv9j0Z6h_I)**（Tech With Nikola - 🌟 首選推薦動畫神片）
2. **[How an SSD Works — Where Your Files Actually Go](https://www.youtube.com/watch?v=0-tA-g30R48)**（Inside The Detail - 檔案路徑透視）
3. **[Basics of SSD controllers, FTL, NAND](https://www.youtube.com/watch?v=jW2i3u1sM_o)**（Cadence Design Systems - 原廠晶片與韌體架構）
