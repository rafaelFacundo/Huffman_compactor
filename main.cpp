#include <fstream>
#include <iostream>
#include <string>
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
        int numberOfElements;
    public:

        Heap(occurrenceNode *Heap) : heap{Heap} {}

        void printHeap() {
            for (int i = 0; i <= (this->numberOfElements-1); ++i) {
                cout << "======\n";
                cout << "treeInde: " << this->heap[i].occurrenceStruct.treeIndex  << '\n';
                cout << "ocore:  " << this->heap[i].occurrenceStruct.occurrence << '\n';
                cout << "======\n";
            }
            cout << '\n';
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

        void insertAnode(occurrenceNode newNode) {
            ++this->numberOfElements;
            this->heap[this->numberOfElements-1] = newNode;
            this->fixHeapInsertion(this->numberOfElements-1);
            
        }

        void fixRemotion() {
            int fatherPostion = 0;
            int sonOne = 2 * fatherPostion + 1;
            int sonTwo = 2 * fatherPostion + 2;
            int smallInd = fatherPostion;
            while (true) {
                if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonOne].occurrenceStruct.occurrence && sonOne <= this->numberOfElements) {
                    smallInd = sonOne;
                    
                }
                if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonTwo].occurrenceStruct.occurrence && sonTwo <= this->numberOfElements) {
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
            this->heap[0] = this->heap[this->numberOfElements-1];
            --this->numberOfElements;
            this->fixRemotion();
            return temp;
        }

        int numberOfele() {
            return this->numberOfElements;
        }

};

//===================================================================
//TREE

class HuffmanTree {
    public:
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

        void printCodes(int index, string codigo = "") {
            if (index >= 0 && index <= 4) {
                cout << this->tree[index].character << ": " << codigo << '\n';
            }else {
                string newC = codigo + "0";
                printCodes(this->tree[index].internalNode.leftIndex, newC);
                string newC2 = codigo + "1";
                printCodes(this->tree[index].internalNode.rightIndex, newC2);
            }
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


    for (int *i = occurenceVector; i != (occurenceVector + 256); ++i)
    {
        if (*(i) > 0)
        {
            occurrenceNode treeNode;
            treeNode.character = (char)(i - occurenceVector);
            int indexInserted = tree.insertAnode(treeNode);

            occurrenceNode newNode;
            newNode.occurrenceStruct.treeIndex = indexInserted;
            newNode.occurrenceStruct.occurrence = *(i);
            occurrenceHeap.insertAnode(newNode);

           
        }
    }

    while(occurrenceHeap.numberOfele() > 1) {
        occurrenceNode nodeOne = occurrenceHeap.removeMinimun();
        occurrenceNode nodeTwo = occurrenceHeap.removeMinimun();

        occurrenceNode newTreeNode;
        newTreeNode.internalNode.leftIndex = nodeOne.occurrenceStruct.treeIndex;
        newTreeNode.internalNode.rightIndex = nodeTwo.occurrenceStruct.treeIndex;

        int indexInsertedOnTree = tree.insertAnode(newTreeNode);

        occurrenceNode newHeapNode;
        newHeapNode.occurrenceStruct.occurrence = nodeOne.occurrenceStruct.occurrence + nodeTwo.occurrenceStruct.occurrence;
        newHeapNode.occurrenceStruct.treeIndex = indexInsertedOnTree;
        occurrenceHeap.insertAnode(newHeapNode);
    }

    /* cout << "Vou printar as folhas das árvores: ======\n";

    cout << "==============\n";

    for (int i = 0; i < 5; ++i) {
        cout << tree.tree[i].character << " \n";
    }

    cout << "==============\n";

    cout << "Vou printar os nós internos: =====\n";

    for (int i = 5; i < 9; ++i) {
        cout << "==============\n";
        cout << "left ind: " << tree.tree[i].internalNode.leftIndex << '\n';
        cout << "right ind: " << tree.tree[i].internalNode.rightIndex << '\n';
        cout << "==============\n";
    }
 */


    tree.printCodes(8);





    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */