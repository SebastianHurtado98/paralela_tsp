#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <limits.h>
#include <fstream>
#include <openmpi/mpi.h>
#include <omp.h>

using namespace std;

int get_min_and_substract(int n, int mat[], int prev_rc)
{
    int min;

    int *rc = new int[n];
    int i, j;

    #pragma omp parallel for private(j, min) shared(i, mat, rc)
    for (i = 0; i < n; i++)
    {
        min = INT_MAX;
        for (j = 0; j < n; j++)
        {
            if (min > mat[(i * n) + j])
            {
                min = mat[(i * n) + j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (j = 0; j < n; j++)
            {
                if (mat[(i * n) + j] != INT_MAX)
                {
                    mat[(i * n) + j] -= min;
                }
            }
            rc[i] = min;
        } else {
            rc[i] = 0;
        }
    }

    #pragma omp parallel for private(i, min) shared(j, mat, rc)
    for (j = 0; j < n; j++)
    {
        min = INT_MAX;
        for (i = 0; i < n; i++)
        {
            if (min > mat[(i * n) + j])
            {
                min = mat[(i * n) + j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (i = 0; i < n; i++)
            {
                if (mat[(i * n) + j] != INT_MAX)
                {
                    mat[(i * n) + j] -= min;
                }
            }
            rc[j] += min;
        }   
    }

    int total_rc = 0;
    for (int i = 0; i < n; i++)
    {
        total_rc += rc[i];
    }

    return prev_rc + total_rc;
}

int main(int argc, char **argv)
{

    int n;
    int *matrix, *original_matrix;

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
        original_matrix = (int *)malloc(n * n * sizeof(int));

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

        int reduced_counter = get_min_and_substract(n, matrix, 0);
        printf("rc: %i\n", reduced_counter);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                original_matrix[(i * n) + j] = matrix[(i * n) + j];
            }
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        matrix = (int *)malloc(n * n * sizeof(int));
        original_matrix = (int *)malloc(n * n * sizeof(int));
    }

    MPI_Bcast(matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(original_matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    
    if (rank != 0)
    {   
        int h_r = 0, h_c = 1;
        for (int i = 0; i < n; i++) {
            matrix[(h_r * n) + i] = INT_MAX;
            matrix[(i * n) + h_c] = INT_MAX;
        }

        matrix[(h_c*n) + h_r] = INT_MAX;
        
        /*
        cout << endl << "PRINTING MATRIX " << endl;
        int temp;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                temp = matrix[(i * n) + j];
                if (temp != INT_MAX)
                {
                    cout << matrix[(i * n) + j] << " ";
                }
                else
                {
                    cout << -1 << " ";
                }
            }
            cout << endl;
        }
        */

        int reduced_counter = get_min_and_substract(n, matrix, 25) + original_matrix[(h_r*n) + h_c];
        printf("rc: %i\n", reduced_counter);
    }

    double t2 = MPI_Wtime();

    if (rank == 0)
    {
        cout << "MPI Time: " << t2 - t1 << endl;
    }

    MPI_Finalize();
}