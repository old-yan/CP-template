/*
最后修改:
20240925
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_SUBTREEKINDCOUNTER__
#define __OY_SUBTREEKINDCOUNTER__

#include <map>
#include <unordered_map>
#include <utility>

#include "../DS/GlobalHashMap.h"

namespace OY {
    namespace TREEKC {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        template <size_type MAX_VALUE>
        struct ArrayTag {
            template <typename Tp>
            using type = size_type[MAX_VALUE + 1];
        };
        template <bool MakeRecord, size_type BUFFER>
        struct HashmapTag {
            template <typename Tp>
            struct type {
                using hash_table = GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER>;
                using node = typename hash_table::node;
                static hash_table s_hashmap;
                ~type() {
                    if constexpr (MakeRecord) s_hashmap.do_for_each([](node *p) { p->m_mapped = 0; }), s_hashmap.clear();
                }
                size_type &operator[](const Tp &key) { return s_hashmap.insert(key).m_ptr->m_mapped; }
            };
        };
        template <bool MakeRecord, size_type BUFFER>
        template <typename Tp>
        GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER> HashmapTag<MakeRecord, BUFFER>::type<Tp>::s_hashmap;
        template <typename Tag, size_type MAX_BUFFER>
        struct Solver {
            struct node {
                uint32_t m_dfn, m_id;
                bool operator<(const node &rhs) const { return m_dfn < rhs.m_dfn; }
            };
            static node s_stack[MAX_BUFFER];
            template <typename Tree, typename ColorMapping>
            static std::vector<size_type> solve(Tree *rooted_tree, ColorMapping mapping) {
                using Tp = typename std::decay<decltype(mapping(0))>::type;
                size_type n = rooted_tree->vertex_cnt();
                if constexpr (std::is_void<Tag>::value) {
                    struct pair {
                        Tp m_val;
                        size_type m_index;
                        bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                    };
                    std::vector<pair> ps(n);
                    for (size_type i = 0; i != n; i++) ps[i] = {Tp(mapping(i)), i};
                    std::sort(ps.begin(), ps.end());
                    std::vector<size_type> res(n);
                    for (size_type i = 0, j = 0; i != n; i++) {
                        if (i && ps[i - 1].m_val < ps[i].m_val) j++;
                        res[ps[i].m_index] = j;
                    }
                    std::vector<size_type> mp(res[ps[n - 1].m_index] + 1, -1);
                    size_type pos = 0, id = -1;
                    auto pre_work = [&](size_type a, size_type p) {
                        size_type c = res[a], cur = ++id;
                        std::swap(mp[c], cur);
                        res[a] = 1;
                        if (~cur) res[std::prev(std::upper_bound(s_stack, s_stack + pos, node{cur}))->m_id]--;
                        s_stack[pos++] = {id, a};
                    };
                    auto report = [&](size_type a, size_type to) { res[a] += res[to], pos--; };
                    rooted_tree->tree_dp_vertex(rooted_tree->m_root, pre_work, report, {});
                    return res;
                } else {
                    typename Tag::template type<Tp> mp{};
                    std::vector<size_type> res(n);
                    size_type pos = 0, id = 0;
                    auto pre_work = [&](size_type a, size_type p) {
                        auto c = mapping(a);
                        size_type cur = ++id;
                        std::swap(mp[c], cur);
                        res[a] = 1;
                        if (cur) res[std::prev(std::upper_bound(s_stack, s_stack + pos, node{cur}))->m_id]--;
                        s_stack[pos++] = {id, a};
                    };
                    auto report = [&](size_type a, size_type to) { res[a] += res[to], pos--; };
                    rooted_tree->tree_dp_vertex(rooted_tree->m_root, pre_work, report, {});
                    return res;
                }
            }
        };
        template <typename Tag, size_type MAX_BUFFER>
        typename Solver<Tag, MAX_BUFFER>::node Solver<Tag, MAX_BUFFER>::s_stack[MAX_BUFFER];
    }
    template <TREEKC::size_type MAX_VALUE, TREEKC::size_type MAX_BUFFER>
    using ArrayTreeKindCounter = TREEKC::Solver<TREEKC::ArrayTag<MAX_VALUE>, MAX_BUFFER>;
    template <bool MakeRecord, TREEKC::size_type HASH_BUFFER, TREEKC::size_type MAX_BUFFER>
    using HashmapTreeKindCounter = TREEKC::Solver<TREEKC::HashmapTag<MakeRecord, HASH_BUFFER>, MAX_BUFFER>;
}

#endif