#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P3950 部落冲突](https://www.luogu.com.cn/problem/P3950)
*/
/**
 * 本题涉及加边、断边、连通性查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 300000, M = 300000;
uint32_t wars[M][2], id;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LCTTree<true, false, N + 1> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t a, b;
            cin >> a >> b;
            cout << (S.in_same_group(a - 1, b - 1) ? "Yes\n" : "No\n");
        } else if (op == 'C') {
            uint32_t a, b;
            cin >> a >> b;
            S.disconnect<false>(a - 1, b - 1);
            wars[id][0] = a - 1, wars[id][1] = b - 1, id++;
        } else {
            uint32_t x;
            cin >> x;
            uint32_t a = wars[x - 1][0], b = wars[x - 1][1];
            S.connect<false>(a, b);
        }
    }
}
