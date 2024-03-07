/*
最后修改:
20240307
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TRIE__
#define __OY_TRIE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace Trie {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Node>
        struct BaseNodeWrap {
            bool m_is_end;
        };
        template <template <typename> typename NodeWrap, size_type ChildCount>
        struct StaticChildGetter : NodeWrap<StaticChildGetter<NodeWrap, ChildCount>> {
            size_type m_child[ChildCount];
            void set_child(size_type index, size_type child) { m_child[index] = child; }
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename Node>
        struct Tree {
            using node = Node;
            std::vector<node> m_data;
            template <typename Mapping, typename Modify = Ignore>
            node *_insert(size_type cur, size_type index, size_type length, Mapping &&mapping, Modify &&modify = Modify()) {
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(&m_data[cur]);
                if (index != length) {
                    size_type c = mapping(index), child = m_data[cur].get_child(c);
                    if (!child) {
                        m_data.push_back({});
                        m_data[cur].set_child(c, child = m_data.size() - 1);
                    }
                    return _insert(child, index + 1, length, mapping, modify);
                } else
                    return &m_data[cur];
            }
            template <typename Mapping>
            node *_find(size_type cur, size_type index, size_type length, Mapping &&mapping) const {
                size_type c = mapping(index), child = m_data[cur].get_child(c);
                if (!child) return nullptr;
                if (++index == length) return (node *)&m_data[child];
                return _find(child, index, length, mapping);
            }
            template <typename Mapping, typename Modify>
            void _trace(size_type cur, size_type index, size_type length, Mapping &&mapping, Modify &&modify) {
                if (index != length) {
                    size_type c = mapping(index), child = m_data[cur].get_child(c);
                    if (child) _trace(cur, index + 1, length, mapping, modify);
                }
                modify(&m_data[cur]);
            }
            void _init() { m_data.push_back({}); }
            Tree() { _init(); }
            void reserve(size_type length) { m_data.clear(), m_data.reserve(length + 1), _init(); }
            void clear() { m_data.clear(), _init(); }
            node *get_node(size_type index) const { return (node *)&m_data[index]; }
            template <typename Mapping, typename Modify = Ignore>
            node *insert(size_type length, Mapping &&mapping, Modify &&modify = Modify()) { return _insert(0, 0, length, mapping, modify); }
            template <typename Modify = Ignore>
            node *insert_lower(const std::string &s, Modify &&modify = Modify()) {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return insert(s.size(), lower_map, modify);
            }
            template <typename Mapping>
            node *find(size_type length, Mapping &&mapping) const { return _find(0, 0, length, mapping); }
            node *find_lower(const std::string &s) const {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return find(s.size(), lower_map);
            }
            template <typename Mapping, typename Modify>
            void trace(size_type length, Mapping &&mapping, Modify &&modify) { return _trace(0, 0, length, mapping, modify); }
            template <typename Modify>
            void trace_lower(const std::string &s, Modify &&modify) {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return _trace(0, lower_map, modify);
            }
        };
    }
    template <template <typename> typename NodeWrap = Trie::BaseNodeWrap, Trie::size_type ChildCount = 26>
    using StaticTrie = Trie::Tree<Trie::StaticChildGetter<NodeWrap, ChildCount>>;
}

#endif