#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>

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
    int matrix[100];
    int neighbor;
    int to_visit[10];

    queue<int> q;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        cout << "Size: " << size << endl;
        cin >> n;

        for (int i = 0; i < n * n; i++)
        {
            int temp;
            cin >> temp;
            matrix[i] = temp;
        }
    }

    if (rank == 0)
    {
        int src = 0;
        q.push(src);

        int neighbors[n];
        for (int i = 0; i < n; i++)
        {
            if (matrix[(src * n) + i] != -1)
            {
                neighbors[i] = (src * n) + i;
            }
            else
            {
                neighbors[i] = -1;
            }
        }

        int count = 0;

        int i = 0;
        while (i < n)
        {
            if (neighbors[i] != -1)
            {
                to_visit[count] = neighbors[i];
                count++;
            }
            i++;
            if (count == (size - 1) || i == n)
            {
                if (count < size - 1)
                {
                    for (int j = count; j < (size - 1); j++)
                    {
                        to_visit[j] = -1;
                    }
                }

                cout << "Antes de enviar\n";
                for (int j = 0; j < size - 1; j++)
                {
                    cout << to_visit[j] << " ";
                }
                cout << endl;

                for (int j = 1; j < size; j++)
                {
                    MPI_Send(to_visit, size - 1, MPI_INT, j, 0, MPI_COMM_WORLD);
                }

                for (int j = 0; j < size - 1; j++)
                {
                    to_visit[j] = 0;
                }
                count = 0;
            }
        }

        
    }

    if (rank > 0)
    {
        MPI_Status status;
        MPI_Recv(to_visit, size - 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        cout << "Proceso " << rank << ": " << to_visit[rank - 1] << endl;
    }

    MPI_Finalize();
    return 0;
}