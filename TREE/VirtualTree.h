/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_VIRTUALTREE__
#define __OY_VIRTUALTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace VTREE {
        using size_type = uint32_t;
        template <size_type MAX_BUFFER>
        struct VirtualTree {
            struct node {
                size_type m_id, m_dfn;
            };
            static node s_buffer[MAX_BUFFER];
            static size_type s_key_buffer[MAX_BUFFER];
            template <typename Iterator, typename DFNGetter, typename LCAGetter, typename Callback>
            static void solve(Iterator first, Iterator last, DFNGetter &&dfn_getter, LCAGetter &&lca_getter, Callback &&call) {
                size_type n = last - first, len = 0;
                auto dfn_comp = [&](size_type x, size_type y) { return dfn_getter(x) < dfn_getter(y); };
                std::copy(first, last, s_key_buffer);
                std::sort(s_key_buffer, s_key_buffer + n, dfn_comp);
                for (size_type i = 0; i < n; i++) {
                    size_type a = s_key_buffer[i];
                    if (len) {
                        size_type lca = lca_getter(a, s_buffer[len - 1].m_id), dfn = dfn_getter(lca);
                        while (s_buffer[len - 1].m_dfn > dfn)
                            if (len >= 2 && s_buffer[len - 2].m_dfn >= dfn) {
                                call(s_buffer[len - 1].m_id, s_buffer[len - 2].m_id);
                                len--;
                            } else {
                                call(s_buffer[len - 1].m_id, lca);
                                s_buffer[len - 1] = {lca, dfn};
                                break;
                            }
                    }
                    s_buffer[len++] = {a, dfn_getter(a)};
                }
                while (--len) call(s_buffer[len].m_id, s_buffer[len - 1].m_id);
            }
            template <typename Iterator, typename RMQLCA, typename Callback>
            static void solve_rmqlca(Iterator first, Iterator last, RMQLCA &&rmqlca, Callback &&call) {
                solve(
                    first, last, [&](size_type a) { return rmqlca.m_dfn[a]; }, [&](size_type a, size_type b) { return rmqlca.calc(a, b); }, call);
            }
            template <typename Iterator, typename HLD, typename Callback>
            static void solve_hld(Iterator first, Iterator last, HLD &&hld, Callback &&call) {
                solve(
                    first, last, [&](size_type a) { return hld.m_info[a].m_dfn; }, [&](size_type a, size_type b) { return hld.calc(a, b); }, call);
            }
        };
        template <size_type MAX_BUFFER>
        typename VirtualTree<MAX_BUFFER>::node VirtualTree<MAX_BUFFER>::s_buffer[MAX_BUFFER];
        template <size_type MAX_BUFFER>
        size_type VirtualTree<MAX_BUFFER>::s_key_buffer[MAX_BUFFER];
    }
}

#endif