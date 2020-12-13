#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace std;

#define lli long long int

class Node
{
public:
    vector<pair<lli, lli>> path;
    vector<vector<lli>> matrix;
    lli cost;
    lli v;
    lli level;

public:
    Node(lli n, vector<vector<lli>> matrix, vector<pair<lli, lli>> const &path, lli level, lli i, lli j)
    {
        this->path = path;

        if (level != 0)
        {
            this->path.push_back({i, j});
        }

        this->matrix = matrix;

        for (lli k = 0; level != 0 && k < n; k++)
        {
            this->matrix[i][k] = INT_MAX;
            this->matrix[k][j] = INT_MAX;
        }

        this->matrix[j][0] = INT_MAX;
        this->level = level;
        this->v = j;
    }
};

lli get_min_and_substract(lli n, vector<vector<lli>> &mat, lli prev_cost)
{
    lli min;

    vector<lli> rc(n);
    lli i, j;

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

    lli cost = 0;
    for (lli i = 0; i < n; i++)
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

void display(vector<pair<lli, lli>> path)
{
    for (int i = 0; i < path.size(); i++)
    {
        std::cout << path[i].first + 1 << " -> " << path[i].second + 1 << endl;
    }
}

int tsp(vector<vector<lli>> matrix, lli n)
{
    priority_queue<Node *, vector<Node *>, cmp> pq;
    vector<pair<lli, lli>> p;

    Node *root = new Node(n, matrix, p, 0, -1, 0);

    root->cost = get_min_and_substract(n, root->matrix, 0);

    pq.push(root);

    while (!pq.empty())
    {
        Node *min = pq.top();
        pq.pop();

        lli i = min->v;

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

int main(int argc, char **argv)
{
    lli n;
    cin >> n;

    vector<vector<lli>> matrix(n, vector<lli>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int temp;
            cin >> temp;
            if (temp == -1) {
                matrix[i][j] = INT_MAX;
            } else {
                matrix[i][j] = temp;
            }
        }
    }

    auto begin = chrono::high_resolution_clock::now();
    lli res = tsp(matrix, n);
    auto end = chrono::high_resolution_clock::now();

    std::cout << chrono::duration<float>(end - begin).count() << endl;
}