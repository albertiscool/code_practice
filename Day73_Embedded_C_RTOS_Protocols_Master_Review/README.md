# Day 73: 嵌入式 C 語言記憶體對齊、FreeRTOS 即時架構與四大通訊協定終極全攻略

本篇為韌體工程師面試中最核心的「三大底層支柱」系統化總複習講義：
1. **嵌入式 C 語言高階機制**（Struct 記憶體對齊與 Padding 計算、關鍵字矩陣 `static` / `const` / `volatile`、記憶體分區 ELF Layout）。
2. **FreeRTOS 與 ARM 微架構即時系統**（排程器原理、優先級反轉與繼承、Mutex vs Semaphore vs Task Notification、NVIC 中斷優先級與臨界區）。
3. **四大硬體通訊協定與 DMA 底層**（UART、I2C、SPI、DMA 與快取一致性深層比對）。

---

## 🏛️ 支柱一：嵌入式 C 語言底層機制與記憶體佈局

---

### 1. 結構體記憶體對齊 (Struct Memory Alignment & Padding)

微控制器（如 32-bit ARM）為了硬體匯流排存取效率，要求數據位址必須對齊在其型態大小的整數倍上（Natural Alignment）。

#### 📌 對齊 3 大黃金法則：
1. **成員對齊**：每個成員的起始位址必須是「該成員型態大小」的整數倍。
2. **結構體總大小對齊**：結構體的總位元組數必須是「結構體中最大成員型態大小」的整數倍（不足要補 Padding）。
3. **巢狀結構體對齊**：內部結構體的對齊基準為該內部結構體的「最大純量成員」。

#### 🔍 經典面試實例計算 (32-bit MCU)：
```c
struct ExampleA {
    char a;      // 1 Byte (位址 0)
                 // [Padding 3 Bytes] (位址 1, 2, 3)
    int b;       // 4 Bytes (位址 4~7)
    short c;     // 2 Bytes (位址 8~9)
                 // [Padding 2 Bytes] (位址 10~11，因總大小需為 int(4) 的倍數)
}; // 總大小 = 12 Bytes！

// 💡 優化成員排列順序 (由大到小或由小到大緊湊排列)：
struct ExampleA_Optimized {
    int b;       // 4 Bytes (位址 0~3)
    short c;     // 2 Bytes (位址 4~5)
    char a;      // 1 Byte  (位址 6)
                 // [Padding 1 Byte] (位址 7)
}; // 總大小縮減至 8 Bytes！節省 33% RAM！
```

* **禁止對齊語法**：`#pragma pack(1)` 或 `__attribute__((packed))`（常用於通訊封包 Header，但會降低 CPU 存取效率）。

---

### 2. C 語言程式記憶體五大分區 (ELF Memory Layout)

| 記憶體分區 | 存放內容 | 儲存媒介 | 生命週期 |
| :--- | :--- | :--- | :--- |
| **.text (Code)** | 唯讀程式碼、中斷向量表、常數字串 | Flash (ROM) | 晶片上電到斷電 |
| **.rodata** | `const` 全域變數 | Flash (ROM) | 晶片上電到斷電 |
| **.data** | **有初始值** 的全域變數、`static` 變數 | RAM (開機由 Flash 複製) | 程式執行期間永久 |
| **.bss** | **未初始值** 或初始為 0 的全域/`static` 變數 | RAM (開機清為 0) | 程式執行期間永久 |
| **Heap (堆)** | `malloc()` / `free()` 動態配置 | RAM | 手動管理 (注意碎片化) |
| **Stack (棧)** | 區域變數、函式呼叫參數、中斷 Context | RAM | 函式/中斷離開時自動釋放 |

---

### 3. C 語言四大關鍵字矩陣 (`static`, `const`, `volatile`, `extern`)

* **`static` 的 3 種作用域**：
  1. **修飾區域變數**：生命週期由函式呼叫擴展為「全域永久」，存於 `.data` 或 `.bss`，函式退出數值不消失。
  2. **修飾全域變數**：將作用域限制在「當前 `.c` 檔案內」，其他檔案無法透過 `extern` 存取（實現資訊隱藏封裝）。
  3. **修飾函式**：內部私有函式，僅當前檔案可呼叫，方便編譯器進行 `inline` 內聯優化。
* **`const` 的指標 4 大組合**：
  * `const int *p;` ➔ 指向的內容不可改，指標位址可改。
  * `int * const p;` ➔ 指標位址不可改（常數指標），指向的內容可改。
  * `const int * const p;` ➔ 內容與位址皆不可改。

---

## ⚡ 支柱二：FreeRTOS 與 ARM Cortex-M 即時系統架構

---

### 1. 任務通訊與同步機制大比拼 (IPC Mechanisms)

| 機制 | 核心用途 | 是否攜帶數據 | ISR 安全版 API | 效能/RAM 開銷 |
| :--- | :--- | :--- | :--- | :--- |
| **Queue (佇列)** | 多任務間傳遞數據封包 (FIFO) | 是 (值拷貝 `memcpy`) | `xQueueSendFromISR` | 較高 (需配置 Buffer) |
| **Binary Semaphore** | 任務同步、事件觸發 (ISR 喚醒 Task) | 否 (只有 0 與 1) | `xSemaphoreGiveFromISR` | 極低 |
| **Counting Semaphore** | 資源計數 (如環形緩衝區可用資源) | 否 (整數計數) | `xSemaphoreGiveFromISR` | 極低 |
| **Mutex (互斥鎖)** | 共享資源互斥保護 (如共用 SPI/I2C) | 否 (具所有權 Ownership) | ❌ **嚴禁在 ISR 呼叫** | 低 (支援優先級繼承) |
| **Task Notification** | 輕量級一對一通知 (取代 Semaphore) | 可帶 32-bit 數值 | `vTaskNotifyGiveFromISR` | **零額外 RAM 開銷 (最快！)** |

