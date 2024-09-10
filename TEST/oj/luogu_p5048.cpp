#include "DS/RangeMode.h"
#include "IO/FastIO.h"

/*
[P5048 [Ynoi2019 模拟赛] Yuno loves sqrt technology III](https://www.luogu.com.cn/problem/P5048)
*/
/**
 * 本题为众数模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::RangeMode<uint32_t, false> rm(n, read);
    uint32_t lst{};
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        l ^= lst, r ^= lst;
        lst = rm.query(l - 1, r - 1).m_cnt;
        cout << lst << endl;
    }
}