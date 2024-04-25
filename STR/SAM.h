/*
最后修改:
20240108
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SAM__
#define __OY_SAM__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace SAM {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void set_child(size_type index, size_type child) { m_child[index] = child; }
            size_type get_child(size_type index) const { return m_child[index]; }
            void copy_children(const StaticChildGetter<ChildCount> &rhs) { std::copy_n(rhs.m_child, ChildCount, m_child); }
        };
        template <typename ChildGetter>
        struct Automaton {
            struct node : ChildGetter {
                size_type m_length, m_fail;
            };
            std::vector<node> m_data;
            std::vector<size_type> m_node, m_queue;
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            void _init() { m_data.resize(1), m_data[0].m_fail = -1, m_node.push_back(0); }
            Automaton() { _init(); }
            template <typename InitMapping>
            Automaton(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Automaton(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                reserve(length);
                for (size_type i = 0; i != length; i++) push_back(mapping(i));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type length) { m_data.clear(), m_data.reserve(length * 2 + 1), m_node.clear(), m_node.reserve(length + 1), m_queue.clear(), _init(); }
            void clear() { m_data.clear(), m_node.clear(), m_queue.clear(), _init(); }
            size_type size() const { return m_node.size() - 1; }
            bool empty() const { return m_node.size() == 1; }
            template <typename ValueType>
            void push_back(const ValueType &elem) {
                size_type last = m_node.back(), end = _newnode(), child;
                m_node.push_back(end);
                m_data[end].m_length = m_data[last].m_length + 1;
                while (~last) {
                    child = m_data[last].get_child(elem);
                    if (child) break;
                    m_data[last].set_child(elem, end), last = m_data[last].m_fail;
                }
                if (~last)
                    if (m_data[child].m_length == m_data[last].m_length + 1)
                        m_data[end].m_fail = child;
                    else {
                        size_type q = child, newq = _newnode();
                        m_data[newq].m_length = m_data[last].m_length + 1;
                        m_data[newq].m_fail = m_data[q].m_fail;
                        m_data[newq].copy_children(m_data[q]);
                        m_data[q].m_fail = m_data[end].m_fail = newq;
                        do {
                            child = m_data[last].get_child(elem);
                            if (child != q) break;
                            m_data[last].set_child(elem, newq);
                            last = m_data[last].m_fail;
                        } while (~last);
                    }
            }
            void prepare() {
                std::vector<size_type> cnt(m_node.size());
                for (auto &e : m_data) cnt[e.m_length]++;
                std::partial_sum(cnt.begin(), cnt.end(), cnt.begin());
                m_queue.resize(m_data.size());
                for (size_type i = 0; i != m_data.size(); i++) m_queue[--cnt[m_data[i].m_length]] = i;
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            size_type query_fail(size_type node_index) const { return m_data[node_index].m_fail; }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
            const node *get_fail_node(size_type node_index) const { return get_node(query_fail(node_index)); }
            node *get_fail_node(size_type node_index) { return get_node(query_fail(node_index)); }
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (size_type a : m_queue) call(a);
            }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_queue.size() - 1; ~i; i--) call(m_queue[i]);
            }
        };
    }
    template <SAM::size_type ChildCount = 26>
    using StaticSAM_string = SAM::Automaton<SAM::StaticChildGetter<ChildCount>>;
}

#endif