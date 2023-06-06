#include <fstream>
#include <iostream>
using namespace std;

struct treeNode {
    int occurrence;
    int treeIndex;
};

struct internalNodeTree{
    int leftIndex;
    int rightIndex;
};


union occurrenceNode {
    char character;
    treeNode occurrenceStruct;
    internalNodeTree internalNode;
};




//===================================================================
//HEAP

class Heap {
    private:
        occurrenceNode *heap;
        int lastIndex;
    public:

        Heap(occurrenceNode *Heap) : heap{Heap} {}

        void printHeap() {
            for (int i = 0; i <= this->lastIndex; ++i) {
                cout << this->heap[i].character << '-' << this->heap[i].occurrenceStruct.occurrence << ' '; 
            }
            cout << '\n';
        }

        void fixRemotion() {
            int fatherPostion = 0;
            int sonOne = 2 * fatherPostion + 1;
            int sonTwo = 2 * fatherPostion + 2;
            int smallInd = fatherPostion;
            while (true) {
                if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonOne].occurrenceStruct.occurrence && sonOne <= this->lastIndex) {
                    smallInd = sonOne;
                    
                }
                if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonTwo].occurrenceStruct.occurrence && sonTwo <= this->lastIndex) {
                    smallInd = sonTwo;
                }
                if (smallInd != fatherPostion) {
                    occurrenceNode temp = this->heap[smallInd];
                    this->heap[smallInd] = this->heap[fatherPostion];
                    this->heap[fatherPostion] = temp;
                    fatherPostion = smallInd;
                    sonOne = 2 * fatherPostion + 1;
                    sonTwo = 2 * fatherPostion + 2;   
                }else {
                    break;
                }
            };
        }

        occurrenceNode removeMinimun() {
            occurrenceNode temp = this->heap[0];
            this->heap[0] = this->heap[lastIndex];
            this->fixRemotion();
            --this->lastIndex;
            return temp;
        }

        void fixHeapInsertion(int positionImLooking) {
            int fatherPostion = (positionImLooking-1)/2;
            while (fatherPostion != 0 || this->heap[fatherPostion].occurrenceStruct.occurrence > this->heap[positionImLooking].occurrenceStruct.occurrence) {
                occurrenceNode temp = this->heap[fatherPostion];
                this->heap[fatherPostion] = this->heap[positionImLooking];
                this->heap[positionImLooking] = temp;
                positionImLooking = fatherPostion;
                fatherPostion = (positionImLooking-1)/2;
            }
        }

        void insertAnode(int indexTree, int occurrence) {
            occurrenceNode newNode; /* = occurrenceNode{character, occurrence}; */
            newNode.occurrenceStruct.occurrence = occurrence;
            newNode.occurrenceStruct.treeIndex = indexTree;
            this->heap[this->lastIndex] = newNode;
            this->fixHeapInsertion(this->lastIndex);
            ++this->lastIndex;
        }

        void populateHeap(int occurrenceVector[]) {
            for (int *i = (occurrenceVector); i != (occurrenceVector + 256); ++i) {
                if(*(i) > 0) {
                    this->insertAnode(char(i-occurrenceVector), *(i));
                }
            }
            
        }

        int getLastIndex() {
            return this->lastIndex;
        }

};

//===================================================================
//TREE

class HuffmanTree {
    private:
        occurrenceNode *tree;
        int insertIndex = 0;
    public:
        HuffmanTree(occurrenceNode *Tree) : tree{Tree} {}

        int insertAnode(occurrenceNode newNode) {
            this->tree[this->insertIndex] = newNode;
            int indexToReturn = this->insertIndex;
            ++this->insertIndex;
            return indexToReturn;
        }

};

//===================================================================


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
    cout << "and I'm going to create the heap at the same time\n";

    cout << "Now i'm going to create the heap..\n";

    

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

    occurrenceNode *heap = new occurrenceNode[N_numberOfLeafs];
    occurrenceNode *treePointer = new occurrenceNode[2*N_numberOfLeafs-1];

    Heap occurrenceHeap = Heap(heap);
    HuffmanTree tree = HuffmanTree(treePointer);

    
    /* occurrenceHeap.populateHeap(occurenceVector);

    while(occurrenceHeap.getLastIndex() > 0) {
        occurrenceNode nodeOne = occurrenceHeap.removeMinimun();
        occurrenceNode nodeTwo = occurrenceHeap.removeMinimun();


    }
 */
    occurrenceHeap.printHeap();

    cout << "Remove the minimum: " << occurrenceHeap.removeMinimun().character << '\n';

    occurrenceHeap.printHeap();

    cout << "Remove the minimum: " << occurrenceHeap.removeMinimun().character << '\n';

    occurrenceHeap.printHeap();

    
    cout << "Remove the minimum: " << occurrenceHeap.removeMinimun().character << '\n';

    occurrenceHeap.printHeap();

    
    cout << "Remove the minimum: " << occurrenceHeap.removeMinimun().character << '\n';

    occurrenceHeap.printHeap();






    /*occurrenceNode* heap = populateHeap(occurenceVector, N_numberOfLeafs);

    



    cout << "I'm going to print the heap now...\n";
    for (int i = 0; i < N_numberOfLeafs; ++i) {
        cout << "======\n";
        cout << "char: " << heap[i].character << '\n';
        cout << "occurrence: " << heap[i].occurrence << '\n';
        cout << "======\n";
    }
    cout << '\n';
*/

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */