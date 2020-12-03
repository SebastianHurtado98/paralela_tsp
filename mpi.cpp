#include <iostream>
#include <vector>
#include <limits.h>
#include <fstream>
#include <openmpi/mpi.h>
#include <omp.h>

using namespace std;

int main(int argc, char **argv)
{

    int n;
    int *matrix;

    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        ifstream ifs("input.txt");
        ifs >> n;

        matrix = (int *)malloc(n * n * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int temp;
                ifs >> temp;
                if (temp != -1)
                {
                    matrix[(i * n) + j] = temp;
                }
                else
                {
                    matrix[(i * n) + j] = INT_MAX;
                }
            }
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        matrix = (int *)malloc(n * n * sizeof(int));
    }

    MPI_Bcast(matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << matrix[(i * n) + j] << "    ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
}