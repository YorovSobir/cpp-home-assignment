#ifndef IOS_H
#define IOS_H

#include "tree.h"
#include <fstream>

size_t readFile(const char * file_name, unsigned char* (&buf));

size_t compress(const char* file_name, const unsigned char* data, size_t size, Node* root, unsigned char** chCode, size_t* chCode_len);

void deserialize(std::ifstream& in, Node *(&root), size_t& extra_size);

#endif // IOS_H
