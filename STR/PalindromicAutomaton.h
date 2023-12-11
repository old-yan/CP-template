/*
最后修改:
20231208
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PALINDROMICAUTOMATON__
#define __OY_PALINDROMICAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace PAM {
        using size_type = uint32_t;
        template <typename Node>
        struct BaseNodeWrap {};
        template <template <typename> typename NodeWrap, size_type ChildCount>
        struct StaticNode : NodeWrap<StaticNode<NodeWrap, ChildCount>> {
            size_type m_child[ChildCount];
            bool has_child(size_type index) const { return m_child[index]; }
            void add_child(size_type index, size_type child) { m_child[index] = child; }
            void remove_child(size_type index) { m_child[index] = 0; }
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename Node, typename Sequence>
        struct Automaton {
            using value_type = typename Sequence::value_type;
            struct node : Node {
                size_type m_length, m_fail, m_first_appear;
            };
            Sequence m_text;
            std::vector<node> m_data;
            std::vector<size_type> m_node;
            static constexpr size_type odd_root() { return 1; }
            static constexpr size_type even_root() { return 2; }
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            void _init() { m_data.resize(3), m_data[0].m_length = -2, m_data[1].m_length = -1, m_data[1].m_fail = 0, m_data[2].m_length = 0, m_data[2].m_fail = 1, m_node.push_back(1); }
            size_type _next(size_type last_node_index, const value_type &elem) const {
                if (last_node_index)
                    for (size_type index = m_text.size() - 1; index == m_data[last_node_index].m_length || m_text[index - m_data[last_node_index].m_length - 1] != elem; last_node_index = m_data[last_node_index].m_fail) {}
                return last_node_index;
            }
            Automaton() { _init(); }
            template <typename InitMapping>
            Automaton(size_type length, InitMapping &&mapping) { resize(length, mapping); }
            template <typename Iterator>
            Automaton(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping &&mapping) {
                reserve(length);
                for (size_type i = 0; i != length; i++) push_back(mapping(i));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type length) { m_text.clear(), m_text.reserve(length), m_data.clear(), m_data.reserve(length + 3), m_node.clear(), m_node.reserve(length + 1), _init(); }
            void clear() { m_text.clear(), m_data.clear(), m_node.clear(), _init(); }
            void push_back(const value_type &elem) {
                m_text.push_back(elem);
                size_type last = _next(m_node.back(), elem), child = m_data[last].get_child(elem);
                if (!child) {
                    child = _newnode();
                    m_data[last].add_child(elem, child);
                    m_data[child].m_length = m_data[last].m_length + 2;
                    size_type fail_next = _next(m_data[last].m_fail, elem);
                    m_data[child].m_fail = fail_next ? m_data[fail_next].get_child(elem) : 2;
                    m_data[child].m_first_appear = m_text.size();
                }
                m_node.push_back(child);
            }
            void pop_back() {
                if (m_data.back().m_first_appear == m_text.size()) {
                    const auto &elem = m_text.back();
                    size_type last = _next(m_node[m_node.size() - 2], elem);
                    m_data[last].remove_child(elem);
                    m_data.pop_back(), m_text.pop_back(), m_node.pop_back();
                } else
                    m_text.pop_back(), m_node.pop_back();
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            const node *get_node(size_type index) const { return &m_data[index]; }
            node *get_node(size_type index) { return &m_data[index]; }
            size_type get_fail(size_type index) const { return m_data[index].m_fail; }
            const node *get_fail_node(size_type index) const { return get_node(get_fail(index)); }
            node *get_fail_node(size_type index) { return get_node(get_fail(index)); }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_data.size() - 1; i; i--) call(i);
            }
        };
    }
    template <template <typename> typename NodeWrap = PAM::BaseNodeWrap, PAM::size_type ChildCount = 26>
    using StaticAutomaton_string = PAM::Automaton<PAM::StaticNode<NodeWrap, ChildCount>, std::string>;
}

#endif