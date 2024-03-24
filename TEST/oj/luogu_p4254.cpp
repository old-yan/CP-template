#include "DS/LichaoZkwTree.h"
#include "IO/FastIO.h"

/*
[P4254 [JSOI2008] Blue Mary 开公司](https://www.luogu.com.cn/problem/P4254)
*/
/**
 * 显然本题为李超线段树模板题
 */

using Tree = OY::LichaoSlopeZkwTree<double>;
int main() {
    uint32_t n;
    cin >> n;
    Tree S(65536);
    while (n--) {
        std::string op;
        cin >> op;
        if (op[0] == 'P') {
            double s, p;
            cin >> s >> p;
            S.add(0, 65535, {p, s});
        } else {
            double t;
            cin >> t;
            cout << int(S.query(t - 1).calc(t - 1) / 100) << endl;
        }
    }
}