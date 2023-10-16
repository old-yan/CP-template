#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P2147 [SDOI2008] 洞穴勘测](https://www.luogu.com.cn/problem/P2147)
*/
/**
 * 本题涉及加边、断边、连通性查询
 * 树的形态有变化，需要使用 LCT
*/
static constexpr uint32_t N = 10000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LCTTree<true, false, N + 1> S(n);
    for (uint32_t i = 0; i < m; i++) {
        std::string op;
        uint32_t a, b;
        cin >> op >> a >> b;
        if (op[0] == 'C')
            S.connect<false>(a - 1, b - 1);
        else if (op[0] == 'D')
            S.disconnect<false>(a - 1, b - 1);
        else
            cout << (S.in_same_group(a - 1, b - 1) ? "Yes\n" : "No\n");
    }
}
