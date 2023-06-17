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
            this->runThroughoutTheTree(table, this->tree[index].internalNode.leftIndex, code + "0");
            this->runThroughoutTheTree(table, this->tree[index].internalNode.rightIndex, code + "1");
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
        occurrenceNode *i = this->tree;
        while (i != (this->tree + this->numberOfElements))
        {
            (*fileToWrite) << (unsigned char)i->character;
            ++i;
        };

        while (i != (this->tree + this->insertIndex))
        {
            (*fileToWrite).write((char *)&(i->internalNode.leftIndex), sizeof(uint16_t));
            (*fileToWrite).write((char *)&(i->internalNode.rightIndex), sizeof(uint16_t));
            ++i;
        }

        /* (*fileToWrite) << endl; */
    }

    void populateTreeFromAFile(ifstream *fileToRead)
    {
        uint8_t byteToPutInTree;
        int i = 0;
        uint16_t indexes = 0;

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

            fileToRead->read(reinterpret_cast<char *>(&indexes), sizeof(uint16_t));
            newNode.internalNode.leftIndex = indexes;

            fileToRead->read(reinterpret_cast<char *>(&indexes), sizeof(uint16_t));
            newNode.internalNode.rightIndex = indexes;

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

            byteIndex = 7;
            byteReaded = bitset<8>(fileToRead->get());

            while (byteIndex >= 0 && numberOfBytesWritten < numberOfBytes)
            {

                if (treeIndex < this->numberOfElements)
                {

                    (*fileToWrite) << (unsigned char)this->tree[treeIndex].character;
                    numberOfBytesWritten += 1;
                    treeIndex = this->insertIndex - 1;
                }
                else if (byteIndex < 0 && treeIndex >= this->numberOfElements)
                {

                    break;
                }
                else
                {

                    if (byteReaded[byteIndex] == 1)
                    {

                        treeIndex = (int)this->tree[treeIndex].internalNode.rightIndex;
                    }
                    else
                    {

                        treeIndex = (int)this->tree[treeIndex].internalNode.leftIndex;
                    }
                    --byteIndex;
                }
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
        codeAndCodeLen charactereCode = (*table)[byteReaded];
        if (numberOfShifts < 8 && charactereCode.len <= (8 - numberOfShifts))
        {
            byteToWrite <<= charactereCode.len;
            byteToWrite |= bitset<byteToWrite.size()>(charactereCode.code.to_ulong());
            numberOfShifts += charactereCode.len;
        }
        else if (numberOfShifts < 8 && charactereCode.len > (8 - numberOfShifts))
        {
            bitset<32> copyOfByteReaded;
            int shiftsTodo = 0;

            while (charactereCode.len > 0)
            {
                if (charactereCode.len <= (8 - numberOfShifts))
                {
                    byteToWrite <<= charactereCode.len;
                    byteToWrite |= bitset<byteToWrite.size()>(charactereCode.code.to_ulong());
                    numberOfShifts += charactereCode.len;
                    charactereCode.len = 0;
                }
                else
                {

                    shiftsTodo = (charactereCode.len - (8 - numberOfShifts));
                    copyOfByteReaded = charactereCode.code >> shiftsTodo;
                    charactereCode.len = shiftsTodo;
                    byteToWrite <<= (8 - numberOfShifts);
                    byteToWrite |= bitset<byteToWrite.size()>(copyOfByteReaded.to_ulong());
                    (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
                    copyOfByteReaded <<= shiftsTodo;
                    copyOfByteReaded.flip();
                    charactereCode.code &= copyOfByteReaded;
                    byteToWrite.reset();

                    numberOfShifts = 0;
                }
            }
        }

        if (numberOfShifts == 8)
        {
            (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
            numberOfShifts = 0;
            byteToWrite.reset();
        }
    }
    if (numberOfShifts > 0 && numberOfShifts < 8)
    {
        byteToWrite <<= (8 - numberOfShifts);
        (*fileToWrite) << (unsigned char)byteToWrite.to_ullong();
    }

    (*fileToWrite) << std::flush;
}

//===================================================================

int main(int, char *argv[])
{

    string fileName = "1_6_chars.txt";
    int *occurenceVector = new int[256];
    uint16_t N_numberOfLeafs = 0;
    int numberOfBytes = 0;
    string option(argv[1]);
    string inputFileName(argv[2]);
    string outputFileName(argv[3]);

    if (option == "-c")
    {
        ifstream *file = new ifstream(inputFileName.data(), std::ios_base::in | std::ios_base::binary);
        ofstream *outputFile = new ofstream(outputFileName.data(), std::ios_base::out | std::ios_base::binary);

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
            cout << "Arquivo compilado escrito.\n";
            return 0;
        }
        if (N_numberOfLeafs < 2)
        {
            cout << "Escrevendo o arquivo compilado.\n";
            (*outputFile).write((char *)&N_numberOfLeafs, sizeof(uint16_t));
            (*outputFile) << std::flush;
            (*outputFile).write((char *)&numberOfBytes, sizeof(numberOfBytes));
            int i = 0;
            while (occurenceVector[i] == 0)
                ++i;
            (*outputFile) << (unsigned char)i;
            (*outputFile) << std::flush;
            cout << "Arquivo escrito.\n";
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
        cout << "Construindo árvore de huffman...\n";
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
        cout << "Criando tabela de códigos....\n";
        tree.populateSomeTableWithCodes(codeTable);

        file->clear();
        file->seekg(0);

        (*outputFile).write((char *)&N_numberOfLeafs, sizeof(uint16_t));
        cout << "Escrevendo a árvore no arquivo...\n";
        tree.writeTreInAfile(outputFile);

        (*outputFile).write((char *)&numberOfBytes, sizeof(numberOfBytes));
        cout << "Escrevendo os códigos no arquivo de saída...\n";
        writeCompiledCodes(codeTable, file, outputFile);
        cout << "Arquivo escrito...\n";
        (*outputFile).close();
    }
    else if (option == "-d")
    {
        ifstream *saida = new ifstream(inputFileName.data(), std::ios_base::in | std::ios_base::binary);
        ofstream *saidaDescomp = new ofstream(outputFileName.data(), std::ios_base::out | std::ios_base::binary);
        int numberOfDiferentElements = 0;
        saida->read(reinterpret_cast<char *>(&numberOfDiferentElements), sizeof(uint16_t));

        if (saida->eof())
            return 0;

        if (numberOfDiferentElements < 2)
        {

            int numberOfBytesToBeWrite = 0;
            saida->read(reinterpret_cast<char *>(&numberOfBytesToBeWrite), sizeof(int));

            unsigned char charToBeWrite = (char)saida->get();

            while (numberOfBytesToBeWrite > 0)
            {
                (*saidaDescomp) << charToBeWrite;
                --numberOfBytesToBeWrite;
            }
            (*saidaDescomp) << std::flush;
            return 0;
        }

        occurrenceNode *pointerToTheTree = new occurrenceNode[2 * numberOfDiferentElements - 1];
        cout << "Lendo a árvore do arquivo....\n";
        HuffmanTree treeFromTheFile = HuffmanTree(pointerToTheTree, numberOfDiferentElements);

        treeFromTheFile.populateTreeFromAFile(saida);

        saida->read(reinterpret_cast<char *>(&numberOfBytes), sizeof(int));
        cout << "Escrevendo arquivo descompactado....\n";
        treeFromTheFile.writeUnzzipedFileByTheTree(saidaDescomp, saida, numberOfBytes);
        cout << "Arquivo escrito...\n";
    }
    else
    {
        cout << "entrada inválida - Opção não encontrada.\n";
    }

    return 0;
}

/* g++ -Wall -Wextra -std=c++17 -pedantic -o programa main.cpp */
