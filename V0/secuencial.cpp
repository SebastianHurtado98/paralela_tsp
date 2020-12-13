#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <chrono>

using namespace std;

class Node
{
public:
    vector<pair<int, int>> path;
    vector<vector<int>> matrix;
    int cost;
    int v;
    int level;

public:
    Node(int n, vector<vector<int>> matrix, vector<pair<int, int>> const &path, int level, int i, int j)
    {
        this->path = path;

        if (level != 0)
        {
            this->path.push_back({i, j});
        }

        this->matrix = matrix;

        for (int k = 0; level != 0 && k < n; k++)
        {
            this->matrix[i][k] = INT_MAX;
            this->matrix[k][j] = INT_MAX;
        }

        this->matrix[j][0] = INT_MAX;
        this->level = level;
        this->v = j;
    }
};

int get_min_and_substract(int n, vector<vector<int>> &mat, int prev_cost)
{
    int min;

    vector<int> rc(n);
    int i, j;

    for (i = 0; i < n; i++)
    {
        min = INT_MAX;
        for (j = 0; j < n; j++)
        {
            if (min > mat[i][j])
            {
                min = mat[i][j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (j = 0; j < n; j++)
            {
                if (mat[i][j] != INT_MAX)
                {
                    mat[i][j] -= min;
                }
            }
            rc[i] = min;
        }
        else
        {
            rc[i] = 0;
        }
    }

    for (j = 0; j < n; j++)
    {
        min = INT_MAX;
        for (i = 0; i < n; i++)
        {
            if (min > mat[i][j])
            {
                min = mat[i][j];
            }
        }

        if (min != 0 && min != INT_MAX)
        {
            for (i = 0; i < n; i++)
            {
                if (mat[i][j] != INT_MAX)
                {
                    mat[i][j] -= min;
                }
            }
            rc[j] += min;
        }
    }

    int cost = 0;
    for (int i = 0; i < n; i++)
    {
        cost += rc[i];
    }

    return cost + prev_cost;
}

struct cmp
{
    bool operator()(const Node *a, const Node *b) const
    {
        return a->cost > b->cost;
    }
};

void display(vector<pair<int, int>> path)
{
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i].first + 1 << " -> " << path[i].second + 1 << endl;
    }
}

int tsp(vector<vector<int>> matrix, int n)
{
    priority_queue<Node *, vector<Node *>, cmp> pq;
    vector<pair<int, int>> p;

    Node *root = new Node(n, matrix, p, 0, -1, 0);

    root->cost = get_min_and_substract(n, root->matrix, 0);

    pq.push(root);

    while (!pq.empty())
    {
        Node *min = pq.top();
        pq.pop();

        int i = min->v;

        if (min->level == n - 1)
        {
            min->path.push_back({i, 0});
            //display(min->path);
            return min->cost;
        }

        for (int j = 0; j < n; j++)
        {
            if (min->matrix[i][j] != INT_MAX)
            {
                Node *child = new Node(n, min->matrix, min->path, min->level + 1, i, j);
                child->cost = min->matrix[i][j] + get_min_and_substract(n, child->matrix, min->cost);
                pq.push(child);
            }
        }
    }
}

int main()
{
    int n = 5;

    srand(time(NULL));

    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = rand() % 1000;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                matrix[i][j] = INT_MAX;
            }
        }
    }

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int res = tsp(matrix, n);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds> (end - begin).count() << "[µs]" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "[ns]" << endl;
}