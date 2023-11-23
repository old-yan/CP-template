#include "IO/FastIO.h"
#include "MATH/ExtendedEuclidean.h"

/*
[P5656 【模板】二元一次不定方程 (exgcd)](https://www.luogu.com.cn/problem/P5656)
*/
/**
 * 本题为扩展欧几里得算法模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        // 因为我的模板返回的是 a 的最小非负系数，要求的是 a 的最小正系数
        // 不妨开始时，主动先取一个 a
        int64_t a, b, target;
        cin >> a >> b >> target;
        auto res = OY::ExtenedEuclideanSolver::solve(a, b, target - a);
        if (!res.m_flag)
            cout << "-1\n";
        else {
            res.m_coef1++;
            if (a * res.m_coef1 >= target) {
                cout << res.m_coef1 << ' ';
                cout << OY::ExtenedEuclideanSolver::solve(b, a, target - b).m_coef1 + 1 << endl;
            } else {
                auto coef2 = (target - a * res.m_coef1) / b;
                auto d1 = b / res.m_gcd, d2 = a / res.m_gcd;
                auto cnt = (coef2 - 1) / d2;
                cout << cnt + 1 << ' ' << res.m_coef1 << ' ' << coef2 - cnt * d2 << ' ' << res.m_coef1 + cnt * d1 << ' ' << coef2 << endl;
            }
        }
    }
}