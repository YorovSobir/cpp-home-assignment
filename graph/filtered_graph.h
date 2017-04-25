#ifndef HA1_FILTERED_GRAPH_H
#define HA1_FILTERED_GRAPH_H

#include "graph_iterator.h"
namespace au {

    template<class graph, class vertex_filter, class edge_filter>
    struct filtered_graph {

        using vertex_data = typename graph::vertex_data;
        using edge_data = typename graph::edge_data;

        using vertex_iterator = vertex_it_policy<graph, true, vertex_filter>;
        using edge_iterator = edge_it_policy<graph, true, edge_filter, vertex_filter>;
        using vertex_const_iterator = vertex_it_policy<graph, true, vertex_filter>;
        using edge_const_iterator = edge_it_policy<graph, true, edge_filter, vertex_filter>;

        filtered_graph(graph const& g, const vertex_filter& vfil,
                        const edge_filter& efil)
                : g_(g)
                , vfil_(vfil)
                , efil_(efil)
        {}

        vertex_iterator find_vertex(typename graph::vertex_data const& data) const {
            for (auto vnode = vertex_begin(); vnode != vertex_end(); ++vnode) {
                if (*vnode == data) {
                    return vnode;
                }
            }
            return vertex_end();
        }

        edge_iterator find_edge(vertex_iterator from, vertex_iterator to) const {
            for (auto lnode = edge_begin(from); lnode != edge_end(from); ++lnode) {
                if (*(lnode.from()) == *from &&
                        *(lnode.to()) == *to) {
                    return lnode;
                }
            }
            return edge_end(from);
        }

        vertex_iterator vertex_begin() const {
            auto it = g_.vertex_begin();
            return vertex_iterator(it.get_it(), it.get_end(), vfil_);
        }

        vertex_iterator vertex_end() const {
            auto it = g_.vertex_end();
            return vertex_iterator(it.get_it(), it.get_end(), vfil_);
        }

        edge_iterator edge_begin(vertex_iterator from) const {
            auto it = g_.edge_begin(typename graph::vertex_const_iterator(from.get_it(), from.get_end()));
            edge_iterator temp(it.get_it(), it.get_end(), efil_, vfil_);
            return temp;
        }

        edge_iterator edge_end(vertex_iterator from) const {
            auto it = g_.edge_end(typename graph::vertex_const_iterator(from.get_it(), from.get_end()));
            edge_iterator temp(it.get_it(), it.get_end(), efil_, vfil_);
            return temp;
        }

    private:
        graph g_;
        vertex_filter vfil_;
        edge_filter efil_;
    };

}
#endif //HA1_FILTERED_GRAPH_H
