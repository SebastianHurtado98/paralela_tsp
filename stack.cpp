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

    int matrix[100];
    int row[100];
    int queue[10];
    int visited[100];
    int bfs[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        cin >> n;

        for (int i = 0; i < n * n; i++)
        {
            cin >> matrix[i];
        }

        cin >> root;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Scatter(matrix, n, MPI_INT, row, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
    {
        queue[i] = -1;
    }

    int index = 0;
    if (rank >= root)
    {
        for (int i = 0; i < n; i++)
        {
            if (row[i] != -1)
            {
                queue[index++] = i;
            }
        }
    }

    cout << "Process " << rank << ": ";
    for (int i = 0; i < index; i++)
    {
        cout << queue[i] << " ";
    }
    cout << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(queue, n, MPI_INT, bfs, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
    {
        visited[i] = 0;
    }

    if (rank == 0)
    {
        cout << "\nBFS Traversal: " << endl;
        cout << root;
        for (int i = 0; i < n * n; i++)
        {
            if (areAllVisited(visited, n))
            {
                break;
            }

            if (bfs[i] != -1)
            {
                if (visited[bfs[i]] == 0)
                {
                    cout << " -> " << bfs[i];
                    visited[bfs[i]] = 1;
                }
            }
            else
            {
                continue;
            }
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}