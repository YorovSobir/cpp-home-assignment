#include "pQueue.h"


size_t PriorityQueue::get_size()
{
    return size;
}

//====================================================================

void PriorityQueue::insert(Node* newnode)
{
    ++size;

    size_t i = size - 1;

    while (i && newnode->freq < node[(i - 1)/2]->freq)
    {
            node[i] = node[(i - 1)/2];
            i = (i - 1)/2;
    }

    node[i] = newnode;
}

//=====================================================================

void swapNode(Node* (&first), Node* (&second))
{
    Node* t = first;
    first = second;
    second = t;
}

//======================================================================

void PriorityQueue::minHeapify(size_t idx)
{
    size_t smallest = idx;
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;

    if (left < size &&
        node[left]->freq < node[smallest]->freq)
      smallest = left;

    if (right < size &&
        node[right]->freq < node[smallest]->freq)
      smallest = right;

    if (smallest != idx)
    {
        swapNode(node[smallest], node[idx]);
        minHeapify(smallest);
    }
}

//======================================================================

Node* PriorityQueue::extractMin()
{
    Node* temp = node[0];
    node[0] = node[size - 1];
    --size;
    minHeapify(0);
    return temp;
}
