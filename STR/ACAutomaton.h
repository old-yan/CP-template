/*
最后修改:
20231208
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ACAUTOMATON__
#define __OY_ACAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace AC {
        using size_type = uint32_t;
        template <template <typename> typename NodeWrap, size_type ChildCount>
        struct Automaton {
            struct node : NodeWrap<node> {
                size_type m_child[ChildCount], m_fail;
                bool has_child(size_type index) const { return m_child[index]; }
                void add_child(size_type index, size_type child) {
                    m_child[index] = child;
                }
                size_type get_child(size_type index) const { return m_child[index]; }
            };
            std::vector<node> m_data;
            std::vector<size_type> m_queue;
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            Automaton() : m_data(1) {}
            void reserve(size_type length) { m_data.reserve(length + 1); }
            template <typename Iterator, typename Mapping>
            size_type insert(Iterator first, Iterator last, Mapping &&map) {
                size_type x = 0;
                for (auto it = first; it != last; ++it) {
                    size_type c = map(*it);
                    if (!m_data[x].has_child(c)) {
                        size_type y = _newnode();
                        m_data[x].add_child(c, y);
                    }
                    x = m_data[x].get_child(c);
                }
                return x;
            }
            size_type insert_lower(const std::string &s) {
                return insert(s.begin(), s.end(), [](char c) { return c - 'a'; });
            }
            void prepare() {
                m_queue.reserve(m_data.size() - 1);
                size_type cursor = 0;
                for (size_type i = 0; i != ChildCount; i++)
                    if (m_data[0].get_child(i))
                        m_queue.push_back(m_data[0].get_child(i));
                while (cursor != m_queue.size()) {
                    size_type cur = m_queue[cursor++];
                    for (size_type i = 0; i != ChildCount; i++) {
                        size_type child = m_data[cur].get_child(i);
                        if (child) {
                            m_data[child].m_fail = m_data[m_data[cur].m_fail].get_child(i);
                            m_queue.push_back(child);
                        } else
                            m_data[cur].add_child(i, m_data[m_data[cur].m_fail].get_child(i));
                    }
                }
            }
            const node *get_node(size_type index) const { return &m_data[index]; }
            node *get_node(size_type index) { return &m_data[index]; }
            size_type get_fail(size_type index) const { return m_data[index].m_fail; }
            const node *get_fail_node(size_type index) const { return get_node(get_fail(index)); }
            node *get_fail_node(size_type index) { return get_node(get_fail(index)); }
            size_type next(size_type last_pos, size_type elem) const { return m_data[last_pos].get_child(elem); }
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (auto &a : m_queue) call(a);
            }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_queue.size() - 1; ~i; i--) call(m_queue[i]);
            }
        };
    }
}

#endif