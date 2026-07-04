uint32_t modify_bits(uint32_t reg, int start_bit, int end_bit, uint32_t value) {
    // 1. 算出你要替換的區間「總共有幾個 Bit」
    int length = end_bit - start_bit + 1; 

    // 2. 製作一塊「底部全部是 1」的基礎遮罩
    // 例如 length 是 8，(1 << 8) - 1 會變成 256 - 1 = 255 (二進位的 0000...11111111)
    uint32_t base_mask = (1U << length) - 1; 

    // 3. 把這塊遮罩推到正確的位址上
    uint32_t target_mask = base_mask << start_bit;

    // 4. 標準動作：先用反相遮罩 (~) 清零，再把 value 推過去塞進去！
    reg &= ~target_mask;
    reg |= (value << start_bit);

    return reg;
}