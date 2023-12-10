#include "DS/WaveLet.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

/*
[P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
*/
/**
 * 本题求子串出现次数，可以有多种做法
 * 可以使用后缀数组解决
 */

void solve_SA() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;

    OY::SA_string<100000> SA(s);
    SA.get_rank(), SA.get_height();
    auto st = OY::make_ZkwTree<uint32_t, 1 << 18>(n, std::min<uint32_t>, [&](uint32_t i) { return SA.query_height(i); });
    OY::WaveLetTable<uint32_t, uint64_t, 100000> wave(
        s.size(), [&](uint32_t i) { return SA.query_sa(i); }, 17);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c, d;
        cin >> a >> b >> c >> d;

        auto check = [&](uint32_t len) {
            uint32_t rnk = SA.query_rank(c - 1);
            auto l = st.min_left(rnk, [&](auto x) { return x >= len; }) - 1;
            auto r = st.max_right(rnk + 1, [&](auto x) { return x >= len; });
            return wave.count(l, r, a - 1, b - len);
        };

        uint32_t low = 0, high = std::min(d - c + 1, b - a + 1);
        while (low < high) {
            auto mid = (low + high + 1) / 2;
            if (check(mid))
                low = mid;
            else
                high = mid - 1;
        }
        cout << low << endl;
    }
}

int main() {
    solve_SA();
}