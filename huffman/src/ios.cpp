#include "ios.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

size_t readFile(const char * file_name, unsigned char* (&buf));
void compress(const char* file_name, const char* data, size_t size, Node* root, unsigned char** chCode, size_t* chCode_len);
void serialize(std::ofstream& out, Node* root);
void deserialize(std::ifstream& in, Node *(&root), size_t& extra_size);


//=============================================================================

size_t readFile(const char * file_name, unsigned char* (&buf))
{
    std::ifstream in(file_name, std::ifstream::in | std::ifstream::binary);

    if (!in)
    {
        std::cout << "Cannot open file!\n";
        exit(-1);
    }

    in.seekg(0, in.end);

    size_t file_size = in.tellg();

    in.seekg(0, in.beg);

    buf = new unsigned char[file_size];

    in.read(reinterpret_cast<char*>(buf),file_size);

    in.close();

    return file_size;
}

//================================================================================


size_t compress(const char* file_name, const unsigned char* data, size_t size, Node* root, unsigned char** chCode, size_t* chCode_len)
{
    std::ofstream out(file_name, std::ofstream::out | std::ofstream::binary);

    if (!out)
    {
        std::cout<< "Cannot open file "<<file_name<<" to write";
        exit(-1) ;
    }

    uint32_t code_len = 0;
    for(size_t i = 0; i < size; ++i)
    {
        code_len += chCode_len[static_cast<size_t>(data[i])];
    }

    out.write(reinterpret_cast<char*>(&code_len), sizeof (code_len));

    unsigned char buf;
    size_t m = 8;

    for(size_t i = 0; i < size; ++i)
    {
        size_t k = static_cast<size_t>(data[i]);

        for(size_t j = 0; j < chCode_len[k]; ++j)
        {
            switch(chCode[k][j])
            {
                case '0':
                {
                    buf &= ~(1 << (m-1));
                    --m;
                    break;
                }

                case '1':
                {
                    buf |= (1 << (m-1));
                    --m;
                    break;
                }

            }


            if (m == 0)
            {
                out.write(reinterpret_cast<const char*>(&buf), 1);
                m = 8;
            }
        }
    }

    if (m < 8)
    {
        out.write(reinterpret_cast<const char*>(&buf), 1);
    }


    serialize(out, root);

    out.close();

    return code_len;
}

//============================================================================

void serialize(std::ofstream& out, Node* root)
{
    if (!root)
    {
        return;
    }

    if (!root->left && !root->right)
    {
        out.write("1", 1);
        out.write(reinterpret_cast<const char*>(&root->data), 1);
    }

    else
    {
        out.write("0", 1);
        serialize(out,root->left);
        serialize(out, root->right);

    }

}

//=============================================================================

void deserialize(std::ifstream& in, Node *(&root), size_t& extra_size)
{

    char val;

    if (!in.read(&val, 1))
    {
        root = newNode('\0', 0);
        return;
    }

    ++extra_size;

    if (val == '0')
    {
        root = newNode('$', 0);
        deserialize(in, root->left, extra_size);
        deserialize(in, root->right, extra_size);
    }

    if (val == '1')
    {
        in.read(&val, 1);
        ++extra_size;
        root = newNode(val, 0);
    }

}
