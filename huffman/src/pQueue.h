#ifndef PQUEUE_H
#define PQUEUE_H
#include "tree.h"

struct PriorityQueue
{
    PriorityQueue(size_t capacity): size(0), capacity(capacity), node(new Node*[capacity])
    {}

    ~PriorityQueue()
    {
        delete[] node;
    }

    void insert(Node* node);
    Node* extractMin();
    void minHeapify(size_t idx);
    size_t get_size();

private:
    size_t size;
    size_t capacity;
    Node ** node;

};



#endif // PQUEUE_H
