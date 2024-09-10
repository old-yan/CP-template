#include "IO/FastIO.h"

/*
[Longest Increasing Subsequence](https://judge.yosupo.jp/problem/longest_increasing_subsequence)(https://github.com/yosupo06/library-checker-problems/issues/803)
*/
/**
 * 本题为最长上升子序列模板
 */

struct LIS {
    template <typename Tp, typename Mapping>
    static std::vector<uint32_t> solve(uint32_t n, Mapping mapping) {
        std::vector<std::pair<Tp, uint32_t>> dp;
        dp.reserve(n);
        std::vector<uint32_t> trace(n);
        for (uint32_t i = 0; i != n; i++) {
            auto x = mapping(i);
            auto it = std::lower_bound(dp.begin(), dp.end(), x, [x](auto a, Tp) { return a.first < x; });
            trace[i] = it == dp.begin() ? -1 : std::prev(it)->second;
            if (it == dp.end())
                dp.emplace_back(x, i);
            else if (x < it->first)
                *it = {x, i};
        }
        uint32_t i = dp.back().second;
        std::vector<uint32_t> indexes;
        indexes.reserve(dp.size());
        do indexes.push_back(i);
        while (~(i = trace[i]));
        std::reverse(indexes.begin(), indexes.end());
        return indexes;
    }
};
int main() {
    uint32_t n;
    cin >> n;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto res = LIS::solve<uint32_t>(n, read);
    cout << res.size() << endl;
    for (auto e : res) cout << e << ' ';
}