#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[XOR](https://acm.hdu.edu.cn/showproblem.php?pid=3949)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        cout << "Case #" << i << ":\n";
        uint32_t n;
        cin >> n;
        OY::StaticHamelXorBase64<60> hxb{};
        for (uint32_t j = 0; j < n; j++) {
            uint64_t x;
            cin >> x;
            hxb.insert(x);
        }
        auto tot = hxb.total_count();
        // 注意，由于本题至少要选一个元素，所以需要注意一下会不会有线性相关的基
        // 有的话，可以选 0；没有的话，不能选 0
        bool flag = hxb.base_count() < n;
        uint32_t q;
        cin >> q;
        while (q--) {
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
}