#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
using namespace std;

struct treeNode
{
    int occurrence;
    uint8_t treeIndex;
};

struct internalNodeTree
{
    uint8_t leftIndex;
    uint8_t rightIndex;
};

union occurrenceNode
{
    unsigned char character;
    treeNode occurrenceStruct;
    internalNodeTree internalNode;
};

struct codeAndCodeLen
{
    int len;
    bitset<8> code;
};

//===================================================================
// HEAP

class Heap
{
private:
    occurrenceNode *heap;
    int numberOfElements = 0;

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

    void runThroughoutTheTree(unordered_map<unsigned char, codeAndCodeLen> *table, int index, string code = "")
    {
        if (index >= 0 && index <= numberOfElements - 1)
        {
            (*table)[this->tree[index].character] = codeAndCodeLen{int(code.length()), bitset<8>(code)};
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

    void populateSomeTableWithCodes(unordered_map<unsigned char, codeAndCodeLen> *table)
    {
        this->runThroughoutTheTree(table, this->insertIndex - 1);
    }

    void writeTreInAfile(ofstream *fileToWrite)
    {
        cout << "Vou escrever a arvore no arquivo \n";
        cout << "Resultado ==========\n";
        occurrenceNode *i = this->tree;
        while (i != (this->tree + this->numberOfElements))
        {
            (*fileToWrite) << (unsigned char)i->character;

            cout << bitset<8>(i->character);
            ++i;
        };

        while (i != (this->tree + this->insertIndex))
        {
            /* cout << "Esout escrevendo o nó interno---------\n";
            cout << "Esque " << i->internalNode.leftIndex << '\n';
            cout << "Dire  " << i->internalNode.rightIndex << '\n';
            cout << "==============\n"; */
            (*fileToWrite) << (unsigned char)i->internalNode.leftIndex << (unsigned char)i->internalNode.rightIndex;

            cout << bitset<8>(i->internalNode.leftIndex) << bitset<8>(i->internalNode.rightIndex);
            ++i;
        }

        cout << '\n';

        (*fileToWrite) << endl;
    }

    void populateTreeFromAFile(ifstream *fileToRead)
    {
        uint8_t byteToPutInTree;
        int i = 0;
        while (i < this->numberOfElements)
        {

            byteToPutInTree = fileToRead->get();
            occurrenceNode newNode;
            newNode.character = byteToPutInTree;
            this->insertAnode(newNode);
            ++i;
        }

        int lastIn = 2 * this->numberOfElements - 1;
        while (i < lastIn)
        {

            occurrenceNode newNode;
            byteToPutInTree = fileToRead->get();

            newNode.internalNode.leftIndex = byteToPutInTree;
            cout << "EU LLI O INDEXX: " << (int)newNode.internalNode.leftIndex << '\n';
            byteToPutInTree = fileToRead->get();
            newNode.internalNode.rightIndex = byteToPutInTree;
            cout << "EU LLI O INDEXX: " << (int)newNode.internalNode.rightIndex << '\n';

            this->insertAnode(newNode);
            ++i;
        }
    }
};

//==================================================================

void writeCompiledCodes(unordered_map<unsigned char, codeAndCodeLen> *table, ifstream *fileToRead, ofstream *fileToWrite)
{
    unsigned char byteReaded;
    bitset<8> byteToWrite(0);
    uint8_t numberOfShifts = 0;

    while (true)
    {
        byteReaded = fileToRead->get();
        if (fileToRead->eof())
            break;
        codeAndCodeLen charactereCode = (*table)[byteReaded];
        if (numberOfShifts < 8 && charactereCode.len <= (8 - numberOfShifts))
        {
            /* cout << "==========\n";
            cout << "vou escrever o: " << (char)(byteReaded) << '\n';
            cout << "O código dele é: " << charactereCode.code << '\n';
            cout << "O tamanho do código é: " << charactereCode.len << '\n'; */
            byteToWrite <<= charactereCode.len;
            byteToWrite |= charactereCode.code;
            /* cout << "o byte a ser escrito é o: " << byteToWrite << '\n';
            cout << "==========\n"; */
            numberOfShifts += charactereCode.len;
        }
        else if (numberOfShifts < 8 && charactereCode.len > (8 - numberOfShifts))
        {
            /* cout << "caractere passou do limite\n";
            cout << "Só restam: " << (8 - numberOfShifts) << " posições\n";
            cout << "Mas o tam do code é: " << charactereCode.len << '\n'; */
            int numberOfShiftsToDoInCode = charactereCode.len - (8 - numberOfShifts);
            bitset<8> copyOfByteReaded = (charactereCode.code >> numberOfShiftsToDoInCode);
            /* cout << "Após a copia e shift ficou: " << copyOfByteReaded << '\n'; */
            byteToWrite <<= (8 - numberOfShifts);
            byteToWrite |= copyOfByteReaded;
            cout << byteToWrite;
            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            /* cout << "o byte a ser escrito: " << byteToWrite << " - ";
            cout << "zerar"; */
            copyOfByteReaded <<= numberOfShiftsToDoInCode;
            copyOfByteReaded.flip();
            charactereCode.code &= copyOfByteReaded;
            byteToWrite.reset();
            byteToWrite |= charactereCode.code;
            cout << byteToWrite;
            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            /* cout << byteToWrite << '\n'; */
            numberOfShifts = numberOfShiftsToDoInCode;
        }

        if (numberOfShifts == 8)
        {
            /* cout << "atingiu o limite ===\n"; */
            cout << byteToWrite;
            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            numberOfShifts = 0;
            byteToWrite.reset();
        }
    }
    cout << '\n';
    (*fileToWrite) << endl;
}

//===================================================================

int main()
{
    string fileName = "arquivoTeste.txt";
    int *occurenceVector = new int[256];
    uint8_t N_numberOfLeafs = 0;

    ifstream *file = new ifstream(fileName.data(), std::ios_base::in | std::ios_base::binary);
    ofstream *outputFile = new ofstream("said.txt", std::ios_base::out | std::ios_base::binary);

    unsigned char byte;

    while (true)
    {
        byte = file->get();
        if (file->eof())
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

    /* tree.printCodes(); */

    unordered_map<unsigned char, codeAndCodeLen> *codeTable = new unordered_map<unsigned char, codeAndCodeLen>();

    tree.populateSomeTableWithCodes(codeTable);

    for (auto element : (*codeTable))
    {
        cout << element.first << ": " << element.second.code << " - " << element.second.len << '\n';
    }

    file->clear();
    file->seekg(0);
    writeCompiledCodes(codeTable, file, outputFile);

    /* outputFile->close();
    ifstream *outputFile2 = new ifstream("said.txt", std::ios_base::in | std::ios_base::binary);

    while (true)
    {
        byte = outputFile2->get();
        if (outputFile2->eof())
            break;
        cout << bitset<8>(byte);
    }

    cout << '\n'; */

    ofstream *outputFile2 = new ofstream("testArvore.txt", std::ios_base::out | std::ios_base::binary);

    tree.writeTreInAfile(outputFile2);

    outputFile2->close();

    /* ============================= */

    ifstream *outputFile3 = new ifstream("testArvore.txt", std::ios_base::in | std::ios_base::binary);

    occurrenceNode *treePointer2 = new occurrenceNode[2 * N_numberOfLeafs - 1];

    HuffmanTree tree2 = HuffmanTree(treePointer2, N_numberOfLeafs);

    tree2.populateTreeFromAFile(outputFile3);

    cout << "leitura do arquivo da arvore\n";
    outputFile3->clear();
    outputFile3->seekg(0);
    while (true)
    {
        byte = outputFile3->get();
        if (outputFile3->eof())
            break;
        cout << bitset<8>(byte);
    }

    cout << '\n';
    cout << "====================================\n";
    cout << "PRINTAR ARVORE DE ANTES ===\n";
    tree.runThroughoutTheTree(2 * N_numberOfLeafs - 2);
    cout << "PRINTAR ARVORE LIDA ========\n";
    tree2.runThroughoutTheTree(2 * N_numberOfLeafs - 2);

    /* cout << "Arvore 1 =======================\n";

    tree.runThroughoutTheTree(2 * N_numberOfLeafs - 2);

    cout << "Arvore 2 ========================\n";

    tree2.runThroughoutTheTree(2 * N_numberOfLeafs - 2); */

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */