#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <limits.h>
#include <fstream>
#include <openmpi/mpi.h>
#include <omp.h>
#include <queue>

using namespace std;

int get_min_and_substract(int n, int mat[], int h_r, int h_c)
{
    int min;

    int *rc = new int[n];
    int i, j;

    if (h_c != -1 && h_r != -1)
    {
        for (int i = 0; i < n; i++)
        {
            mat[(h_r * n) + i] = INT_MAX;
            mat[(i * n) + h_c] = INT_MAX;
        }

        mat[(h_c * n) + h_r] = INT_MAX;
    }

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
        }
        else
        {
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
    return total_rc;
}

int main(int argc, char **argv)
{
    /*
    Pendientes: 
    Tomar en cuenta distancia + prev_rc para escoger la min distancia.
    Coger la min_matrix y reemplazar la matrix. 
    Tener un while con queue para repetir la operacion.
    */

    int world_rank;
    int world_size;

    int vecinos[5] = {-1, 20, 30, 10, 11}, n_procesos = 4, cnt_v = 0, number, distance, next_origin;
    int *vecinos_por_visitar = new int[5];

    int n;

    int *matrix, *original_matrix;

    int i = 0;

    int origin = 0;

    //Pre processing first matrix

    omp_set_num_threads(4);

    ifstream ifs("input.txt");
    ifs >> n;

    //vecinos = new int[n];

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

    int reduced_counter = get_min_and_substract(n, matrix, -1, -1);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            original_matrix[(i * n) + j] = matrix[(i * n) + j];
        }
    }

    for (int k = 0; k < n; k++)
        vecinos[k] = original_matrix[(origin * n) + k];

    cnt_v = 0;

    //Variables para seleccionar el mejor valor
    int rc_parcial_min = INT_MAX;
    int rc_parcial;
    int *matrix_parcial_rc = new int[n * n];
    int *matrix_min_rc = new int[n * n];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //SACA EL FOR PARA QUE FUNCIONE UNA ITERACION

    for (int i  = 0; i < n; i++) {
        if (vecinos[i] != -1) {
            vecinos_por_visitar[count] = i;
        }
    }

    max_it = ceil(count/world_size - 1);

    if (world_rank == 0) {
        for(int i = 0; i < max_it; i++) {
            MPI_Send(&origin, 1, MPI_INT, (i + 1)%(world_size - 1), 0, MPI_COMM_WORLD);
            MPI_Send(&vecinos_por_visitar[i], 1, MPI_INT, (i + 1)%(world_size - 1), 1, MPI_COMM_WORLD);
            MPI_Send(matrix, n * n, MPI_INT, (i + 1)%(world_size - 1), 2, MPI_COMM_WORLD);
            MPI_Recv(&rc_parcial, 1, MPI_INT, (i + 1)%(world_size - 1), 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(matrix_parcial_rc, n * n, MPI_INT, (i + 1)%(world_size - 1), 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
            MPI_Recv(&origin, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&number, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //Reemplazar por matriz
            //Obtener
            MPI_Recv(matrix, n * n, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (number != -1)
                rc_parcial = get_min_and_substract(n, matrix, origin, number);
            else
                rc_parcial = -1;

            MPI_Send(&rc_parcial, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
            MPI_Send(matrix, n * n, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }

   
    MPI_Finalize();
}