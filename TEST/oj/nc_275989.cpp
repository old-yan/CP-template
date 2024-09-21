#include "DS/MonoAVL.h"
#include "IO/FastIO.h"

/*
[旅途的终点](https://ac.nowcoder.com/acm/problem/275989)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/86639/G
 * 维护最小的 k 个元素之和
 */

using Tree = OY::VectorMonoSumAVL<uint64_t, uint64_t, 0, false>;
int main() {
    uint32_t n, k;
    uint64_t m;
    cin >> n >> m >> k;
    Tree::_reserve(n + 1);
    Tree S;
    uint32_t ans{};
    for (uint32_t i = 0; i < n; i++) {
        uint64_t x;
        cin >> x;
        S.insert_by_comparator(x);
        if (S.size() > k and S.query(0, S.size() - k - 1) >= m) break;
        ans++;
    }
    cout << ans;
}