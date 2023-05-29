#include <fstream>
#include <iostream>
using namespace std;

struct occurenceNode {
    char character;
    int occurrence;
};

void fixHeapInsertion(occurenceNode *heap, int positionImLooking) {
    int fatherPostion = (positionImLooking-1)/2;
    while (fatherPostion != 0 || heap[fatherPostion].occurrence > heap[positionImLooking].occurrence) {
        occurenceNode temp = heap[fatherPostion];
        heap[fatherPostion] = heap[positionImLooking];
        heap[positionImLooking] = temp;
        positionImLooking = fatherPostion;
        fatherPostion = (positionImLooking-1)/2;
    }
}

occurenceNode *createHeap(int occurrenceVector[], int numberOfCharacters) {
    occurenceNode *heap = new occurenceNode[numberOfCharacters];
    int indexToInsertNewNode = 0;
    for (int *i = (occurrenceVector); i != (occurrenceVector + 256); ++i) {
        if(*(i) > 0) {
            occurenceNode newNode = occurenceNode{char(i-occurrenceVector), *(i)};
            heap[indexToInsertNewNode] = newNode;
            fixHeapInsertion(heap, indexToInsertNewNode);
            ++indexToInsertNewNode;
        }
    }
    return heap;
}


int main()
{
    string fileName = "arquivoTeste.txt";
    int *occurenceVector = new int[256];
    int N_numberOfLeafs = 0;
    
    //variables to store the first and final index where a char occurs
    /* For now I wont do it */
    /* int firstCharOccurrence = -1;
    int lastCharOccurrence = -1; */

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
            N_numberOfLeafs += 1;
        }
    }

    cout << "Now i'm going to create the heap..\n";
    occurenceNode* heap = createHeap(occurenceVector, N_numberOfLeafs);

    cout << "I'm going to print the heap now...\n";
    for (int i = 0; i < N_numberOfLeafs; ++i) {
        cout << "======\n";
        cout << "char: " << heap[i].character << '\n';
        cout << "occurrence: " << heap[i].occurrence << '\n';
        cout << "======\n";
    }
    cout << '\n';


    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */