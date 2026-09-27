#include <stdio.h>

void print_christmas_tree(int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n - 1 - i; j++)
        {
            printf(" ");
        }
        for(int k = 0; k < 2 * i + 1; k++)
        {
            printf("*");
        }
        printf("\n");
    }
    for(int q = 0; q < n - 1; q++)
    {
        printf(" ");
    }
    printf("|\n");
}

int main(void)
{
    print_christmas_tree(4);
    return 0;
}