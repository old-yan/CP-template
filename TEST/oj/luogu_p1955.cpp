#include "DS/DSU.h"
#include "DS/GlobalUnorderedDiscretizer.h"
#include "IO/FastIO.h"

/*
[P1955 [NOI2015] 程序自动分析](https://www.luogu.com.cn/problem/P1955)
*/
/**
 * 先把相等的合并了，再判断不等的
 */

static constexpr uint32_t N = 100000;
OY::GlobalUnorderedDiscretizer<uint32_t, 270007> GD;
struct Query {
    uint32_t a, b, eq;
} qs[N];
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t q;
        cin >> q;
        for (uint32_t i = 0; i != q; i++) {
            auto &[a, b, eq] = qs[i];
            cin >> a >> b >> eq;
            a = GD.insert(a);
            b = GD.insert(b);
        }
        std::partition(qs, qs + q, [&](auto &&x) { return x.eq == 1; });
        OY::DSUTable<true> U(GD.size());
        bool flag = true;
        for (uint32_t i = 0; i != q; i++) {
            auto [a, b, eq] = qs[i];
            if (eq) {
                U.unite_by_size(a, b);
            } else if (U.in_same_group(a, b)) {
                flag = false;
                break;
            }
        }
        cout << (flag ? "YES\n" : "NO\n");
        GD.clear();
    }
}