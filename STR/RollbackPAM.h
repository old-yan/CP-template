/*
最后修改:
20230107
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKPAM__
#define __OY_ROLLBACKPAM__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RollbackPAM {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void add_child(size_type index, size_type child) { m_child[index] = child; }
            void remove_child(size_type index) { m_child[index] = 0; }
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename ChildGetter, typename Sequence, size_type DepthBitCount>
        struct Automaton {
            using value_type = typename Sequence::value_type;
            struct node : ChildGetter {
                size_type m_parent[DepthBitCount + 1], m_length, m_fail, m_fa, m_size_when_appear;
                void add_parent(size_type level_bit, size_type parent) { m_parent[level_bit] = parent; }
                size_type get_parent(size_type level_bit) const { return m_parent[level_bit]; }
            };
            struct series {
                size_type m_longest, m_shortest, m_delta;
            };
            Sequence m_text;
            std::vector<node> m_data;
            std::vector<size_type> m_node;
            static constexpr size_type odd_root() { return 0; }
            static constexpr size_type even_root() { return 1; }
            static constexpr bool is_node(size_type node_index) { return node_index >> 1; }
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            void _init() { m_data.resize(2), m_data[0].m_length = m_data[0].m_fail = -1, m_data[1].m_length = m_data[1].m_fail = 0, m_node.push_back(0); }
            size_type _jump(size_type node_index, size_type index, const value_type &elem) const {
                if (!~node_index) return -1;
                size_type len = m_data[node_index].m_length, pre_len = std::numeric_limits<size_type>::max() / 2;
                if (len == index) node_index = m_data[node_index].m_fail, pre_len = len, len = m_data[node_index].m_length;
                while (m_text[index - len - 1] != elem)
                    if (len * 3 > pre_len * 2) {
                        size_type d = pre_len - len, q = len / d, r = len - q * d;
                        pre_len = len, len = d + r, q--;
                        do {
                            size_type i = std::countr_zero(q);
                            node_index = m_data[node_index].get_parent(i), q -= q & -q;
                        } while (q);
                    } else
                        node_index = m_data[node_index].m_fail, pre_len = len, len = m_data[node_index].m_length;
                return node_index;
            }
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
            void reserve(size_type length) { m_text.clear(), m_text.reserve(length), m_data.clear(), m_data.reserve(length + 2), m_node.clear(), m_node.reserve(length + 1), _init(); }
            void clear() { m_text.clear(), m_data.clear(), m_node.clear(), _init(); }
            size_type size() const { return m_text.size(); }
            bool empty() const { return m_text.empty(); }
            void push_back(const value_type &elem) {
                size_type index = size();
                m_text.push_back(elem);
                size_type last = _jump(m_node.back(), index, elem), child = m_data[last].get_child(elem);
                if (!child) {
                    child = _newnode();
                    m_data[last].add_child(elem, child);
                    m_data[child].m_fa = last;
                    m_data[child].m_length = m_data[last].m_length + 2;
                    size_type fail = _jump(m_data[last].m_fail, index, elem);
                    if (~fail) {
                        size_type x = m_data[fail].get_child(elem), l = 0;
                        m_data[child].add_parent(0, m_data[child].m_fail = x);
                        while (size_type y = m_data[x].get_parent(l)) m_data[child].add_parent(++l, x = y);
                    } else
                        m_data[child].add_parent(0, m_data[child].m_fail = 1);
                    m_data[child].m_size_when_appear = size();
                }
                m_node.push_back(child);
            }
            void pop_back() {
                if (m_data.back().m_size_when_appear == size()) {
                    const auto &elem = m_text.back();
                    m_data[m_data[m_node.back()].m_fa].remove_child(elem);
                    m_data.pop_back(), m_text.pop_back(), m_node.pop_back();
                } else
                    m_text.pop_back(), m_node.pop_back();
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            size_type query_fail(size_type node_index) const { return m_data[node_index].m_fail; }
            size_type query_ancestor(size_type node_index) const {
                size_type cur_len = m_data[node_index].m_length, nxt = m_data[node_index].m_fail, nxt_len = m_data[nxt].m_length, d = cur_len - nxt_len;
                if (d * 2 <= cur_len) {
                    size_type q = cur_len / d - 1, shortest = cur_len - q * d;
                    do {
                        size_type i = std::countr_zero(q);
                        node_index = m_data[node_index].get_parent(i), q -= q & -q;
                    } while (q);
                    if (shortest > d && m_data[m_data[node_index].m_fail].m_length == shortest - d)
                        return m_data[node_index].m_fail;
                    else
                        return node_index;
                } else
                    return nxt;
            }
            size_type query_fail_to_half(size_type node_index) const {
                if (get_fail_node(node_index)->m_length * 2 <= get_node(node_index)->m_length) return query_fail(node_index);
                size_type i = DepthBitCount - 1, len = m_data[node_index].m_length, ans = 1;
                while (!m_data[node_index].get_parent(i)) i--;
                do {
                    size_type p = m_data[node_index].get_parent(i);
                    if (m_data[p].m_length * 2 <= len)
                        ans = p;
                    else
                        node_index = p;
                } while (~--i);
                return ans;
            }
            bool query(size_type left, size_type right) const {
                size_type x = query_node_index(right), len = right - left + 1;
                if (m_data[x].m_length <= len) return m_data[x].m_length == len;
                size_type i = DepthBitCount - 1, ans = 1;
                while (!m_data[x].get_parent(i)) i--;
                do {
                    size_type p = m_data[x].get_parent(i);
                    if (m_data[p].m_length <= len)
                        ans = p;
                    else
                        x = p;
                } while (~--i);
                return m_data[ans].m_length == len;
            }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
            const node *get_fail_node(size_type node_index) const { return get_node(query_fail(node_index)); }
            node *get_fail_node(size_type node_index) { return get_node(query_fail(node_index)); }
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (size_type i = 2; i != m_data.size(); i++) call(i);
            }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_data.size() - 1; is_node(i); i--) call(i);
            }
            template <typename Callback>
            void do_for_each_series(size_type i, Callback &&call) {
                size_type cur = m_node[i + 1], cur_len = m_data[cur].m_length;
                while (cur_len > 1) {
                    size_type nxt = m_data[cur].m_fail, nxt_len = m_data[nxt].m_length, d = cur_len - nxt_len;
                    if (d * 2 <= cur_len) {
                        size_type q = cur_len / d - 1, shortest = cur_len - q * d;
                        do {
                            size_type i = std::countr_zero(q);
                            cur = m_data[cur].get_parent(i), q -= q & -q;
                        } while (q);
                        if (shortest > d && m_data[m_data[cur].m_fail].m_length == shortest - d) {
                            call(series{cur_len, shortest - d, d});
                            cur = m_data[cur].m_fail, cur_len = shortest - d;
                        } else {
                            call(series{cur_len, shortest, d});
                            cur_len = shortest;
                        }
                    } else {
                        call(series{cur_len, nxt_len, d});
                        cur = nxt, cur_len = nxt_len;
                    }
                }
            }
            template <typename Callback>
            void do_for_each_distinct_node(size_type init_node_index, Callback &&call) const {
                size_type node_index = init_node_index, len = m_data[node_index].m_length, pre_len = std::numeric_limits<size_type>::max() / 2;
                do {
                    call(node_index);
                    if (len * 3 > pre_len * 2) {
                        size_type d = pre_len - len, q = len / d, r = len - q * d;
                        pre_len = len, len = d + r, q--;
                        do {
                            size_type i = std::countr_zero(q);
                            node_index = m_data[node_index].get_parent(i), q -= q & -q;
                        } while (q);
                    } else
                        node_index = m_data[node_index].m_fail, pre_len = len, len = m_data[node_index].m_length;
                } while (is_node(node_index));
            }
            template <typename Callback>
            void do_for_each_node(size_type init_node_index, Callback &&call) const {
                size_type node_index = init_node_index;
                do call(node_index);
                while (is_node(node_index = m_data[node_index].m_fail));
            }
        };
    }
    template <RollbackPAM::size_type ChildCount = 26, RollbackPAM::size_type DepthBitCount = 20>
    using StaticRollbackPAM_string = RollbackPAM::Automaton<RollbackPAM::StaticChildGetter<ChildCount>, std::string, DepthBitCount>;
}

#endif