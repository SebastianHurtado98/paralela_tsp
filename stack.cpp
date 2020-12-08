#include <iostream>
#include <limits.h>
#include <openmpi/mpi.h>

using namespace std;

int main()
{
    int n;
    int root;
    int *matrix, *row;
    int *stack;
    int *queue;
    int *visited;

    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        int n;
        cin >> n;

        stack = (int *)malloc(n * sizeof(int));
        matrix = (int *)malloc(n * n * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int temp;
                cin >> temp;
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

        cin >> root;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    row = (int *)malloc(n * sizeof(int));
    queue = (int *)malloc(n * sizeof(int));
    visited = (int *)malloc(n * sizeof(int));

    MPI_Scatter(matrix, n, MPI_INT, row, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
    {
        queue[i] = -1;
    }

    int idx = 0;
    if (rank >= root)
    {
        for (int i = 0; i < n; i++)
        {
            if (row[i] >= 1)
            {
                queue[idx++] = i;
            }
        }
    }

    cout << "Process " << rank << ": ";
    for (int i = 0; i < idx; i++)
    {
        cout << queue[i] << " ";
    }
    cout << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(queue, n, MPI + INT, bfs_traversal, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
    {
        visited[i] = 0;
    }

    if (rank == 0)
    {
        cout << "BFS:\n";
        cout << root;
        for (int i = 0; i < n * n; i++)
        {
            if (allVisited(visited, n))
            {
                break;
            }

            if (bfs_traversal[i] != -1)
            {
                if (visited[bfs_traversal[i]] == 0)
                {
                    cout << " -> " << bfs_traversal[i];
                    visited[bfs_traversal[i]] = 1;
                }
            }
            else
            {
                continue;
            }
        }
    }

    MPI_Finalize();
}