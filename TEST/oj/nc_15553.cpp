#include "DS/RollbackAdjDiff.h"
#include "IO/FastIO.h"

/*
[数学考试](https://ac.nowcoder.com/acm/problem/15553)
*/
/**
 * 本题需要前缀和差分计算子区间和
 */

int main() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 0; i != t; i++) {
        uint32_t n, k;
        cin >> n >> k;
        OY::RollbackAdjDiff::Table<int64_t> S;
        S.reserve(n);

        int64_t ans = INT64_MIN, first_max = INT64_MIN / 2;
        for (uint32_t i = 0; i != n; i++) {
            int x;
            cin >> x;
            S.push_back(x);
            if (S.size() >= k) {
                if (S.size() >= k * 2) first_max = std::max(first_max, S.query(S.size() - k * 2, S.size() - k - 1));
                ans = std::max(ans, first_max + S.query(S.size() - k, S.size() - 1));
            }
        }
        cout << ans << endl;
    }
}