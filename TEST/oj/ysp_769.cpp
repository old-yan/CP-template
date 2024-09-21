#include "DS/FHQCounter.h"
#include "DS/GlobalHashMap.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[Point Set Range Frequency](https://judge.yosupo.jp/problem/point_set_range_frequency)(https://github.com/yosupo06/library-checker-problems/issues/769)
 */
/**
 * 本题对每个数值维护其出现位置的有序集合
 * 区间出现次数的查询就是有序集合的区间数量查询
 */

static constexpr uint32_t N = 200000, Q = 200000;
using Counter = OY::FHQCNT::Table<uint32_t, uint32_t, true, false, OY::StaticBufferWrapWithCollect<N>::type>;
OY::GHASH::UnorderedMap<uint32_t, Counter, false, 800003> GS;
uint32_t arr[N];
int main() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) {
        cin >> arr[i];
        GS[arr[i]].add(i, 1);
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t k, v;
            cin >> k >> v;
            GS[arr[k]].add(k, -1);
            GS[arr[k] = v].add(k, 1);
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            if (l == r)
                cout << "0\n";
            else if (auto it = GS.find(x); !it)
                cout << "0\n";
            else
                cout << it->m_mapped.query(l, r - 1) << endl;
        }
    }
}
