#include <mpi.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;

void swapValue(int *a, int *b)
{
    int *temp = a;
    a = b;
    b = temp;
    return;
}

void InsertionSort(int arr[], int *begin, int *end)
{
    int left = begin - arr;
    int right = end - arr;

    for (int i = left + 1; i <= right; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    return;
}

int *Partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (arr + i + 1);
}

int *MedianOfThree(int *a, int *b, int *c)
{
    if (*a < *b && *b < *c)
        return (b);

    if (*a < *c && *c <= *b)
        return (c);

    if (*b <= *a && *a < *c)
        return (a);

    if (*b < *c && *c <= *a)
        return (c);

    if (*c <= *a && *a < *b)
        return (a);

    if (*c <= *b && *b <= *a)
        return (b);

    return (a);
}

void IntrosortUtil(int arr[], int *begin, int *end,
                   int depthLimit)
{
    int size = end - begin;
    if (size < 16)
    {
        InsertionSort(arr, begin, end);
        return;
    }
    if (depthLimit == 0)
    {
        make_heap(begin, end + 1);
        sort_heap(begin, end + 1);
        return;
    }
    int *pivot = MedianOfThree(begin, begin + size / 2, end);
    swapValue(pivot, end);
    int *partitionPoint = Partition(arr, begin - arr, end - arr);
    IntrosortUtil(arr, begin, partitionPoint - 1,
                  depthLimit - 1);
    IntrosortUtil(arr, partitionPoint + 1, end,
                  depthLimit - 1);

    return;
}

void Introsort(int arr[], int *begin, int *end)
{
    int depthLimit = 2 * log(end - begin);
    IntrosortUtil(arr, begin, end, depthLimit);

    return;
}

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
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

int main(int argc, char *argv[])
{
    int n, *arr;
    int npes, myrank;
    float stime, etime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    n = atoi(argv[1]) / npes;
    srandom(myrank);
    arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = random() % (10 * n + 1);
    // sleep(myrank);
    // printf("Array of %d° processor before odd-even: ", myrank);
    // printArray(arr, n);
    // printf("\n");
    Introsort(arr, arr, arr + n - 1);
    stime = MPI_Wtime();
    for (int i = 1; i <= npes; i++)
    {
        if (i % 2 == 1)
        {
            if (myrank % 2 == 1)
            {
                if (myrank != npes - 1)
                    compare_split(arr, n, myrank + 1, 0); // min
            }
            else
            {
                if (myrank != 0)
                    compare_split(arr, n, myrank - 1, 1); // max
            }
        }
        if (i % 2 == 0)
        {
            if (myrank % 2 == 0)
            {
                if (myrank != npes - 1)
                    compare_split(arr, n, myrank + 1, 0); // min
            }
            else
            {
                if (myrank != 0)
                    compare_split(arr, n, myrank - 1, 1); // max
            }
        }
    }
    // if (myrank == 0)
    // {
    //     printf("Sorted array: ");
    // }
    // sleep(myrank);
    // printArray(arr, 10);
    etime = MPI_Wtime();
    // if (myrank == n - 1)
    // {
    //     printf("\n");
    // }
    free(arr);
    printf("time = %f\n", etime - stime);
    MPI_Finalize();

    return (0);
}