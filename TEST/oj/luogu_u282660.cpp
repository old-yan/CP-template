#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[U282660 区间合并](https://www.luogu.com.cn/problem/U282660)
*/
/**
 * 本题为区间合并模板题
*/
int main() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<int> rm;
    for (uint32_t i = 0; i < n; i++) {
        int l, r;
        cin >> l >> r;
        rm.add_range({l, r - 1});
    }
    cout << rm.size();
}