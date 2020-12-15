#include <iostream>
#include <openmpi/mpi.h>
#include <omp.h>
using namespace std;

int get_min_and_substract(int n, int mat[])
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

    return total_rc;
}


int main(int argc, char **argv)
{
    int world_rank;
    int world_size;

    int n = 8, vecinos[n] = {-1, 2, -1, 3, 4, 2, -1, 2}, n_procesos = 4, cnt_v = 0;
    
    int *vecinos_por_visitar = new int[n_procesos - 1];
    
    int i = 0;
    
    int origin = 0;

    omp_set_num_threads(4);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int number;

    while (i < n)
    {
        if (vecinos[i] != -1)
        {
            vecinos_por_visitar[cnt_v] = i;
            cnt_v++;
        }
        i++;
        if (cnt_v == n_procesos - 1 || i == n)
        {
            if (cnt_v < n_procesos - 1)
            {
                for (int k = cnt_v; k < n_procesos - 1; k++)
                    vecinos_por_visitar[k] = -1;
            }
            

            if (world_rank == 0) {
                for (int k = 0; k < n_procesos - 1; k++) MPI_Send(&origin, 1, MPI_INT, k+1, 0, MPI_COMM_WORLD);
                for (int k = 0; k < n_procesos - 1; k++) MPI_Send(&vecinos_por_visitar[k], 1, MPI_INT, k+1, 1, MPI_COMM_WORLD);
                for (int k = 0; k < n_procesos - 1; k++) MPI_Send(&vecinos, n, MPI_INT, k+1, 2, MPI_COMM_WORLD);


            } else {
                MPI_Recv(&origin, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&number, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //Reemplazar por matriz
                //Obtener 
                MPI_Recv(&vecinos, n, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (number != -1) printf("Hide row %d and column %d according to process 0\n", origin, number);
                else printf("Dont visit it. \n");
            
            }

            cout << endl;
            
            for (int k = 0; k < n_procesos - 1; k++)
                vecinos_por_visitar[k] = 0;
            cnt_v = 0;
        }
    }
    MPI_Finalize();
}