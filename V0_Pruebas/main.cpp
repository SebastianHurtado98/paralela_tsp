#include <iostream>
#include <limits>
#include <omp.h>
#include <iomanip>

using namespace std;

//Esto está así por si necesitamos modificar las dimensiones en caso el
//speed up no se note tan bien.
typedef int DIM1;
typedef int DIM2;
DIM2 MAX_DIM2 = numeric_limits<DIM2>::max();

int main()
{
    //Listas enlazadas. El grafo va a tener DEMASIADOS infinitos si no.

    DIM1 n;
    DIM2 temp;
    cin >> n;
    DIM2 **matriz = new DIM2 *[n];

    double t1 = omp_get_wtime();

    DIM1 i;

    for (i = 0; i < n; i++)
    {
        matriz[i] = new DIM2[n];
        for (DIM1 j = 0; j < n; j++)
        {
            cin >> temp;
            if (temp != -1)
                matriz[i][j] = temp;
            else
                matriz[i][j] = MAX_DIM2;
        }
    }

    DIM2 min;
    DIM2 reduced_counter = 0;

    // Get min value in each row and substract

    for (i = 0; i < n; i++)
    {
        min = MAX_DIM2;
        //GET MIN VALUE
        for (DIM1 j = 0; j < n; j++)
        {
            if (min > matriz[i][j])
            {
                min = matriz[i][j];
            }
        }
        //SUBSTRACT MIN VALUE
        if (min != 0 && min != MAX_DIM2)
        {
            
            for (DIM1 j = 0; j < n; j++)
            {
                if (matriz[i][j] != MAX_DIM2)
                {
                    matriz[i][j] -= min;
                }
            }
            reduced_counter += min;
        }
    }

    // Get min value in each column and substract
    for (DIM1 j = 0; j < n; j++)
    {
        min = MAX_DIM2;
        //GET MIN VALUE
        for (DIM1 i = 0; i < n; i++)
        {
            if (min > matriz[i][j])
            {
                min = matriz[i][j];
            }
        }
        //SUBSTRACT MIN VALUE
        if (min != 0 && min != MAX_DIM2)
        {
            for (DIM1 i = 0; i < n; i++)
            {
                if (matriz[i][j] != MAX_DIM2)
                {
                    matriz[i][j] -= min;
                }
            }
            reduced_counter += min;
        }
    }



    // From the reduced matrix, check path a -> b by hiding row a and column b
    // This is performed by updating a and b to inf.
    // In this code, just ignore them. You also need to ignore the backwards path: b->a

    DIM2 new_reduced_counter = 0;
    DIM1 h_r = 0, h_c = 1;

    // Get min value in each row and substract

    for (DIM1 i = 0; i < n; i++)
    {
        min = MAX_DIM2;
        //GET MIN VALUE
        for (DIM1 j = 0; j < n; j++)
        {
            if (i != h_r && j != h_c && min > matriz[i][j])
                min = matriz[i][j];
        }
        //SUBSTRACT MIN VALUE
        if (min != 0 && min != MAX_DIM2)
        {
            for (DIM1 j = 0; j < n; j++)
            {
                if (matriz[i][j] != MAX_DIM2)
                    matriz[i][j] -= min;
            }
            new_reduced_counter += min;
        }
    }

    // Get min value in each column and substract
    for (DIM1 j = 0; j < n; j++)
    {
        min = MAX_DIM2;
        //GET MIN VALUE
        for (DIM1 i = 0; i < n; i++)
        {
            if (i != h_r && j != h_c && min > matriz[i][j])
                min = matriz[i][j];
        }
        //SUBSTRACT MIN VALUE
        if (min != 0 && min != MAX_DIM2)
        {
            for (DIM1 i = 0; i < n; i++)
            {
                if (matriz[i][j] != MAX_DIM2)
                    matriz[i][j] -= min;
            }
            new_reduced_counter += min;
        }
    }

    double t2 = omp_get_wtime();

    cout << "RC: " << reduced_counter << endl;

    cout << "NRC: " << new_reduced_counter << endl;

    cout << setprecision(8) << "Time: " << t2 - t1 << endl;
}