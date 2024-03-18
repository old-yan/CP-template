#include "DS/Accumulator.h"
#include "IO/FastIO.h"

/*
[U214300 前缀最大值之和](https://www.luogu.com.cn/problem/U214300)
*/
/**
 * 本题为前缀表模板题
 */
int main() {
    uint32_t n;
    cin >> n;
    auto S = OY::make_Accumulator<int, true, false>(n, std::max<int>, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    int64_t sum = 0;
    for (uint32_t i = 0; i < n; i++) {
        auto s = S.prefix(i);
        sum += s;
        cout << s << ' ';
    }
    cout << endl
         << sum;
}