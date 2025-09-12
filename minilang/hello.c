#include <stdio.h>

int main()
{
    int arr[10];
    int i = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        arr[i] = i;
    }

    int j = 0;
    for (j = 0; j < 10; j = j + 1)
    {
        printf("%d",arr[j]);
    }
    return 0;
}