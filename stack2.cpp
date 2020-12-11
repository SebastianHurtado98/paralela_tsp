#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int areAllVisited(int *visited, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (visited[i] == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size;

    int n;
    int root;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
    return 0;
}