#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[#114. k 大异或和](https://loj.ac/p/114)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::StaticHamelXorBase64<50> hxb{};
    for (uint32_t i = 0; i < n; i++) {
        uint64_t x;
        cin >> x;
        hxb.insert(x);
    }
    uint64_t tot = hxb.total_count();
    // 注意，由于本题至少要选一个元素，所以需要注意一下会不会有线性相关的基
    // 有的话，可以选 0；没有的话，不能选 0
    bool flag = hxb.base_count() < n;
    uint32_t m;
    cin >> m;
    while (m--) {
        uint64_t k;
        cin >> k;
        if (flag) {
            if (k <= tot)
                cout << hxb.kth(k - 1) << endl;
            else
                cout << "-1\n";
        } else if (k < tot)
            cout << hxb.kth(k) << endl;
        else
            cout << "-1\n";
    }
}