#include <mpi.h>
#include <bits/stdc++.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

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

void merge(int firstArray[], int secondArray[], int aws[], int n)
{
    int k = 0, j = 0, i = 0;
    for (; k < n; k++)
    {
        if (firstArray[i] <= secondArray[j])
        {
            aws[k] = firstArray[i];
            i++;
        }
        else
        {
            aws[k] = secondArray[j];
            j++;
        }
    }
}

void merge_inverse(int firstArray[], int secondArray[], int aws[], int n)
{
    int k = n - 1, j = n - 1, i = n - 1;
    for (; k > -1; k--)
    {
        if (firstArray[i] >= secondArray[j])
        {
            aws[k] = firstArray[i];
            i--;
        }
        else
        {
            aws[k] = secondArray[j];
            j--;
        }
    }
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
        printf("The size of the array is a base of 2 but the number of processors aren't.\nTry again.\n");
        return 1;
    }
    else if (!isBaseOf2(n) && isBaseOf2(npes))
    {
        printf("The number of processors is a base of 2 but the size of the array aren't.\nTry again.\n");
        return 1;
    }
    else
    {
        printf("The number of processors and the size of the array aren't base of 2.\nTry again.\n");
        return 1;
    }
}

int IncOrder(const void *e1, const void *e2)
{
    return (*((int *)e1) - *((int *)e2));
}

void compare_split(int arr[], int n, int rankDestiny, int op)
{
    int *res, *recv;
    res = (int *)malloc(n * sizeof(int));
    recv = (int *)malloc(n * sizeof(int));
    MPI_Sendrecv(arr, n, MPI_INT, rankDestiny, 1, recv, n, MPI_INT, rankDestiny, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (op == 0) // min
    {
        merge(arr, recv, res, n);
    }
    else // max
    {
        merge_inverse(arr, recv, res, n);
    }
    for (int i = 0; i < n; i++)
    {
        arr[i] = res[i];
    }
    free(res);
    free(recv);
}

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
}

void createRandomArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = random() % (10 * n + 1);
}

int main(int argc, char *argv[])
{
    int n, *arr, myrank, npes, base;
    float stime, etime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)
    {
        if (verifyBaseOfEntries(atoi(argv[1]), npes))
        {
            MPI_Abort(MPI_COMM_WORLD, 0);
            return 0;
        }
    }
    n = atoi(argv[1]) / npes;
    base = baseOf2(npes);

    srandom(myrank + 1);
    arr = (int *)malloc(n * sizeof(int));
    createRandomArray(arr, n);

    if (n != 1)
    {
        qsort(arr, n, sizeof(int), IncOrder);
    }

    stime = MPI_Wtime();
    for (int i = 0; i < base; i++)
    {
        for (int j = i; j >= 0; j--)
        {
            int aux = pow(2, j);
            int aux2 = pow(2, i + 2);

            if (myrank % (aux * 2) < aux)
            {
                if (myrank % aux2 < pow(2, i + 1))
                {
                    compare_split(arr, n, myrank + aux, 0);
                }
                else
                {
                    compare_split(arr, n, myrank + aux, 1);
                }
            }
            else
            {
                if (myrank % aux2 < pow(2, i + 1))
                {
                    compare_split(arr, n, myrank - aux, 1);
                }
                else
                {
                    compare_split(arr, n, myrank - aux, 0);
                }
            }
        }
    }
    etime = MPI_Wtime();
    free(arr);
    printf("time = %f\n", etime - stime);
    MPI_Finalize();
    return 0;
}