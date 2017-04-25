#ifndef HA1_ITERATOR_H
#define HA1_ITERATOR_H


#include <iterator>
#include <tuple>
#include "utils.h"

namespace au {

    template <class GRAPH, bool is_const,
            class filter = utils::always_true<typename GRAPH::vertex_data>>
    struct vertex_it_policy {

    private:
        using iterator = std::conditional_t<is_const,
                typename GRAPH::adjlist_container::const_iterator,
                typename GRAPH::adjlist_container::iterator>;

        using filter_func = std::function<bool(const typename GRAPH::vertex_data&)>;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const typename GRAPH::vertex_data;
        using difference_type = size_t;
        using pointer = value_type*;
        using reference = value_type&;

        vertex_it_policy(iterator it, iterator end, filter_func func = filter())
                : it_(it)
                , end_(end)
                , func_(func)
        {
            advance();
        }

        iterator get_it() {
            return it_;
        }

        iterator get_end() {
            return end_;
        }

        vertex_it_policy() = default;

        template <class iterator_t>
        vertex_it_policy(const iterator_t& other,
                         std::enable_if_t<!iterator_t::is_const_iterator>* = nullptr) {
            it_ = other.it_;
            end_ = other.end_;
            func_ = other.func_;
        };

        bool operator==(const vertex_it_policy& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const vertex_it_policy& other) const {
            return !(*this == other);
        }

        reference operator*() const {
            return (it_->first);
        }

        pointer operator->() const {
            return &(it_->first);
        }

        vertex_it_policy& operator++() {
            ++it_;
            advance();
            return *this;
        }

        vertex_it_policy operator++(int) {
            vertex_it_policy tmp(*this);
            ++(*this);
            return tmp;
        }

        friend class vertex_it_policy<GRAPH, true, filter>;

    private:

        void advance() {
            while (it_ != end_ && !func_(it_->first)) {
                ++it_;
            }
        }

        iterator it_;
        iterator end_;
        filter_func func_;
        static bool const is_const_iterator = is_const;
    };

    template <  class GRAPH, bool is_const,
                class filter_edge_t = utils::always_true<typename GRAPH::edge_data>,
                class filter_vertex_t = utils::always_true<typename GRAPH::vertex_data>>
    struct edge_it_policy {

    private:

        using iterator = std::conditional_t<is_const,
                typename GRAPH::adjlist_container::mapped_type::const_iterator,
                typename GRAPH::adjlist_container::mapped_type::iterator>;

        using filter_edge_f = std::function<bool(const typename GRAPH::edge_data&)>;
        using filter_vertex_f = std::function<bool(const typename GRAPH::vertex_data&)>;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::conditional_t<is_const, const typename GRAPH::edge_data,
                                              typename GRAPH::edge_data>;
        using difference_type = size_t;
        using pointer = value_type*;
        using reference = value_type&;

        edge_it_policy(iterator it, iterator end,
                       filter_edge_f func_edge = filter_edge_t(),
                       filter_vertex_f func_vertex = filter_vertex_t())
                : it_(it)
                , end_(end)
                , func_edge(func_edge)
                , func_vertex(func_vertex)
        {
            advance();
        }

        iterator get_it() {
            return it_;
        }

        iterator get_end() {
            return end_;
        }

        edge_it_policy() = default;

        template<class iterator_t>
        edge_it_policy(const iterator_t& other,
                       std::enable_if_t<!iterator_t::is_const_iterator>* = nullptr) {
            it_ = other.it_;
            end_ = other.end_;
            func_edge = other.func_edge;
            func_vertex = other.func_vertex;
        }

        friend class edge_it_policy<GRAPH, true>;

        typename GRAPH::vertex_iterator from() {
            return std::get<0>(*it_);
        };

        typename GRAPH::vertex_iterator to() {
            return std::get<2>(*it_);
        };

        bool operator==(const edge_it_policy& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const edge_it_policy& other) const {
            return !(*this == other);
        }

        reference operator*() {
            return std::get<1>(*it_);
        }

        pointer operator->() {
            return &(std::get<1>(*it_));
        }

        edge_it_policy& operator++() {
            ++it_;
            advance();
            return *this;
        }

        edge_it_policy operator++(int) {
            edge_it_policy tmp(*this);
            ++(*this);
            return tmp;
        }

    private:

        void advance() {
            while (it_ != end_ &&
                    (
                     !func_edge(std::get<1>(*it_)) ||
                     !func_vertex(*(std::get<0>(*it_))) ||
                     !func_vertex(*(std::get<2>(*it_)))
                    )
                  ) {

                ++it_;
            }
        }

        iterator it_;
        iterator end_;
        filter_edge_f func_edge;
        filter_vertex_f func_vertex;
        static bool const is_const_iterator = is_const;
    };

}

#endif //HA1_ITERATOR_H
