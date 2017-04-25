#include <iostream>
#include "huffman.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

bool get_option(size_t args, char** argv, char* (&input), char* (&output));

int main(int args, char** argv)
{
    char* input;
    char* output;
    size_t input_size;
    size_t output_size;
    size_t extra_size;

    if (get_option(args, argv, input, output))
    {
      encoding(input, output, input_size, output_size, extra_size);
      std::cout << input_size << "\n" << output_size << "\n" << extra_size << std::endl;
    }

    else
    {
       decoding(input, output, input_size, output_size, extra_size);
       std::cout << input_size << "\n" << output_size << "\n" << extra_size << std::endl;
    }

    return 0;

}


bool get_option(size_t argc, char** argv, char* (&input), char* (&output))
{
    bool operation;
    bool right_option = 0;

    for(size_t i = 1; i < argc;)
    {
        if (!strncmp(argv[i], "-c", 2))
        {
            operation = 1;
            right_option = !right_option;
            ++i;
            continue;
        }

        if (!strncmp(argv[i], "-u", 2))
        {
            operation = 0;
            right_option = !right_option;
            ++i;
            continue;
        }

        if (!strncmp(argv[i], "-f", 2) || !strncmp(argv[i], "--file", 6))
        {
            if (i + 1 != argc)
            {
                input = argv[i + 1];
            }
            else
            {
                std::cout<<"Wrong argument\n";
                exit(-1);
            }

            i += 2;
            continue;
        }

        if (!strncmp(argv[i], "-o", 2) || !strncmp(argv[i], "--output", 8))
        {
            if (i + 1 != argc)
            {
                output = argv[i + 1];
            }

            else
            {
                std::cout<<"Wrong argument\n";
                exit(-1);
            }

            i += 2;
            continue;
        }

        std::cout<<"Wrong argument"<<std::endl;
        exit(-1);

    }

    if (!right_option)
    {
        std::cout<<"Wrong argument"<<std::endl;
        exit(-1);
    }

    return operation;

}
