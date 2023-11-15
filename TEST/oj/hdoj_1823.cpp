#include "DS/ZkwTree2D.h"
#include "IO/FastIO.h"

/*
[Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
*/
/**
 * 本题为二维线段树模板题，单点修改+区域查询
 */

static constexpr uint32_t M = 101, N = 1001;
struct Node {
    using value_type = double;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    value_type m_val;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
};
int main() {
    cout.precision(1);

    uint32_t q;
    while (true) {
        cin >> q;
        if (!q) break;
        OY::Zkw2D::Tree<Node, 256 * 2048> S(101, 1001);
        while (q--) {
            char op;
            cin >> op;
            if (op == 'I') {
                uint32_t H;
                double A, L;
                cin >> H >> A >> L;
                S.add(H - 100, A * 10, L);
            } else {
                uint32_t H1, H2;
                double A1, A2;
                cin >> H1 >> H2 >> A1 >> A2;
                if (H1 > H2) std::swap(H1, H2);
                if (A1 > A2) std::swap(A1, A2);
                auto res = S.query(H1 - 100, H2 - 100, A1 * 10, A2 * 10);
                if (res)
                    cout << res << "\n";
                else
                    cout << "-1\n";
            }
        }
    }
}
