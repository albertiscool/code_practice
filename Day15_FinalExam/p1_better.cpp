uint32_t reverse_bits(uint32_t num) {
    uint32_t result = 0; // 準備一個全新的空箱子

    // 32 個 Bit，所以跑 32 次
    for (int i = 0; i < 32; i++) {
        // 1. 把空箱子往左推一格，騰出最右邊的空間
        result = result << 1; 

        // 2. 把 num 的最右邊那 1 個 Bit 抓出來，塞進 result 騰出的空間
        result = result | (num & 1); 

        // 3. 把 num 往右推一格，準備下個回合抓下一個 Bit
        num = num >> 1; 
    }
    
    return result; // 記得交差！
}