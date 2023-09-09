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
    for (; k < 2 * n && j < n && i < n; k++)
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
    for (; i < n; i++)
    {
        aws[k] = firstArray[i];
        k++;
    }
    for (; j < n; j++)
    {
        aws[k] = secondArray[j];
        k++;
    }
}

void compare_split(int arr[], int n, int rankDestiny, int op)
{
    int res[2 * n], recv[n];

    MPI_Sendrecv(arr, n, MPI_INT, rankDestiny, 1, recv, n, MPI_INT, rankDestiny, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    merge(arr, recv, res, n);
    if (op == 0) // min
    {
        for (int i = 0; i < n; i++)
        {
            arr[i] = res[i];
        }
    }
    else // max
    {
        for (int i = 0; i < n; i++)
        {
            arr[i] = res[n + i];
        }
    }
}

int main(int argc, char *argv[])
{
    int n = 4, arr[n], aux[n * n];
    int npes, myrank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    for (int i = 0; i < n * n; i++)
    {
        aux[i] = rand() % 50;
    }
    for (int i = 0; i < n; i++)
    {
        arr[i] = aux[n * myrank + i];
    }
    // sleep(myrank);
    printf("Array of %d° processor before odd-even: ", myrank);
    printArray(arr, n);
    printf("\n");
    Introsort(arr, arr, arr + n - 1);

    for (int i = 1; i <= npes; i++)
    {
        if (i % 2 == 1)
        {
            if (myrank % 2 == 1)
            {
                if (myrank != n - 1)
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
                if (myrank != n - 1)
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
    printArray(arr, n);
    if (myrank == n - 1)
    {
        printf("\n");
    }
    MPI_Finalize();

    return (0);
}