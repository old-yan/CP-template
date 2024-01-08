/*
最后修改:
20240108
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BISUFFIXTREE__
#define __OY_BISUFFIXTREE__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace BISUFTREE {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void set_child(size_type index, size_type child) { m_child[index] = child; }
            size_type get_child(size_type index) const { return m_child[index]; }
            void copy_children(const StaticChildGetter<ChildCount> &rhs) { std::copy_n(rhs.m_child, ChildCount, m_child); }
        };
        template <typename Tp, typename ChildGetter, size_type MAX_LEN>
        struct Tree {
            static constexpr size_type inf = std::numeric_limits<size_type>::max() / 2;
            struct node : ChildGetter {
                size_type m_length, m_parent, m_fail;
                Tp *m_ptr;
                ChildGetter m_trans;
            };
            std::vector<node> m_data;
            Tp m_seq[MAX_LEN * 2], *m_first, *m_last;
            size_type m_now, m_remain, m_lst1, m_lst2, m_leaf;
            uint64_t m_unique_cnt;
            size_type _newnode(size_type length, Tp *ptr) {
                m_data.push_back({});
                m_data.back().m_length = length, m_data.back().m_ptr = ptr;
                return m_data.size() - 1;
            }
            void _link(size_type child, size_type parent) {
                m_data[child].m_parent = parent;
                m_data[parent].set_child(m_data[child].m_ptr[m_data[parent].m_length], child);
            }
            void _init() {
                m_now = m_remain = m_lst1 = m_lst2 = m_leaf = m_unique_cnt = 0;
                m_first = m_last = m_seq + MAX_LEN;
                m_data.clear(), m_data.resize(1), m_data[0].m_fail = m_data[0].m_parent = -1;
            }
            Tree() { _init(); }
            void reserve(size_type length) { m_data.clear(), m_data.reserve(length * 2 + 1), _init(); }
            void clear() { m_data.clear(), _init(); }
            size_type size() const { return m_last - m_first; }
            size_type leaf_count() const { return m_leaf; }
            uint64_t unique_count() const { return m_unique_cnt; }
            bool empty() const { return m_first == m_last; }
            void push_back(const Tp &elem) {
                *m_last = elem, m_remain++;
                size_type lst = 0;
                while (m_remain) {
                    if (m_remain <= m_data[m_now].m_length ? m_data[m_now].m_ptr[m_remain - 1] == elem : m_data[m_now].get_child(elem)) break;
                    if (m_remain <= m_data[m_now].m_length) {
                        size_type u = _newnode(m_remain - 1, m_data[m_now].m_ptr);
                        m_data[u].m_trans.copy_children(m_data[m_now].m_trans);
                        _link(u, m_data[m_now].m_parent), _link(m_now, u);
                        m_now = u;
                    }
                    size_type x = _newnode(inf, m_last - m_remain + 1);
                    _link(x, m_now);
                    m_remain--, m_leaf++;
                    if (lst)
                        m_data[lst].m_fail = m_now, m_data[m_now].m_trans.set_child(*m_data[lst].m_ptr, lst);
                    else if (m_lst2)
                        m_data[m_now].m_trans.set_child(*m_data[m_lst2].m_ptr, m_lst2);
                    if (m_lst2) m_data[x].m_trans.set_child(*m_data[m_lst2].m_ptr, m_lst2);
                    lst = m_now, m_lst2 = x;
                    if (!m_now)
                        m_data[0].m_trans.set_child(*m_data[x].m_ptr, x);
                    else {
                        if (!m_data[m_now].m_parent)
                            m_data[0].m_trans.set_child(*m_data[lst].m_ptr, lst), m_now = 0;
                        else
                            m_now = m_data[m_data[m_now].m_parent].m_fail;
                        if (m_data[m_now].m_length < m_remain - 1)
                            while (m_now = m_data[m_now].get_child((m_last - m_remain)[m_data[m_now].m_length + 1]), m_data[m_now].m_length < m_remain - 1) m_data[m_now].m_trans.set_child(*m_data[lst].m_ptr, lst);
                    }
                }
                if (lst) m_data[lst].m_fail = m_now, m_data[m_now].m_trans.set_child(*m_data[lst].m_ptr, lst);
                if (!m_lst1) m_lst1 = m_data[0].get_child(elem);
                if (m_remain > m_data[m_now].m_length) m_now = m_data[m_now].get_child(elem);
                if (m_remain == m_data[m_now].m_length) m_data[m_now].m_trans.set_child(*m_data[m_lst2].m_ptr, m_lst2);
                m_last++, m_unique_cnt += m_leaf;
            }
            void push_front(const Tp &elem) {
                auto old_Ans = m_unique_cnt;
                *--m_first = elem;
                size_type p = m_lst1, x;
                while (~p && !m_data[p].m_trans.get_child(elem)) p = m_data[p].m_parent;
                if (!~p)
                    x = _newnode(inf, m_first), m_unique_cnt += m_remain + ++m_leaf;
                else if ((m_data[p].m_length == m_remain ? p : m_data[p].get_child(m_data[m_lst1].m_ptr[m_data[p].m_length])) == m_now && m_data[m_lst1].m_ptr[m_leaf - 1] == elem) {
                    x = m_lst2, m_data[x].m_length = inf, m_data[x].m_ptr = m_first;
                    if (x != m_lst1) {
                        m_lst2 = m_data[x].m_trans.get_child(m_data[m_lst1].m_ptr[m_leaf - 2]);
                        m_data[x].m_trans.set_child(m_data[m_lst1].m_ptr[m_leaf - 2], 0);
                    }
                    m_remain++, m_now = x, m_unique_cnt += m_leaf;
                } else
                    x = _newnode(inf, m_first), m_unique_cnt += m_leaf++ + m_remain - m_data[p].m_length;
                if (x != m_lst1) {
                    for (p = m_lst1; ~p && !m_data[p].m_trans.get_child(elem); p = m_data[p].m_parent) m_data[p].m_trans.set_child(elem, x);
                    if (!~p)
                        _link(x, 0);
                    else if (m_data[p].m_trans.get_child(elem) != x) {
                        size_type t = m_data[p].m_trans.get_child(elem);
                        if (m_data[t].m_length == m_data[p].m_length + 1)
                            _link(x, t);
                        else {
                            size_type u = _newnode(m_data[p].m_length + 1, m_data[t].m_ptr);
                            m_data[u].m_fail = p;
                            m_data[u].m_trans.copy_children(m_data[t].m_trans);
                            _link(u, m_data[t].m_parent), _link(t, u), _link(x, u);
                            for (; ~p && m_data[p].m_trans.get_child(elem) == t; p = m_data[p].m_parent) m_data[p].m_trans.set_child(elem, u);
                            if (m_now == t) {
                                if (m_remain <= m_data[u].m_length) m_now = u;
                                if (m_data[u].m_length <= m_remain) m_data[u].m_trans.set_child(*m_data[m_lst2].m_ptr, m_lst2);
                            }
                        }
                    }
                }
                m_lst1 = x;
                if (!m_lst2) m_lst2 = m_lst1;
            }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
        };
    }
    template <typename Tp, BISUFTREE::size_type ChildCount = 26, BISUFTREE::size_type MAX_LEN = 1 << 20>
    using StaticBiSufTree_string = BISUFTREE::Tree<Tp, BISUFTREE::StaticChildGetter<ChildCount>, MAX_LEN>;
}

#endif