#ifndef TREE_H
#define TREE_H
#include <iostream>

struct Node
{
    unsigned char data;
    size_t freq;
    Node * left;
    Node * right;
};

static inline Node* newNode(unsigned char data, size_t freq)
{
    Node* temp = new Node;
    temp->data = data;
    temp->freq = freq;
    temp->left = 0;
    temp->right = 0;

    return temp;
}

#endif // TREE_H