---

### 2. 優先級反轉 (Priority Inversion) 與優先級繼承 (Priority Inheritance)

* **什麼是優先級反轉？**：
  低優先級 Task A 拿到 Mutex 鎖，高優先級 Task C 想要鎖進入 Block。此時中優先級 Task B（不需要鎖）搶占 Task A 執行，導致 **Task C 被無關的 Task B 無限期延遲**！
* **解決方法：優先級繼承 (Priority Inheritance)**：
  當 Task C 嘗試獲取已被 Task A 持有的 Mutex 時，FreeRTOS 會 **暫時將 Task A 的優先級提升至 Task C 的等級**，讓 Task A 快速執行完畢並釋放 Mutex，釋放後 Task A 恢復原優先級，Task C 立即獲得鎖！
* *(注意：Binary Semaphore 不支援優先級繼承，互斥鎖 Mutex 才支援！)*

---

### 3. 臨界區 (Critical Section) vs 中斷遮蔽

* **`taskENTER_CRITICAL()`**：
  * 底層透過改寫 `BASEPRI` 暫存器，遮蔽所有優先級低於 `configMAX_SYSCALL_INTERRUPT_PRIORITY` 的中斷。
  * **使用原則**：執行時間必須極短（數微秒內），嚴禁在臨界區內呼叫任何可能 Block 的 API！

---

## 📡 支柱三：四大硬體通訊協定與 DMA 深層比對

---

### 1. UART vs I2C vs SPI 協定全方位規格對比

| 特性 | **UART** | **I2C** | **SPI** |
| :--- | :--- | :--- | :--- |
| **同步方式** | 非同步 (Asynchronous，需約定 Baudrate) | 同步 (Synchronous，由 SCL 提供時脈) | 同步 (Synchronous，由 SCK 提供時脈) |
| **線數 (Wires)** | 2 條 (TX, RX) + GND | 2 條 (SDA, SCL) + GND | 4 條 (MOSI, MISO, SCK, CS) + GND |
| **電氣架構** | 推挽 (Push-Pull) | **開漏 (Open-Drain) + 外部上拉電阻** | 推挽 (Push-Pull) |
| **傳輸模式** | 全雙工 (Full-Duplex) | 半雙工 (Half-Duplex) | 全雙工 (Full-Duplex) |
| **定址能力** | 點對點 (Point-to-Point) | 軟體 7-bit / 10-bit 位址定址 (多從機) | 硬體 CS (Chip Select) 獨立片選腳位 |
| **傳輸速度** | 典型 115.2 kbps ~ 1 Mbps | 標準 100 kHz / 快速 400 kHz / 3.4 MHz | 高速 **10 MHz ~ 50+ MHz** |
| **應答機制** | 無硬體 ACK (需軟體 Checksum) | **硬體 ACK / NACK (第 9 個 Clock)** | 無硬體 ACK |

---

### 2. I2C 開漏架構與時脈伸展 (Clock Stretching)

* **為什麼 I2C 必須用 Open-Drain + 上拉電阻？**
  1. **避免匯流排短路衝突 (Bus Contention)**：多個設備同時拉高/拉低時不會短路燒毀晶片（實現「線與 (Wired-AND)」邏輯）。
  2. **支援多主機仲裁 (Multi-Master Arbitration)**。
* **什麼是時脈伸展 (Clock Stretching)？**
  當從機 (Slave) 處理速度太慢時，從機可以主動將 **SCL 線強行拉住維持在 LOW 電位**，強迫主機 (Master) 暫停發送下一個 Byte，直到從機準備好釋放 SCL 為止。

---

### 3. SPI 的 4 種時脈極性與相位模式 (CPOL & CPHA)

* **CPOL (Clock Polarity)**：閒置時 SCK 的電位狀態（`0`: 閒置為 LOW，`1`: 閒置為 HIGH）。
* **CPHA (Clock Phase)**：資料採樣的時脈邊緣（`0`: 第 1 個跳變沿採樣，`1`: 第 2 個跳變沿採樣）。
* **Mode 0 (CPOL=0, CPHA=0)**：最常用模式，閒置 LOW，上升沿採樣、下降沿切換數據。

---

### 4. DMA (Direct Memory Access) 與 Ring Buffer 協同架構

* **DMA 核心價值**：由專屬硬體 DMA Controller 負責周邊暫存器與 RAM 之間的資料搬移，**CPU 0% 負擔**。
* **高階接收架構：UART DMA Circular Mode + IDLE Line Interrupt**：
  1. DMA 在背景以循環模式持續接收 UART 資料寫入 RAM Buffer。
  2. 當發送端一串封包發送完畢，匯流排閒置 1 個字元時間，硬體觸發 **IDLE 空閒中斷**。
  3. CPU 僅在 IDLE 中斷觸發時進來讀取 DMA 計數器（`DMA_CNDTR`），將新資料推進 Ring Buffer。
  4. **達成工業級「不定長度封包、零封包遺失、極低 CPU 佔用」的高效串口驅動！**
