#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
using namespace std;

struct treeNode
{
    int occurrence;
    int treeIndex;
};

struct internalNodeTree
{
    int leftIndex;
    int rightIndex;
};

union occurrenceNode
{
    char character;
    treeNode occurrenceStruct;
    internalNodeTree internalNode;
};

struct codeAndCodeLen
{
    int len;
    bitset<16> code;
};

//===================================================================
// HEAP

class Heap
{
private:
    occurrenceNode *heap;
    int numberOfElements;

public:
    Heap(occurrenceNode *Heap) : heap{Heap} {}

    void printHeap()
    {
        for (int i = 0; i <= (this->numberOfElements - 1); ++i)
        {
            cout << "======\n";
            cout << "treeInde: " << this->heap[i].occurrenceStruct.treeIndex << '\n';
            cout << "ocore:  " << this->heap[i].occurrenceStruct.occurrence << '\n';
            cout << "======\n";
        }
        cout << '\n';
    }

    void fixHeapInsertion(int positionImLooking)
    {
        int fatherPostion = (positionImLooking - 1) / 2;
        while (fatherPostion != 0 || this->heap[fatherPostion].occurrenceStruct.occurrence > this->heap[positionImLooking].occurrenceStruct.occurrence)
        {
            occurrenceNode temp = this->heap[fatherPostion];
            this->heap[fatherPostion] = this->heap[positionImLooking];
            this->heap[positionImLooking] = temp;
            positionImLooking = fatherPostion;
            fatherPostion = (positionImLooking - 1) / 2;
        }
    }

    void insertAnode(occurrenceNode newNode)
    {
        ++this->numberOfElements;
        this->heap[this->numberOfElements - 1] = newNode;
        this->fixHeapInsertion(this->numberOfElements - 1);
    }

    void fixRemotion()
    {
        int fatherPostion = 0;
        int sonOne = 2 * fatherPostion + 1;
        int sonTwo = 2 * fatherPostion + 2;
        int smallInd = fatherPostion;
        while (true)
        {
            if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonOne].occurrenceStruct.occurrence && sonOne <= this->numberOfElements)
            {
                smallInd = sonOne;
            }
            if (this->heap[smallInd].occurrenceStruct.occurrence > this->heap[sonTwo].occurrenceStruct.occurrence && sonTwo <= this->numberOfElements)
            {
                smallInd = sonTwo;
            }
            if (smallInd != fatherPostion)
            {
                occurrenceNode temp = this->heap[smallInd];
                this->heap[smallInd] = this->heap[fatherPostion];
                this->heap[fatherPostion] = temp;
                fatherPostion = smallInd;
                sonOne = 2 * fatherPostion + 1;
                sonTwo = 2 * fatherPostion + 2;
            }
            else
            {
                break;
            }
        };
    }

    occurrenceNode removeMinimun()
    {
        occurrenceNode temp = this->heap[0];
        this->heap[0] = this->heap[this->numberOfElements - 1];
        --this->numberOfElements;
        this->fixRemotion();
        return temp;
    }

    int numberOfele()
    {
        return this->numberOfElements;
    }
};

//===================================================================
// TREE

class HuffmanTree
{
public:
    occurrenceNode *tree;
    int insertIndex = 0;
    int numberOfElements;

public:
    HuffmanTree(occurrenceNode *Tree, int nofElements) : tree{Tree}, numberOfElements{nofElements} {}

    int insertAnode(occurrenceNode newNode)
    {
        this->tree[this->insertIndex] = newNode;
        int indexToReturn = this->insertIndex;
        ++this->insertIndex;
        return indexToReturn;
    }

    void runThroughoutTheTree(int index, string codigo = "")
    {
        if (index >= 0 && index <= numberOfElements - 1)
        {
            cout << this->tree[index].character << ": " << codigo << '\n';
        }
        else
        {
            string newC = codigo + "0";
            runThroughoutTheTree(this->tree[index].internalNode.leftIndex, newC);
            string newC2 = codigo + "1";
            runThroughoutTheTree(this->tree[index].internalNode.rightIndex, newC2);
        }
    }

    void runThroughoutTheTree(unordered_map<char, codeAndCodeLen> *table, int index, string code = "")
    {
        if (index >= 0 && index <= numberOfElements - 1)
        {
            (*table)[this->tree[index].character] = codeAndCodeLen{int(code.length()), bitset<16>(code)};
        }
        else
        {
            string newC = code + "0";
            this->runThroughoutTheTree(table, this->tree[index].internalNode.leftIndex, newC);
            string newC2 = code + "1";
            this->runThroughoutTheTree(table, this->tree[index].internalNode.rightIndex, newC2);
        }
    }

    void printCodes()
    {
        this->runThroughoutTheTree(this->insertIndex - 1);
    }

    void populateSomeTableWithCodes(unordered_map<char, codeAndCodeLen> *table)
    {
        this->runThroughoutTheTree(table, this->insertIndex - 1);
    }
};

//===================================================================

int main()
{
    string fileName = "arquivoTeste.txt";
    int *occurenceVector = new int[256];
    int N_numberOfLeafs = 0;

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
    occurrenceNode *treePointer = new occurrenceNode[2 * N_numberOfLeafs - 1];

    Heap occurrenceHeap = Heap(heap);
    HuffmanTree tree = HuffmanTree(treePointer, N_numberOfLeafs);

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

    while (occurrenceHeap.numberOfele() > 1)
    {
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

    tree.printCodes();

    unordered_map<char, codeAndCodeLen> *codeTable = new unordered_map<char, codeAndCodeLen>();
    string t = "1";
    bitset<8> a(t);

    cout << "The number in binary is: " << a << '\n';

    tree.populateSomeTableWithCodes(codeTable);

    for (auto element : (*codeTable))
    {
        cout << element.first << ": " << element.second.code << " - " << element.second.len << '\n';
    }

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */