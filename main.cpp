#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    string fileName = "arquivoTeste.txt";
    int *occurenceVector = new int[256];

    ifstream file(fileName.data(), std::ios_base::in | std::ios_base::binary);

    unsigned char byte;

    while (true)
    {
        byte = file.get();
        if (file.eof())
            break;
        occurenceVector[(int)byte] += 1;
    }

    cout << "runing throught the occurence vector \n";
    for (int *i = occurenceVector; i != (occurenceVector + 256); ++i)
    {
        if (*(i) > 0)
        {
            cout << "found in the position: " << i - occurenceVector << '\n';
            cout << "number of occurences: " << *(i) << '\n';
            cout << "The code in char is: " << (char)(i - occurenceVector) << '\n';
        }
    }

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */