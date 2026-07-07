void reverse_string(char *str)
{
    int length = 0;
    int i = 0;
    char temp;
    while(str[length] != '\0')
    {
        length++;
    }
    while(i<length/2)
    {
        int upper_index = i;
        int lower_index = length - i - 1;
        temp = *(str+upper_index);
        *(str+upper_index) = *(str+lower_index);
        *(str+lower_index) = temp;
        i++;
    }
}