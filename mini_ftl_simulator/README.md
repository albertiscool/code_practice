# Mini FTL (Flash Translation Layer) 固態硬碟主控韌體模擬器

本專案是一個專為 **儲存控制晶片廠（群聯 Phison、慧榮 SMI）韌體工程師（FW/Algorithm）技術面試** 打造的 C 語言迷你 FTL 模擬器。

透過純 C 語言完整實作了現代 SSD 控制晶片最核心的底層邏輯：**NAND Flash 物理限制抽象、Page-level L2P 位址對映、異地更新、Greedy 垃圾回收（GC）、TRIM 命令支援與寫入放大（WAF）動態統計**。

---

## 🏗️ 系統架構圖 (System Architecture)

```
[ Host Layer (主機作業系統端) ]
       │
       │ 發送讀寫請求: ftl_write(LBA, data), ftl_read(LBA), ftl_trim(LBA)
       ▼
[ FTL 核心韌體層 (Flash Translation Layer) ]
  ├── 1. L2P Mapping Table   : 邏輯位址 (LBA) 映射至 實體位址 (PBA: Block, Page)
  ├── 2. Out-of-Place Update : 舊資料標記 INVALID，新資料寫入全新 FREE Page
  ├── 3. Garbage Collection    : 剩餘空間不足時，用 Greedy 貪婪演算法挑選垃圾最多的 Victim Block，
  │                              搬移有效資料、整塊抹除，並計算寫入放大率 WAF！
  └── 4. TRIM Command Support  : 主機刪除檔案時通知 FTL 直接標記 INVALID，降低 GC 負擔
       │
       │ 發送底層晶片指令: nand_read, nand_program, nand_erase
       ▼
[ Physical NAND Flash 模擬層 ]
  ├── 模擬 4 個 Blocks (每個 Block 包含 4 個 Pages，共 16 物理頁)
  ├── 嚴格遵守物理鐵律：
  │     - 寫入前必須先抹除 (Erase-before-Write，不可覆寫非 FREE 頁面)
  │     - 抹除只能以整個 Block 為單位進行
  └── 物理抹寫次數統計 (Erase Count / WAF 指標)
```

---

## 📂 專案檔案結構

- [`nand_flash.h`](./nand_flash.h) / [`nand_flash.c`](./nand_flash.c)：實體 NAND Flash 物理抽象層（模擬 Page 狀態、Erase-before-Write 防禦、終端機圖形化 Block 佈局）。
- [`ftl.h`](./ftl.h) / [`ftl.c`](./ftl.c)：FTL 核心韌體層（L2P 對映表、異地更新、Greedy 垃圾回收、TRIM、WAF 統計）。
- [`main.c`](./main.c)：SSD 完整生命週期展示驅動（循序寫入 $\to$ 隨機覆寫 $\to$ 自動 GC $\to$ 資料校驗 $\to$ TRIM 釋放 $\to$ WAF 報告）。

---

## 🛠️ 編譯與執行方式

在 Windows MSVC 開發者環境下直接編譯：

```powershell
cl /utf-8 /nologo /W4 main.c ftl.c nand_flash.c /Fe:ftl_sim.exe
.\ftl_sim.exe
```

---

## 🎙️ 面試應答亮點（面試官提問時如何展示此專案）

> **面試官問**：「你對 SSD 的 FTL 了解多少？有實際寫過或模擬過嗎？」
>
> **🎯 你的滿分回答攻略**：
> 1. *「我有用 C 語言實作過一個完整的 Mini FTL 模擬器，包含 NAND 物理層抽象與 FTL 演算法。」*
> 2. *「在 NAND 物理層，我嚴格模擬了 **Erase-before-Write** 限制，如果 FTL 試圖在非 FREE 的 Page 上 Program 會直接觸發硬體 Assert 報警；同時抹除必須以整塊 Block 為單位。」*
> 3. *「在 FTL 核心，我實作了 **Page-level L2P 位址對映**。當 Host 覆寫同一個 LBA 時，採用 **Out-of-Place Update（異地更新）**，將舊的實體 Page 標記為 INVALID，並將新資料寫入當前 Active Block 的 Free Page。」*
> 4. *「當可用 Free Block 降至門檻時，自動觸發 **Greedy 垃圾回收（GC）**：掃描所有 Block 挑選 Invalid Page 最多的 Victim Block，將其剩餘的少數 Valid Page 搬遷至新位置並更新 L2P 表，最後整塊抹除。」*
> 5. *「專案中實時統計了 Host 寫入與 NAND 物理寫入，精準計算出 **寫入放大率（WAF）**，並支援了 **TRIM** 命令來提早標記無效頁，驗證了降低 WAF 的核心機制！」*
