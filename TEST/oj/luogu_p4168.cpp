#include "DS/RangeMode.h"
#include "IO/FastIO.h"
#include "MATH/Barrett32.h"

/*
[P4168 [Violet] 蒲公英](https://www.luogu.com.cn/problem/P4168)
*/
/**
 * 本题需要对静态区间进行区间众数查询
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::RangeMode<uint32_t, true> S(n, read);
    uint32_t lst{};
    OY::Barrett32 L(n);
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        l = (l + lst - 1) % L, r = (r + lst - 1) % L;
        if (l > r) std::swap(l, r);
        cout << (lst = S.query_min(l, r).m_val) << endl;
    }
}