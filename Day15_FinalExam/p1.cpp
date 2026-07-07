uint32_t reverse_bits(uint32_t num)
{
    int size = 32;
    while(size > 16)
    {
        int upper_index = size - 1;
        int lower_index = 31 - upper_index;
        int num_upper = (num >> upper_index) & 1;
        int num_lower = (num >> lower_index) & 1;
        if(num_lower == 1)
        {
            num |= (1U << upper_index);
        }
        else
        {
            num &= ~(1U << upper_index);
        }

        if(num_upper == 1)
        {
            num |= (1U << lower_index);
        }
        else
        {
            num &= ~(1U << lower_index);
        }

        size--;
    }

    return num;
}