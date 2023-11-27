#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

/*
[P4718 【模板】Pollard-Rho](https://www.luogu.com.cn/problem/P4718)
*/
/**
 * 本题为 Pollard Rho 算法模板题
*/

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint64_t x;
        cin >> x;
        auto ps = OY::PollardRho::decomposite(x);
        if (ps.size() == 1 and ps[0].m_count == 1)
            cout << "Prime\n";
        else {
            uint64_t max = 0;
            for (auto &[p, c] : ps) max = std::max(max, p);
            cout << max << endl;
        }
    }
}