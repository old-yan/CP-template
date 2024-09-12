#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

/*
[P1196 [NOI2002] 银河英雄传说](https://www.luogu.com.cn/problem/P1196)
*/
/**
 * 维护势能关系的并查集
 */

int main() {
    uint32_t t;
    cin >> t;
    OY::AddPDSUTable<uint32_t, true> U(30000);
    while (t--) {
        char op;
        uint32_t a, b;
        cin >> op >> a >> b;
        if (op == 'M') {
            a = U.find(a - 1).m_head;
            b = U.find(b - 1).m_head;
            U.unite_to(a, b, U.size<true>(b));
        } else {
            auto [flag, dis] = U.calc(a - 1, b - 1);
            if (flag)
                cout << (dis > 30000 ? -dis - 1 : dis - 1) << endl;
            else
                cout << "-1\n";
        }
    }
}
