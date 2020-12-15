#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <openmpi/mpi.h>

using namespace std;

#define MAX_ARR_SIZE 10

int main(int argc, char **argv)
{
    int n;
    int matrix[MAX_ARR_SIZE * MAX_ARR_SIZE];
    int stack[MAX_ARR_SIZE];
    int curr, node;

    int neighbors[MAX_ARR_SIZE];
    int neighbor;

    int rank, size;

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int temp;
            cin >> temp;
            if (temp == -1)
            {
                matrix[(i * n) + j] = INT_MAX;
            }
            else
            {
                matrix[(i * n) + j] = temp;
            }
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        node = 0;
        stack[curr] = node;
        curr++;

        for (int i = 0, j = 0; i < n; i++)
        {
            if (matrix[(node * n) + i] < INT_MAX)
            {
                neighbors[j] = i;
                j++;
            }
        }
    }

    MPI_Scatter(neighbors, MAX_ARR_SIZE, MPI_INT, &neighbor, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Soy rank %i y tengo nodo %i\n", rank, neighbor);

    MPI_Finalize();

}