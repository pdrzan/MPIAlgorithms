#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>
#include <bits/stdc++.h>
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

int IncOrder(const void *e1, const void *e2)
{
  return (*((int *)e1) - *((int *)e2));
}

int *SampleSort(int n, int *elmnts, int *nsorted, MPI_Comm comm)
{

  int i, j, nlocal, npes, myrank;
  int *sorted_elmnts, *splitters, *allpicks;
  int *scounts, *sdispls, *rcounts, *rdispls;

  MPI_Comm_size(comm, &npes);
  MPI_Comm_rank(comm, &myrank);
  nlocal = n / npes;

  splitters = (int *)malloc(npes * sizeof(int));
  allpicks = (int *)malloc(npes * (npes - 1) * sizeof(int));

  // qsort(elmnts, nlocal, sizeof(int), IncOrder);
  Introsort(elmnts, elmnts, elmnts + nlocal - 1);

  for (i = 1; i < npes; i++)
    splitters[i - 1] = elmnts[i * nlocal / npes];

  MPI_Allgather(splitters, npes - 1, MPI_INT, allpicks, npes - 1,
                MPI_INT, comm);

  // qsort(allpicks, npes * (npes - 1), sizeof(int), IncOrder);
  Introsort(allpicks, allpicks, allpicks + (npes * (npes - 1)) - 1);

  for (i = 1; i < npes; i++)
    splitters[i - 1] = allpicks[i * npes];
  splitters[npes - 1] = INT_MAX;

  scounts = (int *)malloc(npes * sizeof(int));
  for (i = 0; i < npes; i++)
    scounts[i] = 0;

  //fazer a busca binária aqui
  //TO DO
  for (j = i = 0; i < nlocal; i++)
  {
    if (elmnts[i] < splitters[j])
      scounts[j]++;
    else
      scounts[++j]++;
  }

  sdispls = (int *)malloc(npes * sizeof(int));
  sdispls[0] = 0;

  for (i = 1; i < npes; i++)
    sdispls[i] = sdispls[i - 1] + scounts[i - 1];

  rcounts = (int *)malloc(npes * sizeof(int));
  MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, comm);

  rdispls = (int *)malloc(npes * sizeof(int));
  rdispls[0] = 0;
  for (i = 1; i < npes; i++)
    rdispls[i] = rdispls[i - 1] + rcounts[i - 1];
  *nsorted = rdispls[npes - 1] + rcounts[i - 1];
  sorted_elmnts = (int *)malloc((*nsorted) * sizeof(int));

  MPI_Alltoallv(elmnts, scounts, sdispls, MPI_INT, sorted_elmnts, rcounts, rdispls, MPI_INT, comm);

  // qsort(sorted_elmnts, *nsorted, sizeof(int), IncOrder);
  Introsort(sorted_elmnts, sorted_elmnts, sorted_elmnts + (*nsorted - 1));
  free(splitters);
  free(allpicks);
  free(scounts);
  free(sdispls);

  free(rcounts);
  free(rdispls);
  return sorted_elmnts;
}

void createRandomArray(int *arr, int n)
{
  for (int i = 0; i < n; i++)
    arr[i] = random() % (10 * n + 1);
}

int main(int argc, char *argv[])
{
  int n, npes, myrank, nlocal, *arr, *vsorted, nsorted, i;
  MPI_Status status;
  double stime, etime;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  n = atoi(argv[1]);
  nlocal = n / npes;
  arr = (int *)malloc(nlocal * sizeof(int));

  srandom(myrank);
  createRandomArray(arr, nlocal);

  MPI_Barrier(MPI_COMM_WORLD);

  stime = MPI_Wtime();
  vsorted = SampleSort(n, arr, &nsorted, MPI_COMM_WORLD);
  etime = MPI_Wtime();

  MPI_Barrier(MPI_COMM_WORLD);

  printf("time = %f\n", etime - stime);

  free(arr);
  free(vsorted);

  MPI_Finalize();

  return 0;
}