#include <mpi.h>
#include <bits/stdc++.h>
#include <math.h>

bool isBaseOf2(int n)
{
    while (n % 2 == 0)
    {
        n /= 2;
    }
    if (n == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int baseOf2(int n)
{
    int base = 0;
    for (; n != 1; base++)
        n /= 2;
    return base;
}

int verifyBaseOfEntries(int n, int npes)
{
    if (isBaseOf2(n) && isBaseOf2(npes))
    {
        printf("The number of processors and the size of the array are base of 2.\n");
        return 0;
    }
    else if (isBaseOf2(n) && !isBaseOf2(npes))
    {
        printf("The size of the array is a base of 2 but the number of processors aren't.\nTry again.");
        return 1;
    }
    else if (!isBaseOf2(n) && isBaseOf2(npes))
    {
        printf("The number of processors is a base of 2 but the size of the array aren't.\nTry again.");
        return 1;
    }
    else
    {
        printf("The number of processors and the size of the array aren't base of 2.\nTry again.");
        return 1;
    }
}

void swapValue(int *a, int *b)
{
  int *temp = a;
  a = b;
  b = temp;
  return;
}

void bitonicLocal(int *begin, n){
    if(n != 2){
        bitonicLocal()
    } 
    else
    {
        if()
    }
}

int main(int argc, char *argv[])
{
    int n, *arr, myrank, npes, base;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)
    {
        if (verifyBaseOfEntries(atoi(argv[1]), npes))
        {
            MPI_Finalize();
            return 0;
        }
    }
    n = atoi(argv[1]) / npes;
    base = baseOf2(atoi(argv[1]));
    // printf("Base of the array: %d\n", base);

    srandom(myrank);
    arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = random() % (10 * n + 1);



    for(int i = 0; i < base; i++)
    {
        for(int j = 0; i <= i; j++){

        }
    }
    MPI_Finalize();
    return 0;
}