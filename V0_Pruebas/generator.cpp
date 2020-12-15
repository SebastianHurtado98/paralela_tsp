#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);

    srand(time(NULL));

    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int temp = rand() % 100;
            if (temp % 7 == 0) {
                matrix[i][j] = temp;
            } else {
                matrix[i][j] = -1;
            }
            
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                matrix[i][j] = -1;
            }
        }
    }

    ofstream file;
    file.open("matrix.txt");
    file << n << "\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            file << matrix[i][j] << " ";
        }
        file << matrix[i][n - 1];
        file << "\n";
    }
    file.close();

}