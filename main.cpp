#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <unordered_map>
using namespace std;

struct treeNode
{
    int occurrence;
    uint16_t treeIndex;
};

struct internalNodeTree
{
    uint16_t leftIndex;
    uint16_t rightIndex;
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
    bitset<32> code;
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

    void printLeafs()
    {
        for (int i = 0; i < this->numberOfElements; ++i)
        {
            cout << this->tree[i].character << ' ';
        }
        cout << '\n';
    }

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
            (*table)[this->tree[index].character] = codeAndCodeLen{int(code.length()), bitset<32>(code)};
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
        cout << "o indice de comec " << this->insertIndex - 1 << "\n";
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
            //(*fileToWrite) << (unsigned char)i->internalNode.leftIndex << (unsigned char)i->internalNode.rightIndex;
            (*fileToWrite).write((char *)&(i->internalNode.leftIndex), sizeof(uint16_t));
            (*fileToWrite).write((char *)&(i->internalNode.rightIndex), sizeof(uint16_t));
            cout << bitset<8>(i->internalNode.leftIndex) << bitset<8>(i->internalNode.rightIndex);
            ++i;
        }

        cout << '\n';

        /* (*fileToWrite) << endl; */
    }

    void populateTreeFromAFile(ifstream *fileToRead)
    {
        uint8_t byteToPutInTree;
        int i = 0;
        uint16_t indexes = 0;

        cout << "O numero de elemenetos é : " << this->numberOfElements << '\n';
        while (i < this->numberOfElements)
        {

            cout << "o valor de i: " << i << '\n';
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
            // byteToPutInTree = fileToRead->get();
            // leftIndex
            fileToRead->read(reinterpret_cast<char *>(&indexes), sizeof(uint16_t));
            newNode.internalNode.leftIndex = indexes;
            cout << "EU LLI O INDEXX esquerd: " << (int)newNode.internalNode.leftIndex << '\n';
            // byteToPutInTree = fileToRead->get();
            fileToRead->read(reinterpret_cast<char *>(&indexes), sizeof(uint16_t));
            newNode.internalNode.rightIndex = indexes;
            cout << "EU LLI O INDEXX direit : " << (int)newNode.internalNode.rightIndex << '\n';

            this->insertAnode(newNode);
            ++i;
        }
    }

    void printTreeAsVector()
    {
        for (int i = 0; i < 5; ++i)
        {
            cout << this->tree[i].character << '\n';
        }
        for (int i = 5; i < this->insertIndex; ++i)
        {
            cout << "========\n";
            cout << "left: " << (int)this->tree[i].internalNode.leftIndex << '\n';
            cout << "right: " << (int)this->tree[i].internalNode.rightIndex << '\n';
            cout << "=========\n";
        }
    }

    void writeUnzzipedFileByTheTree(ofstream *fileToWrite, ifstream *fileToRead, int numberOfBytes)
    {
        bitset<8> byteReaded;
        int byteIndex = 0;
        int treeIndex = this->insertIndex - 1;
        int numberOfBytesWritten = 0;
        while (numberOfBytesWritten < numberOfBytes)
        {
            cout << "o numero de bytes escritos é: " << numberOfBytesWritten << '\n';
            byteIndex = 7;
            byteReaded = bitset<8>(fileToRead->get());
            cout << "Eu li o byte: " << byteReaded << '\n';

            while (byteIndex >= 0 && numberOfBytesWritten < numberOfBytes)
            {
                cout << "o numero de bytes escritos é: " << numberOfBytesWritten << '\n';
                if (treeIndex < this->numberOfElements)
                {
                    cout << "escrevi o caracte " << (char)this->tree[treeIndex].character << '\n';
                    (*fileToWrite) << (unsigned char)this->tree[treeIndex].character;
                    numberOfBytesWritten += 1;
                    treeIndex = this->insertIndex - 1;
                }
                else if (byteIndex < 0 && treeIndex >= this->numberOfElements)
                {
                    cout << "Entrei no segundo else == \n";
                    break;
                }
                else
                {
                    cout << "Entrei no ultimo else ===\n";
                    if (byteReaded[byteIndex] == 1)
                    {
                        cout << "no ultimo else - primeiro - valor do bit lido é 1 -  " << byteReaded[byteIndex] << '\n';
                        treeIndex = (int)this->tree[treeIndex].internalNode.rightIndex;
                        cout << "Tree index recebeu == " << treeIndex << '\n';
                        cout << "O treeNode a ser recebido ==== \n";
                        cout << "======\n";
                        cout << "left: " << (int)this->tree[treeIndex].internalNode.leftIndex << '\n';
                        cout << "right: " << (int)this->tree[treeIndex].internalNode.rightIndex << '\n';
                        cout << "=======\n";
                    }
                    else
                    {
                        cout << "no ultimo else - segundo - valor do bit lido é 0 - " << byteReaded[byteIndex] << '\n';
                        treeIndex = (int)this->tree[treeIndex].internalNode.leftIndex;
                        cout << "Tree index recebeu == " << treeIndex << '\n';
                        cout << "O treeNode a ser recebido ==== \n";
                        cout << "======\n";
                        cout << "left: " << (int)this->tree[treeIndex].internalNode.leftIndex << '\n';
                        cout << "right: " << (int)this->tree[treeIndex].internalNode.rightIndex << '\n';
                        cout << "=======\n";
                    }
                    cout << "O index da arvore recebe == " << treeIndex << '\n';
                    --byteIndex;
                }
                cout << "=====================\n";
            }
        }
        (*fileToWrite) << std::flush;
    }
};

