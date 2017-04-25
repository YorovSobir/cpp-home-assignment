#include "huffman.h"
#include "tree.h"
#include "pQueue.h"
#include "ios.h"
#include <cstring>
#include <stdlib.h>
#include <stdint.h>

size_t getUniqCharAndFreq(unsigned char* data, size_t size, unsigned char* (&uniqChar), size_t* (&uniqCharFreq));
Node* buildHuffmanTree(unsigned char* uniqChar, size_t* uniqCharFreq, size_t uniqCharCount, size_t& extra_size);
PriorityQueue* createPriorityQueue(unsigned char* data, size_t* priority, size_t size);
void getTableCode(Node* root, unsigned char** chCode, size_t* chCode_len, size_t top);
bool isLeaf(Node* node);
void destroyTree(Node* root);

//=============================================================================

void encoding(const char* input, const char* output, size_t& input_size, size_t& output_size, size_t& extra_size)
{
   unsigned char* data;

   input_size = readFile(input, data);

   if (input_size > 0)
   {
       unsigned char* uniqChar;
       size_t* uniqCharFreq;

       size_t uniqCharCount = getUniqCharAndFreq(data, input_size, uniqChar, uniqCharFreq);

       extra_size = 0;
       Node* root = buildHuffmanTree(uniqChar, uniqCharFreq, uniqCharCount, extra_size);
       Node* root1 = root;

       //extra memory to store tree
       extra_size += 2 * uniqCharCount; //перед каждым символом при сериализации я записываю 1 байт дополнительно
       extra_size += 4; //в начало последовательности кодов записываю длину последовательности (4 байта)

       unsigned char** chCode;
       chCode = new unsigned char*[257];

       for(size_t i = 0; i < 257; ++i)
       {
           chCode[i] = new unsigned char[256];

       }

       size_t* chCode_len = new size_t[256];

       memset(chCode_len, 0, sizeof(chCode_len)*256);


       size_t top = 0;

       getTableCode(root, chCode, chCode_len, top);

       root = root1;

       output_size = compress(output, data, input_size, root, chCode, chCode_len);

       size_t j = 0;
       while ((output_size + j) % 8 != 0)
       {
           ++j;
       }

       output_size = extra_size + (output_size + j)/8;

       delete[] data;
       delete[] uniqCharFreq;
       delete[] uniqChar;
       for(size_t i = 0; i < 257; ++i)
       {
           delete[] chCode[i];
       }
       delete[] chCode;
       delete[] chCode_len;

       destroyTree(root1);
   }
   else
   {
       std::ofstream out(output, std::ofstream::out | std::ofstream::binary);
       input_size = 0;
       output_size = 0;
       extra_size = 0;
       delete[] data;
       out.close();
   }


}

//=============================================================================

void decoding(const char* input, const char* output, size_t& input_size, size_t& output_size, size_t& extra_size)
{
    std::ifstream in(input, std::ifstream::in | std::ifstream::binary);

    in.seekg(0, in.end);

    size_t fsize = in.tellg();

    in.seekg(0, in.beg);

    if (fsize > 0)
    {
        uint32_t size;
        in.read(reinterpret_cast<char*>(&size),sizeof (size));

        size_t i = 0;
        while ((size + i) % 8 != 0)
        {
            ++i;
        }

        size_t size_in_byte = (size + i)/8;

        unsigned char* data = new unsigned char[size_in_byte];

        in.read(reinterpret_cast<char*>(data), size_in_byte);

        Node* root = nullptr;
        extra_size = 0;
        deserialize(in, root, extra_size);

        in.close();

        extra_size += 4; //4 байта в начале файла - длина последовательности кода

        input_size = size_in_byte + extra_size;

        std::ofstream out(output, std::ofstream::out);

        Node* r = root;

        size_t m = 0;
        i = 0;
        size_t k = 8;
        output_size = 0;

        while (i < size)
        {

            if ((r->data != '$') || ((r->right == 0) && (r->left == 0)))
            {
                out.write(reinterpret_cast<const char*>(&r->data),1);
                ++output_size;
            }

            else
            {
                if (data[m] & (1 << (k - 1)))
                {
                    r = r->right;
                }
                else
                {
                    r = r->left;
                }

                if ((r->data != '$') || ((r->right == 0) && (r->left == 0)))
                {
                    out.write(reinterpret_cast<const char*>(&r->data),1);
                    ++output_size;
                    r = root;
                }

            }


            ++i;
            --k;

            if (k == 0)
            {
                k = 8;
                ++m;
            }
        }

        out.close();

        delete[] data;


        destroyTree(root);

    }

    else
    {
        std::ofstream out(output, std::ofstream::out | std::ofstream::binary);
        input_size = 0;
        output_size = 0;
        extra_size = 0;
        out.close();
        in.close();
    }


}

//========================================================================

size_t getUniqCharAndFreq(unsigned char* data, size_t size, unsigned char* (&uniqChar), size_t* (&uniqCharFreq))
{
    uniqChar = new unsigned char[256];
    uniqCharFreq = new size_t[256];

    memset(uniqCharFreq, 0, sizeof(uniqCharFreq)*256);
    size_t uniqCharCount = 0;


    for(size_t i = 0; i < size; ++i)
    {

        size_t temp = static_cast<size_t>(data[i]);

        if (!uniqCharFreq[temp])
        {
            uniqChar[uniqCharCount] = data[i];
            ++uniqCharCount;
        }

        ++uniqCharFreq[temp];
    }

    return uniqCharCount;

}

//===============================================================================================

Node* buildHuffmanTree(unsigned char* uniqChar, size_t* uniqCharFreq, size_t uniqCharCount, size_t& extra_size)
{

    PriorityQueue* pqueue = createPriorityQueue(uniqChar, uniqCharFreq, uniqCharCount);

    Node* left;
    Node* right;
    Node* top;

    while (!(pqueue->get_size() <= 1))
    {
        left = pqueue->extractMin();
        right = pqueue->extractMin();

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        pqueue->insert(top);
        ++extra_size;
    }

    top = pqueue->extractMin();
    delete pqueue;

    return top;
}

//=========================================================================

PriorityQueue* createPriorityQueue(unsigned char* data, size_t* priority, size_t size)
{
    size_t capacity = 2*size;

    PriorityQueue *pqueue = new PriorityQueue(capacity);

    for(size_t i = 0; i < size; ++i)
    {
        Node* temp = newNode(data[i],priority[static_cast<size_t>(data[i])]);

        pqueue->insert(temp);
    }

    return pqueue;

}

//===========================================================================

bool isLeaf(Node* node)
{
    return !node->left && !node->right;
}

//===========================================================================

void getTableCode(Node* root, unsigned char** chCode, size_t* chCode_len, size_t top)
{

    if (root->left)
    {
        chCode[256][top] = '0';
        getTableCode(root->left, chCode, chCode_len, top + 1);
    }


    if (root->right)
    {
        chCode[256][top] = '1';
        getTableCode(root->right, chCode, chCode_len, top + 1);
    }


    if (isLeaf(root))
    {
        if (top == 0)
        {
            chCode[256][top] = root->data;
            ++top;

        }

        size_t k = static_cast<size_t>(root->data);

        for(size_t i = 0; i < top; ++i)
        {
            chCode[k][i] = chCode[256][i];
        }

        chCode_len[k] = top;

    }

}

//============================================================================

void destroyTree(Node* root)
{

    if (isLeaf(root))
    {
        delete root;
    }

    else
    {
        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
    }

}

//==============================================================================
