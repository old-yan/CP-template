/*
最后修改:
20231219
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ONLINEACAUTOMATON__
#define __OY_ONLINEACAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "ACAutomaton.h"

namespace OY {
    namespace ONLINEAC {
        using size_type = uint32_t;
        struct BaseNode {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename = void>
        struct Has_set_get : std::false_type {};
        template <typename Tp>
        struct Has_set_get<Tp, void_t<decltype(std::declval<Tp>().set(std::declval<Tp>().get()))>> : std::true_type {};
        template <typename Node, size_type ChildCount>
        struct Automaton {
            struct InnerNode : Node {
                size_type m_dep, m_length;
            };
            using ACAM_type = AC::Automaton<InnerNode, ChildCount>;
            std::vector<ACAM_type> m_acs;
            void _insert(const ACAM_type &ac, size_type node_index, size_type dep, ACAM_type &ac2, size_type cur) const {
                if (dep && dep == ac.get_node(node_index)->m_length) {
                    ac2.get_node(cur)->m_length = dep;
                    if constexpr (Has_set_get<typename ACAM_type::node>::value) ac2.get_node(cur)->set(ac.get_node(node_index)->get());
                }
                for (size_type i = 0; i != ChildCount; i++)
                    if (ac.get_node(node_index)->has_child(i)) {
                        size_type child = ac.get_node(node_index)->get_child(i);
                        if (ac.get_node(child)->m_dep == dep + 1) {
                            size_type nxt = ac2._get_or_new(cur, i);
                            _insert(ac, child, dep + 1, ac2, nxt);
                        }
                    }
            }
            void clear() {
                for (auto &ac : m_acs) ac.clear();
                m_acs.clear();
            }
            size_type size() const { return m_acs.size(); }
            bool empty() const { return m_acs.empty(); }
            template <typename Iterator, typename Mapping>
            bool contains(Iterator first, Iterator last, Mapping &&map) const {
                for (auto &ac : m_acs) {
                    auto traverse = [&]() {
                        size_type x = 0, len = 0;
                        for (auto it = first; it != last; ++it) {
                            size_type c = map(*it);
                            x = ac.get_node(x)->get_child(c);
                            if (ac.get_node(x)->m_dep != ++len) return false;
                        }
                        return ac.get_node(x)->m_length == len;
                    };
                    if (traverse()) return true;
                }
                return false;
            }
            template <typename Iterator, typename Mapping>
            std::pair<ACAM_type *, size_type> insert(Iterator first, Iterator last, Mapping &&map) {
                size_type ac_index = 0;
                while (ac_index != m_acs.size() && !m_acs[ac_index].empty()) ac_index++;
                if (ac_index == m_acs.size()) m_acs.push_back({});
                for (size_type i = 0; i != ac_index; i++) {
                    _insert(m_acs[i], 0, 0, m_acs[ac_index], 0);
                    m_acs[i].clear();
                }
                size_type pos = m_acs[ac_index].insert(first, last, map);
                m_acs[ac_index].get_node(pos)->m_length = last - first;
                m_acs[ac_index].prepare([&](size_type i, size_type child) {
                    m_acs[ac_index].get_node(child)->m_dep = m_acs[ac_index].get_node(i)->m_dep + 1;
                });
                return std::make_pair(&m_acs[ac_index], pos);
            }
            std::pair<ACAM_type *, size_type> insert_lower(const std::string &s) {
                return insert(s.begin(), s.end(), [](char c) { return c - 'a'; });
            }
            const ACAM_type *get_ACAM(size_type ac_index) const { return &m_acs[ac_index]; }
            template <typename Callback>
            void do_for_each_ACAM(Callback &&call) const {
                for (auto &ac : m_acs) call(ac);
            }
        };
    }
    template <typename Node = ONLINEAC::BaseNode, ONLINEAC::size_type ChildCount = 26>
    using OnlineACAM = ONLINEAC::Automaton<Node, ChildCount>;
}

#endif