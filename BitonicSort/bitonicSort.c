#include <mpi.h>
#include <bits/stdc++.h>
#include <math.h>


int main(int argc, char *argv[]){
    int n, arr, myrank, npes;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    n = atoi(argv[1]);
    if(log(npes) && log(n))

    srandom(myrank);
    arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = random() % (10 * n + 1);

    return 0;
}