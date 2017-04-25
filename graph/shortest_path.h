#ifndef HA1_SHORTEST_PATH_H
#define HA1_SHORTEST_PATH_H

#include <unordered_map>
#include "graph.h"
#include <limits>
#include <queue>
#include <stack>

namespace au {

    template<class graph, class edge_len, class path_visitor>
    bool find_shortest_path(graph const& g_,
                            typename graph::vertex_const_iterator from,
                            typename graph::vertex_const_iterator to,
                            edge_len && len_functor,
                            path_visitor&& visitor) {

        if (from == to) {
            return true;
        }
        if (from == g_.vertex_end() || to == g_.vertex_end()) {
            return false;
        }

        std::unordered_map<typename graph::vertex_data, size_t> vertex_to_ind;
        std::vector<typename graph::vertex_const_iterator> ind_to_vertex;
        std::vector<double> distance;
        size_t i = 0;
        for (auto it = g_.vertex_begin(); it != g_.vertex_end(); ++it) {
            distance.push_back(std::numeric_limits<double>::max());
            ind_to_vertex.push_back(it);
            vertex_to_ind[*it] = i;
            ++i;
        }
        distance[vertex_to_ind[*from]] = 0.;
        std::vector<std::pair<size_t, typename graph::edge_const_iterator>> path(i);

        utils::au_priority_queue<double> pq(distance, i);
        pq.make_heap();

        while(pq.get_size() > 0) {
            auto v = pq.extractMin();
            for (auto it = g_.edge_begin(ind_to_vertex[v]);
                 it != g_.edge_end(ind_to_vertex[v]); ++it) {
                auto to_ind = vertex_to_ind[*it.to()];
                if (distance[to_ind] > distance[v] + len_functor(*it)) {
                    path[to_ind] = std::make_pair(v, it);
                    distance[to_ind] = distance[v] + len_functor(*it);
                    pq.decrease_key(to_ind, distance[to_ind]);
                }
            }
        }

        if (distance[vertex_to_ind[*to]] < std::numeric_limits<double>::max()) {
            std::stack<typename graph::edge_const_iterator> st;
            auto it = path[vertex_to_ind[*to]];
            while (it.first != vertex_to_ind[*from]) {
                st.push(it.second);
                it = path[it.first];
            }
            st.push(it.second);
            while (!st.empty()) {
                visitor(st.top());
                st.pop();
            }
            return true;
        } else {
            return false;
        }
    };
}
#endif //HA1_SHORTEST_PATH_H
