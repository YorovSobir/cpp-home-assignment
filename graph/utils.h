#ifndef HA1_UTILS_H
#define HA1_UTILS_H

#include <vector>

namespace au {
    namespace utils {

        template <class data_type>
        struct always_true {
            bool operator()(const data_type& data) {
                return true;
            }
        };

        template <typename T>
        struct au_priority_queue
        {
            au_priority_queue(std::vector<T>& array, size_t n)
                    : size(n)
                    , heap(n)
                    , index(std::vector<size_t>(size))
            {
                for (size_t i = 0; i < size; ++i) {
                    heap[i] = std::make_pair(array[i], i);
                    index[i] = i;
                }
            }

            void make_heap() {
                for (size_t i = size / 2; i > 0; --i) {
                    minHeapify(i);
                }
                minHeapify(0);
            }

            int extractMin() {
                if (size == 0) {
                    throw std::runtime_error("heap is empty");
                }
                index[heap[0].second] = size - 1;
                index[heap[size - 1].second] = 0;
                std::swap(heap[0], heap[size - 1]);
                --size;
                minHeapify(0);
                return heap[size].second;
            }

            void decrease_key(size_t v, const T& heap_value) {
                size_t index_in_heap = index[v];
                heap[index_in_heap].first = heap_value;
                while (index_in_heap != 0) {
                    size_t parent = index_in_heap % 2 == 0 ? index_in_heap / 2 - 1 : (index_in_heap - 1) / 2;
                    if (heap[parent].first > heap[index_in_heap].first) {
                        index[heap[parent].second] = index_in_heap;
                        index[heap[index_in_heap].second] = parent;
                        std::swap(heap[parent], heap[index_in_heap]);
                        index_in_heap = parent;
                    } else {
                        break;
                    }
                }
            }

            void minHeapify(size_t idx) {
                size_t smallest = idx;
                size_t left = 2 * idx + 1;
                size_t right = 2 * idx + 2;

                if (left < size &&
                    heap[left].first < heap[smallest].first)
                    smallest = left;

                if (right < size &&
                    heap[right].first < heap[smallest].first)
                    smallest = right;

                if (smallest != idx)
                {
                    index[heap[smallest].second] = idx;
                    index[heap[idx].second] = smallest;
                    std::swap(heap[smallest], heap[idx]);
                    minHeapify(smallest);
                }
            }

            size_t get_size() {
                return size;
            }

        private:
            size_t size;
            std::vector<std::pair<T, size_t>> heap;
            std::vector<size_t> index;
        };
    }
}
#endif //HA1_UTILS_H
