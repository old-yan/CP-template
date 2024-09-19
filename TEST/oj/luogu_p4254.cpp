#include "DS/LichaoZkwTree.h"
#include "IO/FastIO.h"

/*
[P4254 [JSOI2008] Blue Mary 开公司](https://www.luogu.com.cn/problem/P4254)
*/
/**
 * 显然本题为李超线段树模板题
 * 涉及到五位小数，直接乘以十万倍，在整数域处理
 */

using Tree = OY::LichaoSlopeChmaxZkwTree<int64_t>;
int main() {
    uint32_t n;
    cin >> n;
    Tree S(65536);
    auto read_double = [](auto...) {
        std::string s;
        cin >> s;
        uint64_t val{};
        for (uint32_t i = s[0] == '-'; i < s.size(); i++)
            if (s[i] != '.') val = val * 10 + s[i] - '0';
        return s[0] == '-' ? -int64_t(val) : val;
    };
    while (n--) {
        std::string op;
        cin >> op;
        if (op[0] == 'P') {
            auto s = read_double(), p = read_double();
            S.add(0, 65535, {p, s});
        } else {
            uint32_t t;
            cin >> t;
            cout << S.query(t - 1).calc(t - 1) / 10000000 << endl;
        }
    }
}