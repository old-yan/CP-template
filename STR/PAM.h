/*
最后修改:
20240107
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PAM__
#define __OY_PAM__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

namespace OY {
    namespace PAM {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void add_child(size_type index, size_type child) { m_child[index] = child; }
            void remove_child(size_type index) { m_child[index] = 0; }
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename ChildGetter, typename Sequence>
        struct Automaton {
            using value_type = typename Sequence::value_type;
            struct node : ChildGetter {
                size_type m_length, m_fail, m_ancestor, m_size_when_appear;
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
            void _init() { m_data.resize(2), m_data[0].m_length = m_data[0].m_fail = -1, m_data[1].m_length = m_data[1].m_fail = 0, m_data[1].m_ancestor = 1, m_node.push_back(0); }
            size_type _jump(size_type node_index, size_type index, const value_type &elem) const {
                if (~node_index)
                    while (index == m_data[node_index].m_length || m_text[index - m_data[node_index].m_length - 1] != elem) node_index = m_data[node_index].m_fail;
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
                size_type index = m_text.size();
                m_text.push_back(elem);
                size_type last = _jump(m_node.back(), index, elem), child = m_data[last].get_child(elem);
                if (!child) {
                    child = _newnode();
                    m_data[last].add_child(elem, child);
                    m_data[child].m_length = m_data[last].m_length + 2;
                    size_type fail = _jump(m_data[last].m_fail, index, elem);
                    fail = ~fail ? m_data[fail].get_child(elem) : 1;
                    m_data[child].m_fail = fail;
                    m_data[child].m_ancestor = m_data[m_data[fail].m_fail].m_length + m_data[child].m_length == m_data[fail].m_length * 2 ? m_data[fail].m_ancestor : fail;
                    m_data[child].m_size_when_appear = m_text.size();
                }
                m_node.push_back(child);
            }
            void pop_back() {
                if (m_data.back().m_size_when_appear == m_text.size()) {
                    const auto &elem = m_text.back();
                    size_type last = _jump(m_node[m_node.size() - 2], m_text.size() - 1, elem);
                    m_data[last].remove_child(elem);
                    m_data.pop_back(), m_text.pop_back(), m_node.pop_back();
                } else
                    m_text.pop_back(), m_node.pop_back();
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            size_type query_fail(size_type node_index) const { return m_data[node_index].m_fail; }
            size_type query_ancestor(size_type node_index) const { return m_data[node_index].m_ancestor; }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
            const node *get_fail_node(size_type node_index) const { return get_node(query_fail(node_index)); }
            node *get_fail_node(size_type node_index) { return get_node(query_fail(node_index)); }
            bool query(size_type left, size_type right) const {
                size_type len = right - left + 1, cur = m_node[right + 1], cur_len = m_data[cur].m_length;
                if (cur_len < len) return false;
                while (true) {
                    size_type fail = m_data[cur].m_fail, nxt = m_data[cur].m_ancestor, shortest = m_data[nxt].m_length;
                    if (!~shortest) shortest = 1;
                    size_type delta = fail == nxt ? cur_len - shortest : cur_len - m_data[fail].m_length;
                    if (len >= shortest) return (len - shortest) % delta == 0;
                    cur = nxt, cur_len = shortest;
                }
                return false;
            }
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
                    size_type fail = m_data[cur].m_fail, nxt = m_data[cur].m_ancestor, shortest = m_data[nxt].m_length;
                    if (!shortest) shortest = 1;
                    if (fail == nxt)
                        call(series{cur_len, shortest, cur_len - shortest});
                    else
                        call(series{cur_len, shortest, cur_len - m_data[fail].m_length});
                    cur = nxt, cur_len = shortest;
                }
            }
            template <typename Callback>
            void do_for_each_node(size_type init_node_index, Callback &&call) const {
                size_type node_index = init_node_index;
                do call(node_index);
                while (is_node(node_index = m_data[node_index].m_fail));
            }
        };
    }
    template <PAM::size_type ChildCount = 26>
    using StaticPAM_string = PAM::Automaton<PAM::StaticChildGetter<ChildCount>, std::string>;
}

#endif