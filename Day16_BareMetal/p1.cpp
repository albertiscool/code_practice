uint32_t modify_bits(uint32_t reg, int start_bit, int end_bit, uint32_t value)
{
    for(int i = start_bit;i <= end_bit;i++)
    {
        reg &= ~(1<<i);
    }
    reg |= (value << start_bit);
    return reg;
}