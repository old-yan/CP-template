#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
[P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FHQTreap<uint32_t, std::less<uint32_t>, 1100001> S;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        S.insert_by_key(x);
    }
    uint32_t last = 0, sum = 0;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t x;
        cin >> op >> x;
        if (op == '1')
            S.insert_by_key(x ^ last);
        else if (op == '2')
            S.erase_by_key(x ^ last);
        else if (op == '3')
            last = S.rank(x ^ last) + 1;
        else if (op == '4')
            last = S.kth((x ^ last) - 1)->get();
        else if (op == '5')
            last = S.smaller_bound(x ^ last)->get();
        else
            last = S.upper_bound(x ^ last)->get();
        if (op >= '3') sum ^= last;
    }
    cout << sum;
}