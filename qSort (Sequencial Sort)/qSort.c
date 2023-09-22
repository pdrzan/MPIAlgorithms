#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void createRandomArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = random() % (10 * n + 1);
}

int IncOrder(const void *e1, const void *e2)
{
    return (*((int *)e1) - *((int *)e2));
}

int main(int argc, char *argv[])
{
    clock_t t;
    double time_taken;
    int n, *arr;
    n = atoi(argv[1]);
    arr = (int *)malloc(n * sizeof(int));

    createRandomArray(arr, n);

    t = clock();
    qsort(arr, n, sizeof(int), IncOrder);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Time: %f\n", time_taken);

    return 0;
}