//==================================================================

void writeCompiledCodes(unordered_map<unsigned char, codeAndCodeLen> *table, ifstream *fileToRead, ofstream *fileToWrite)
{
    unsigned char byteReaded;
    bitset<8> byteToWrite(0);
    uint8_t numberOfShifts = 0;
    codeAndCodeLen charactereCode;
    ostringstream tempString;

    while (true)
    {
        byteReaded = fileToRead->get();
        if (fileToRead->eof())
            break;
        cout << "=== li o caractere: ==== " << (char)byteReaded << '\n';
        codeAndCodeLen charactereCode = (*table)[byteReaded];
        if (numberOfShifts < 8 && charactereCode.len <= (8 - numberOfShifts))
        {
            cout << "=== já é pequeno - primeiro if = \n";
            byteToWrite <<= charactereCode.len;
            byteToWrite |= bitset<byteToWrite.size()>(charactereCode.code.to_ulong());
            cout << "=== o byte depois do ou " << byteToWrite << '\n';
            numberOfShifts += charactereCode.len;
            cout << "=== quantidade de shifts " << (int)numberOfShifts << '\n';
        }
        else if (numberOfShifts < 8 && charactereCode.len > (8 - numberOfShifts))
        {
            // codigo antigo
            /* int numberOfShiftsToDoInCode = charactereCode.len - (8 - numberOfShifts);
            bitset<32> copyOfByteReaded = (charactereCode.code >> numberOfShiftsToDoInCode);
            byteToWrite <<= (8 - numberOfShifts);
            byteToWrite |= copyOfByteReaded;

            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            copyOfByteReaded <<= numberOfShiftsToDoInCode;
            copyOfByteReaded.flip();
            charactereCode.code &= copyOfByteReaded;
            byteToWrite.reset();
            byteToWrite |= charactereCode.code;

            numberOfShifts = numberOfShiftsToDoInCode; */
            cout << "===  é maior - tamanho é " << charactereCode.len << "e o num " << (8 - numberOfShifts) << '\n';
            bitset<32> copyOfByteReaded;
            int shiftsTodo = 0;

            while (charactereCode.len > 0)
            {
                if (charactereCode.len <= (8 - numberOfShifts))
                {
                    cout << "=== agora ficou menor\n";
                    byteToWrite <<= charactereCode.len;
                    cout << "=== apos o shift do taman " << charactereCode.len << '\n';
                    cout << "=== " << byteToWrite << '\n';
                    cout << "byte a ser escrito1: " << byteToWrite << '\n';
                    byteToWrite |= bitset<byteToWrite.size()>(charactereCode.code.to_ulong());
                    cout << "=== apos o ou no pri " << byteToWrite << '\n';
                    numberOfShifts += charactereCode.len;
                    cout << "number of shif " << numberOfShifts << '\n';
                    charactereCode.len = 0;
                }
                else
                {
                    cout << "=== entrei no else do segundo while \n";
                    shiftsTodo = (charactereCode.len - (8 - numberOfShifts));
                    cout << "=== vou dar um shift de " << shiftsTodo << '\n';
                    copyOfByteReaded = charactereCode.code >> shiftsTodo;
                    cout << "=== o resul do shift é " << copyOfByteReaded << '\n';
                    charactereCode.len = shiftsTodo;
                    cout << "=== ainda soubrou pra colocar " << charactereCode.len << '\n';
                    cout << "=== antes do shift com a copia: " << byteToWrite << '\n';
                    byteToWrite <<= (8 - numberOfShifts);
                    // doing here
                    cout << "=== antes do ou com a copia: " << byteToWrite << '\n';
                    byteToWrite |= bitset<byteToWrite.size()>(copyOfByteReaded.to_ulong());

                    cout << "=== depois do ou com a copia: " << byteToWrite << '\n';
                    (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
                    copyOfByteReaded <<= shiftsTodo;
                    copyOfByteReaded.flip();
                    charactereCode.code &= copyOfByteReaded;
                    byteToWrite.reset();
                    // numberOfShifts = 0;
                    numberOfShifts = 0;
                }
            }
        }

        if (numberOfShifts == 8)
        {
            cout << "shif = 8 byte a ser escrito " << byteToWrite << '\n';
            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            numberOfShifts = 0;
            byteToWrite.reset();
        }
    }

    if (numberOfShifts > 0 && numberOfShifts < 8)
    {
        cout << "\n sai do while antes sobrando bit: " << (int)numberOfShifts << '\n';
        byteToWrite <<= (8 - numberOfShifts);
        (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
    }

    cout << '\n';

    (*fileToWrite) << std::flush;
}

//===================================================================

int main()
{

    string fileName = "6.bmp";
    int *occurenceVector = new int[256];
    uint16_t N_numberOfLeafs = 0;
    int numberOfBytes = 0;

    ifstream *file = new ifstream(fileName.data(), std::ios_base::in | std::ios_base::binary);
    ofstream *outputFile = new ofstream("said6.bmp", std::ios_base::out | std::ios_base::binary);

    unsigned char byte;
    while (true)
    {
        byte = file->get();
        if (file->eof())
        {
            break;
        }
        if (occurenceVector[(int)byte] <= 0)
        {
            N_numberOfLeafs += 1;
        }
        occurenceVector[(int)byte] += 1;
        numberOfBytes += 1;
    }

    if (N_numberOfLeafs < 1)
    {
        cout << "aaaaa\n";
        return 0;
    }
    if (N_numberOfLeafs < 2)
    {
        //(*outputFile) << (unsigned char)N_numberOfLeafs;
        (*outputFile).write((char *)&N_numberOfLeafs, sizeof(uint16_t));
        (*outputFile) << std::flush;
        //(*outputFile) << (unsigned char)numberOfBytes;
        (*outputFile).write((char *)&numberOfBytes, sizeof(numberOfBytes));
        int i = 0;
        while (occurenceVector[i] == 0)
            ++i;
        (*outputFile) << (unsigned char)i;
        (*outputFile) << std::flush;
        return 0;
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

    unordered_map<unsigned char, codeAndCodeLen> *codeTable = new unordered_map<unsigned char, codeAndCodeLen>();

    tree.populateSomeTableWithCodes(codeTable);

    // tree.printTreeAsVector();

    for (auto element : (*codeTable))
    {
        cout << element.first << ": " << element.second.code << " - " << element.second.len << '\n';
    }

    file->clear();
    file->seekg(0);

    //(*outputFile) << (unsigned char)N_numberOfLeafs;
    cout << "numero de folh" << N_numberOfLeafs << '\n';
    (*outputFile).write((char *)&N_numberOfLeafs, sizeof(uint16_t));

    cout << "numberof lead: " << (int)N_numberOfLeafs << '\n';

    tree.writeTreInAfile(outputFile);

    //(*outputFile) << std::flush;
    //(*outputFile) << (unsigned char)numberOfBytes;
    (*outputFile).write((char *)&numberOfBytes, sizeof(numberOfBytes));

    cout << "Number of bytes: " << numberOfBytes << '\n';

    writeCompiledCodes(codeTable, file, outputFile);

    (*outputFile).close();

    return 0;

    /* ==================================================================================== */
    /* ==================================================================================== */
    /* ==================================================================================== */

    ifstream *saida = new ifstream("said5.pdf", std::ios_base::in | std::ios_base::binary);
    ofstream *saidaDescomp = new ofstream("descomp5.pdf", std::ios_base::out | std::ios_base::binary);

    // int numberOfDiferentElements = (int)saida->get();
    int numberOfDiferentElements = 0;
    saida->read(reinterpret_cast<char *>(&numberOfDiferentElements), sizeof(uint16_t));

    if (saida->eof())
        return 0;

    if (numberOfDiferentElements < 2)
    {
        cout << "NUmero de elementos diferentes " << numberOfDiferentElements << '\n';
        int numberOfBytesToBeWrite = 0;
        saida->read(reinterpret_cast<char *>(&numberOfBytesToBeWrite), sizeof(int));
        cout << "NUmer de bytes a ser escrito " << numberOfBytesToBeWrite << '\n';
        unsigned char charToBeWrite = (char)saida->get();
        cout << "Char a ser escrito " << charToBeWrite << '\n';
        while (numberOfBytesToBeWrite > 0)
        {
            (*saidaDescomp) << charToBeWrite;
            --numberOfBytesToBeWrite;
        }
        (*saidaDescomp) << std::flush;
        return 0;
    }

    cout << "primeiro byte: " << numberOfDiferentElements << '\n';

    occurrenceNode *pointerToTheTree = new occurrenceNode[2 * numberOfDiferentElements - 1];

    HuffmanTree treeFromTheFile = HuffmanTree(pointerToTheTree, numberOfDiferentElements);

    treeFromTheFile.populateTreeFromAFile(saida);

    treeFromTheFile.runThroughoutTheTree(2 * numberOfDiferentElements - 2);

    treeFromTheFile.printTreeAsVector();

    cout << "=====\n";

    // numberOfBytes = (int)saida->get();
    saida->read(reinterpret_cast<char *>(&numberOfBytes), sizeof(int));

    cout
        << "O numero de bytes é: " << numberOfBytes << '\n';

    treeFromTheFile.writeUnzzipedFileByTheTree(saidaDescomp, saida, numberOfBytes);

    /* while (true)
    {
        byte = file->get();
        if (file->eof())
            break;
    } */

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */
