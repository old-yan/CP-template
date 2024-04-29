/*
最后修改:
20240429
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_SUBTREEKINDCOUNTER__
#define __OY_SUBTREEKINDCOUNTER__

#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>
#include <unordered_map>
#include <vector>

namespace OY {
    namespace TREEKC {
        using size_type = uint32_t;
        struct MapTag {
            template <typename Tp>
            using type = std::map<Tp, size_type>;
        };
        struct UmapTag {
            template <typename Tp>
            using type = std::unordered_map<Tp, size_type>;
        };
        template <size_type MAX_VALUE>
        struct ArrayTag {
            template <typename Tp>
            using type = size_type[MAX_VALUE + 1];
        };
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
                typename Tag::type<Tp> mp{};
                size_type n = rooted_tree->vertex_cnt();
                std::vector<size_type> res(n);
                s_id = 1, s_tot = 0;
                auto pre_work = [&](size_type a, size_type p) {
                    auto color = mapping(a);
                    size_type cur = s_id;
                    std::swap(mp[color], cur);
                    _minus_one(cur, n), _plus_one(s_id, n);
                    res[a] = s_id++;
                };
                auto after_work = [&](size_type a) { res[a] = s_tot - _presum(res[a] - 1); };
                rooted_tree->tree_dp_vertex(rooted_tree->m_root, pre_work, {}, after_work);
                std::fill_n(s_buffer, n + 1, 0);
                return res;
            }
        };
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_buffer[MAX_BUFFER + 1];
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_id;
        template <typename Tag, size_type MAX_BUFFER>
        size_type Solver<Tag, MAX_BUFFER>::s_tot;
    }
    template <TREEKC::size_type MAX_BUFFER>
    using MapTreeKindCounter = TREEKC::Solver<TREEKC::MapTag, MAX_BUFFER>;
    template <TREEKC::size_type MAX_BUFFER>
    using UmapTreeKindCounter = TREEKC::Solver<TREEKC::UmapTag, MAX_BUFFER>;
    template <TREEKC::size_type MAX_VALUE, TREEKC::size_type MAX_BUFFER>
    using ArrayTreeKindCounter = TREEKC::Solver<TREEKC::ArrayTag<MAX_VALUE>, MAX_BUFFER>;
}

#endif