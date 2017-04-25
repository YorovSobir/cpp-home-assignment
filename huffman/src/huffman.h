#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>

void encoding(const char* input, const char* output, size_t& input_size, size_t& output_size, size_t& extra_size);

void decoding(const char* input, const char* output, size_t& input_size, size_t& output_size, size_t& extra_size);

#endif // HUFFMAN_H
