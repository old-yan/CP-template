#include "DS/RangeMode.h"
#include "IO/FastIO.h"

/*
[P3709 大爷的字符串题](https://www.luogu.com.cn/problem/P3709)
*/
/**
 * 本题需要对静态区间进行区间众数查询
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RangeMode<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << '-' << S.query(l - 1, r - 1).m_cnt << endl;
    }
}