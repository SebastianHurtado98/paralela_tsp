#include <iostream>
using namespace std;

int main() {
    int n=8, vecinos[n] = {-1, 2, -1, 3, 4, 2, -1, 2}, n_procesos = 4, cnt_v = 0;
    int *vecinos_por_visitar = new int[n_procesos - 1];
    int i = 0;
    while(i < n) {
        if (vecinos[i] != -1) {
            vecinos_por_visitar[cnt_v] = vecinos[i];
            cnt_v++;
        }
        i++;    
        if (cnt_v == n_procesos - 1 || i == n) {
            if (cnt_v < n_procesos - 1) {
                 for (int k = cnt_v; k < n_procesos - 1; k++) vecinos_por_visitar[k] = -1;
            }
            cout << "VISITANDO ";
            for (int k = 0; k < n_procesos - 1; k++) cout << vecinos_por_visitar[k] << " ";
            cout << endl;
            for (int k = 0; k < n_procesos - 1; k++) vecinos_por_visitar[k] = 0;
            cnt_v = 0;
        } 
    }
    return 0;
}