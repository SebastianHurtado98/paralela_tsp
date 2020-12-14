#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream file("times.txt");

    int x;
    int count = 0;
    while (file >> x) {
        count += x;
    }
    cout << count << endl;


}