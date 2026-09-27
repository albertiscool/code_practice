#include <stdio.h>
void print_multiplication_table(void)
{
    for(int i = 1; i < 10; i++)
    {
        for(int j = 1; j < 10; j++)
        {
            printf("%d*%d=%-2d ",i,j,i*j);
        }
        printf("\n");
    }
}

int main(void)
{
    print_multiplication_table();
    return 0;
}