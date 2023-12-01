#include "GRAPH/BFS.h"
#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticModInt32.h"

/*
[P7293 [USACO21JAN] Sum of Distances P](https://www.luogu.com.cn/problem/P7293)
*/
/**
 * 本题需要认识到，多维空间里的点的最短路，等于每个维度的奇最短路的最大值，与每个维度的偶最短路的最大值，两者中的最小值
 * min(max1,max2) 可以转化为 max1+max2-max(max1,max2)
 * 最终，我们可以通过对每个维度上的 最大值->数量 进行 Fast-max 卷积，而计算出到 (1,1,1...1) 的最短路为 x 的点有多少个
 */

int main() {
    using mint = OY::mint1000000007;
    uint32_t k;
    cin >> k;
    std::vector<std::vector<mint>> odd_max, even_max, odd_even_max;
    for (uint32_t i = 0; i < k; i++) {
        // 建图求最短路
        uint32_t n, m;
        cin >> n >> m;
        OY::BFS::Graph<200000, 800000> G(n * 2, m * 4);
        for (uint32_t j = 0; j < m; j++) {
            uint32_t a, b;
            cin >> a >> b;
            a--, b--;
            G.add_edge(a * 2, b * 2 + 1), G.add_edge(a * 2 + 1, b * 2);
            if (a != b) G.add_edge(b * 2, a * 2 + 1), G.add_edge(b * 2 + 1, a * 2);
        }
        auto sol = G.calc<false>(0);

        // 把奇最短路、偶最短路、奇偶最长最短路的分布图都记录下来
        std::vector<mint> odd(m * 2 + 1), even(m * 2 + 1), odd_even(m * 2 + 1);
        for (uint32_t j = 0; j < n; j++) {
            uint32_t d1 = sol.query(j * 2), d2 = sol.query(j * 2 + 1);
            if (d1 <= m * 2) even[d1]++;
            if (d2 <= m * 2) odd[d2]++;
            if (d1 <= m * 2 && d2 <= m * 2) odd_even[std::max(d1, d2)]++;
        }
        odd_max.push_back(std::move(odd));
        even_max.push_back(std::move(even));
        odd_even_max.push_back(std::move(odd_even));
    }
    // 先排一下序
    std::sort(odd_max.begin(), odd_max.end(), [](std::vector<mint> &x, std::vector<mint> &y) { return x.size() < y.size(); });
    std::sort(even_max.begin(), even_max.end(), [](std::vector<mint> &x, std::vector<mint> &y) { return x.size() < y.size(); });
    std::sort(odd_even_max.begin(), odd_even_max.end(), [](std::vector<mint> &x, std::vector<mint> &y) { return x.size() < y.size(); });
    // 对分布图进行卷积
    for (uint32_t i = 1; i < odd_max.size(); i++) {
        odd_max[0].resize(odd_max[i].size());
        OY::FASTTRANS::fast_max_transform<true>(odd_max[0].begin(), odd_max[0].end());
        OY::FASTTRANS::fast_max_transform<true>(odd_max[i].begin(), odd_max[i].end());
        for (uint32_t j = 0; j < odd_max[0].size(); j++) odd_max[0][j] *= odd_max[i][j];
        OY::FASTTRANS::fast_max_transform<false>(odd_max[0].begin(), odd_max[0].end());
    }
    for (uint32_t i = 1; i < even_max.size(); i++) {
        even_max[0].resize(even_max[i].size());
        OY::FASTTRANS::fast_max_transform<true>(even_max[0].begin(), even_max[0].end());
        OY::FASTTRANS::fast_max_transform<true>(even_max[i].begin(), even_max[i].end());
        for (uint32_t j = 0; j < even_max[0].size(); j++) even_max[0][j] *= even_max[i][j];
        OY::FASTTRANS::fast_max_transform<false>(even_max[0].begin(), even_max[0].end());
    }
    for (uint32_t i = 1; i < odd_even_max.size(); i++) {
        odd_even_max[0].resize(odd_even_max[i].size());
        OY::FASTTRANS::fast_max_transform<true>(odd_even_max[0].begin(), odd_even_max[0].end());
        OY::FASTTRANS::fast_max_transform<true>(odd_even_max[i].begin(), odd_even_max[i].end());
        for (uint32_t j = 0; j < odd_even_max[0].size(); j++) odd_even_max[0][j] *= odd_even_max[i][j];
        OY::FASTTRANS::fast_max_transform<false>(odd_even_max[0].begin(), odd_even_max[0].end());
    }
    // 把贡献加起来
    mint ans = 0;
    for (uint32_t d = 0; d < odd_max[0].size(); d++) ans += d * odd_max[0][d];
    for (uint32_t d = 0; d < even_max[0].size(); d++) ans += d * even_max[0][d];
    for (uint32_t d = 0; d < odd_even_max[0].size(); d++) ans -= d * odd_even_max[0][d];
    cout << ans;
}
