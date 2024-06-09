#include "DS/RollbackAdjDiff.h"
#include "IO/FastIO.h"

/*
[1481:Maximum sum](http://noi.openjudge.cn/ch0206/1481)
*/
/**
 * 本题需要前缀和差分计算子区间和
 */

int main() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 0; i != t; i++) {
        uint32_t n;
        cin >> n;
        OY::RollbackAdjDiff::Table<int64_t> S;
        S.reserve(n);

        int64_t ans1 = INT64_MIN, ans2 = INT64_MIN, pre_min1 = 0, pre_min2 = INT64_MAX / 2;
        for (uint32_t i = 0; i != n; i++) {
            int x;
            cin >> x;
            S.push_back(x);
            ans1 = std::max(ans1, S.query_all() - pre_min1);
            pre_min1 = std::min(pre_min1, S.query_all());
            ans2 = std::max(ans2, S.query_all() - pre_min2);
            pre_min2 = std::min(pre_min2, S.query_all() - ans1);
        }
        cout << ans2 << endl;
    }
}