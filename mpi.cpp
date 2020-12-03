#include <iostream>
#include <vector>
#include <limits.h>
#include <fstream>
#include <openmpi/mpi.h>
#include <omp.h>

using namespace std;

int get_min_and_substract(int n, int mat[], int &rc)
{
    int i, min;

    #pragma omp parallel for private(i, min) shared(mat, rc)
    for (int i = 0; i < n; i++)
    {
        int min = INT_MAX;
        for (int j = 0; j < n; j++)
        {
            if (min > mat[(i * n) + j])
            {
                min = mat[(i * n) + j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (int j = 0; j < n; j++)
            {
                if (mat[(i * n) + j] != INT_MAX)
                {
                    mat[(i * n) + j] -= min;
                }
            }
        }
        rc += min;
    }

    #pragma omp parallel for private(i, min) shared(mat, rc)
    for (int j = 0; j < n; j++)
    {
        int min = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (min > mat[(i * n) + j])
            {
                min = mat[(i * n) + j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (int i = 0; i < n; i++)
            {
                if (mat[(i * n) + j] != INT_MAX)
                {
                    mat[(i * n) + j] -= min;
                }
            }
        }
        rc += min;
    }

    return rc;
}

int main(int argc, char **argv)
{

    int n;
    int *matrix;

    int rank;
    int size;

    omp_set_num_threads(4);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t1 = MPI_Wtime();

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

        int reduced_counter = get_min_and_substract(n, matrix, reduced_counter);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        matrix = (int *)malloc(n * n * sizeof(int));
    }

    MPI_Bcast(matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        int reduced_counter = get_min_and_substract(n, matrix, reduced_counter);
    }

    double t2 = MPI_Wtime();

    if (rank == 0)
    {
        cout << "MPI Time: " << t2 - t1 << endl;
    }

    MPI_Finalize();
}