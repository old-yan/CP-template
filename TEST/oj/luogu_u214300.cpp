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
    auto op = [](auto x, auto y) { return std::max(x, y); };
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    auto S = OY::make_Accumulator<int, true, false>(n, op, read);
    int64_t sum = 0;
    for (uint32_t i = 0; i < n; i++) {
        auto s = S.prefix(i);
        sum += s;
        cout << s << " \n"[i == n - 1];
    }
    cout << sum;
}