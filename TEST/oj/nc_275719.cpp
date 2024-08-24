#include "DS/DynamicBitset.h"
#include "IO/FastIO.h"
#include "MISC/SubsetSumKnapsack.h"

/*
[自爆机器人](https://ac.nowcoder.com/acm/problem/275719)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/88455/F
 * 显然，在任意墙壁间的反弹，都可以分解为相邻两堵墙之间的反弹的叠加
 * 可以反弹任意次数，所以为完全背包子集和
 */

int main() {
    uint32_t t = 1;
    cin >> t;
    while (t--) {
        uint32_t n, m, t;
        cin >> n >> m >> t;
        std::vector<uint32_t> ts(m);
        for (uint32_t i = 0; i < m; i++) cin >> ts[i];
        if (n > t)
            cout << "0\n";
        else {
            std::sort(ts.begin(), ts.end(), std::greater<uint32_t>());
            for (uint32_t i = 0; i + 1 < ts.size(); i++) ts[i] -= ts[i + 1];
            ts.pop_back();
            OY::DynamicBitset table((t - n) / 2 + 1);
            OY::SSK::solve_unbounded(table, ts, (t - n) / 2);
            auto d = table.last_one() * 2;
            cout << n + d << '\n';
        }
    }
}