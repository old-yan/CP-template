/*
最后修改:
20240308
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STOERWAGNER__
#define __OY_STOERWAGNER__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <valarray>
#include <vector>

namespace OY {
    namespace StoerWagner {
        using size_type = uint32_t;
        template <typename Tp>
        struct Graph {
            std::valarray<std::valarray<Tp>> m_cost;
            std::valarray<std::valarray<bool>> m_behalf;
            std::valarray<bool> m_chosen;
            size_type m_vertex_cnt;
            Graph(size_type vertex_cnt) { m_vertex_cnt = vertex_cnt, m_cost.resize(m_vertex_cnt, std::valarray<Tp>(m_vertex_cnt)); }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_cost[a][b] += cost, m_cost[b][a] += cost; }
            template <bool GetPath, typename SumType = Tp>
            SumType calc(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) {
                std::vector<size_type> a(m_vertex_cnt), a2;
                std::valarray<SumType> w(m_vertex_cnt);
                if constexpr (GetPath) {
                    m_behalf.resize(m_vertex_cnt, std::valarray<bool>(m_vertex_cnt));
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_behalf[i][i] = true;
                }
                Tp ans = infinite;
                for (size_type i = 0; i != m_vertex_cnt; i++) a[i] = i;
                for (size_type i = m_vertex_cnt - 1; i--;) {
                    size_type source = -1, target = -1;
                    a2.assign(a.begin(), a.end()), w = SumType{};
                    while (!a2.empty()) {
                        source = target, target = -1;
                        for (auto i : a2)
                            if (!~target || w[i] > w[target]) target = i;
                        a2.erase(std::find(a2.begin(), a2.end(), target));
                        for (auto i : a2) w[i] += m_cost[target][i];
                    }
                    a.erase(std::find(a.begin(), a.end(), target));
                    if (ans > w[target]) {
                        ans = w[target];
                        if constexpr (GetPath) m_chosen = m_behalf[target];
                    }
                    m_cost[source] += m_cost[target];
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_cost[i][source] += m_cost[i][target];
                    m_cost[source][source] -= m_cost[target][target];
                    if constexpr (GetPath) m_behalf[source] |= m_behalf[target];
                }
                return ans;
            }
            bool is_chosen(size_type i) const { return m_chosen[i]; }
        };
    }
}

#endif