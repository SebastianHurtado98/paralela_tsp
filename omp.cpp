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

    omp_set_num_threads(5);

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

    
    DIM2 *reduced_counter = new DIM2[n];

    // Get min value in each row and substract

    #pragma omp parallel for private(i, min) shared(matriz, reduced_counter)
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
            reduced_counter[i] = min;
        } else {
            reduced_counter[i] = 0;
        }
    }

    // Get min value in each column and substract

    #pragma omp parallel for private(i, min) shared(matriz, reduced_counter)
    for (i = 0; i < n; i++)
    {
        min = MAX_DIM2;
        //GET MIN VALUE
        for (DIM1 j = 0; j < n; j++)
        {
            if (min > matriz[j][i])
            {
                min = matriz[j][i];
            }
        }
        //SUBSTRACT MIN VALUE
        if (min != 0 && min != MAX_DIM2)
        {
            for (DIM1 j = 0; j < n; j++)
            {
                if (matriz[j][i] != MAX_DIM2)
                {
                    matriz[j][i] -= min;
                }
            }
            
            reduced_counter[i] += min;
        }
    }
    
    DIM2 total_reduced_counter = 0;
    for (i = 0; i < n; i++) total_reduced_counter += reduced_counter[i];


    double t2 = omp_get_wtime();

    cout << "RC: " << total_reduced_counter << endl;

    cout << setprecision(8) << "Time: " << t2 - t1 << endl;
}