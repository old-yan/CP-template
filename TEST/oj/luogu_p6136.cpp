#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
[P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
*/
/**
 * 本题为 P3369 加强版
 * 为二叉平衡树模板题
*/
uint32_t buf[100000];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> buf[i];
    std::sort(buf, buf + n);
    auto S = OY::FHQTreap<uint32_t, std::less<uint32_t>, 1100001>::from_sorted(buf, buf + n);
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