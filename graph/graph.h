#ifndef HA1_GRAPH_H
#define HA1_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory>
#include <algorithm>
#include "graph_iterator.h"

namespace au {

    template<class vertex_data_t, class edge_data_t>
    struct graph {

        using vertex_data = vertex_data_t;
        using edge_data = edge_data_t;

        using vertex_iterator = au::vertex_it_policy<graph, false>;
        using vertex_const_iterator = au::vertex_it_policy<graph, true>;
        using edge_iterator = au::edge_it_policy<graph, false>;
        using edge_const_iterator = au::edge_it_policy<graph, true>;

        using adjlist_container = std::unordered_map<vertex_data_t,
                std::list<std::tuple<vertex_iterator, edge_data_t, vertex_iterator>>>;

        graph()
            : adjlist()
        {}

        graph(const graph& other)
            : adjlist(other.adjlist)
        {}

        graph(graph&& other)
            : adjlist(std::move(other.adjlist))
        {}

        graph& operator=(const graph& other) {
            adjlist = other.adjlist;
            return *this;
        }

        graph& operator=(graph&& other) {
            std::swap(adjlist, other.adjlist);
            return *this;
        }

        vertex_iterator add_vertex(const vertex_data& data) {
            auto it = adjlist.emplace(data, std::list<std::tuple<
                    vertex_iterator, edge_data, vertex_iterator>>());
            return vertex_iterator(it.first, adjlist.end());
        }

        edge_iterator add_edge( vertex_iterator const &from,
                                vertex_iterator const &to,
                                const edge_data& data) {

            auto it = adjlist[*from].emplace(adjlist[*from].end(),
                                                 std::make_tuple(from, data, to));
            return edge_iterator(it, adjlist[*from].end());
        }

        void remove_vertex(vertex_iterator it) {
            auto predicate = [&it](const auto& other) {
                return it == std::get<0>(other) || it == std::get<2>(other);
            };
            for (auto& node : adjlist) {
                node.second.remove_if(predicate);
            }
            adjlist.erase(*it);
        }

        void remove_edge(edge_iterator it) {
            auto to = it.to();
            auto from = it.from();
            auto data = *it;
            auto predicate = [&from, &data, &to](const auto& tuple_) {
                return from == std::get<0>(tuple_) &&
                       data == std::get<1>(tuple_) &&
                       to == std::get<2>(tuple_);
            };
            adjlist[*(it.from())].remove_if(predicate);

        }

        edge_iterator find_edge(vertex_iterator from, vertex_iterator to) {

            auto predicate = [&from, &to](const auto& other) {
                return from == std::get<0>(other) && to == std::get<2>(other);
            };
            return edge_iterator(std::find_if(adjlist[*from].begin(),
                                              adjlist[*from].end(), predicate),
                                 adjlist[*from].end());
        }

        edge_const_iterator find_edge(vertex_iterator from, vertex_iterator to) const {
            auto predicate = [&from, &to](const auto& other) {
                return from == std::get<0>(other) && to == std::get<2>(other);
            };
            return edge_const_iterator(std::find_if(adjlist[*from].begin(),
                                                    adjlist[*from].end(), predicate),
                                       adjlist[*from].end());
        }

        vertex_iterator find_vertex(vertex_data_t const & data) {
            return vertex_iterator(adjlist.find(data), adjlist.end());
        }

        vertex_const_iterator find_vertex(vertex_data_t const & data) const {
            return vertex_const_iterator(adjlist.find(data), adjlist.end());
        }


        vertex_iterator vertex_begin() {
            return vertex_iterator(adjlist.begin(), adjlist.end());
        }

        vertex_const_iterator vertex_begin() const {
            return vertex_const_iterator(adjlist.begin(), adjlist.end());
        }

        vertex_iterator vertex_end() {
            return vertex_iterator(adjlist.end(), adjlist.end());
        }

        vertex_const_iterator vertex_end() const {
            return vertex_const_iterator(adjlist.end(), adjlist.end());
        }

        edge_iterator edge_begin(vertex_iterator from) {
            return edge_iterator(adjlist[*from].begin(), adjlist[*from].end());
        }

        edge_const_iterator edge_begin(vertex_const_iterator from) const {
            return edge_const_iterator(adjlist.at(*from).begin(),
                                       adjlist.at(*from).end());
        }

        edge_iterator edge_end(vertex_iterator from) {
            return edge_iterator(adjlist[*from].end(), adjlist[*from].end());
        }

        edge_const_iterator edge_end(vertex_const_iterator from) const {
            return edge_const_iterator(adjlist.at(*from).end(),
                                       adjlist.at(*from).end());
        }

    private:
        std::unordered_map<vertex_data_t, std::list<std::tuple<vertex_iterator, edge_data_t, vertex_iterator>>> adjlist;
    };
}
#endif //HA1_GRAPH_H
