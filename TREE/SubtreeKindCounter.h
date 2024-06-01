/*
最后修改:
20240504
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
            static size_type s_buffer[MAX_BUFFER + 1], s_id, s_tot;
            static size_type _lowbit(size_type x) { return x & -x; }
            static void _plus_one(size_type i, size_type n) {
                while (i <= n) {
                    s_buffer[i]++;
                    i += _lowbit(i + 1);
                }
            }
            static void _minus_one(size_type i, size_type n) {
                if (!i) return void(s_tot++);
                while (i <= n) {
                    s_buffer[i]--;
                    i += _lowbit(i + 1);
                }
            }
            static size_type _presum(size_type x) {
                size_type res{};
                for (size_type j = x; ~j; j -= _lowbit(j + 1)) res += s_buffer[j];
                return res;
            }
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
                    std::vector<Tp> sorted;
                    std::vector<size_type> res(n);
                    sorted.reserve(n);
                    for (size_type i = 0; i != n; i++) {
                        if (!i || ps[i - 1].m_val < ps[i].m_val) sorted.push_back(ps[i].m_val);
                        res[ps[i].m_index] = sorted.size() - 1;
                    }
                    std::vector<size_type> mp(sorted.size());
                    s_id = 1, s_tot = 0;
                    auto pre_work = [&](size_type a, size_type p) {
                        size_type cur = s_id;
                        std::swap(mp[res[a]], cur);
                        _minus_one(cur, n), _plus_one(s_id, n);
                        res[a] = s_id++;
                    };
                    auto after_work = [&](size_type a) { res[a] = s_tot - _presum(res[a] - 1); };
                    rooted_tree->tree_dp_vertex(rooted_tree->m_root, pre_work, {}, after_work);
                    std::fill_n(s_buffer, n + 1, 0);
                    return res;
                } else {
                    typename Tag::template type<Tp> mp{};
                    std::vector<size_type> res(n);
                    s_id = 1, s_tot = 0;
                    auto pre_work = [&](size_type a, size_type p) {
                        size_type cur = s_id;
                        std::swap(mp[mapping(a)], cur);
                        _minus_one(cur, n), _plus_one(s_id, n);
                        res[a] = s_id++;
                    };
                    auto after_work = [&](size_type a) { res[a] = s_tot - _presum(res[a] - 1); };
                    rooted_tree->tree_dp_vertex(rooted_tree->m_root, pre_work, {}, after_work);
                    std::fill_n(s_buffer, n + 1, 0);
                    return res;
                }
            }
        };
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_buffer[MAX_BUFFER + 1];
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_id;
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_tot;
    }
    template <TREEKC::size_type MAX_VALUE, TREEKC::size_type MAX_BUFFER>
    using ArrayTreeKindCounter = TREEKC::Solver<TREEKC::ArrayTag<MAX_VALUE>, MAX_BUFFER>;
    template <bool MakeRecord, TREEKC::size_type HASH_BUFFER, TREEKC::size_type MAX_BUFFER>
    using HashmapTreeKindCounter = TREEKC::Solver<TREEKC::HashmapTag<MakeRecord, HASH_BUFFER>, MAX_BUFFER>;
}

#endif