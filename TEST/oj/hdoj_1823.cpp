#include "DS/MultiDimSegTree.h"
#include "DS/ZkwTree.h"
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
void solve_zkw2d() {
    cout.precision(1);
    uint32_t q;
    while (true) {
        cin >> q;
        if (!q) break;
        OY::ZKW2D::Tree<Node> S(101, 1001);
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

void solve_mdseg() {
    cout.precision(1);
    uint32_t q;
    while (true) {
        cin >> q;
        if (!q) break;
        using base_table = OY::ZKW::Tree<Node>;
        OY::Segtree2D<uint32_t, double, base_table, true> S(101 * 1001);
        for (uint32_t i = 0; i != 101; i++)
            for (uint32_t j = 0; j != 1001; j++) S.add_point(0, i, j);
        S.prepare();
        while (q--) {
            char op;
            cin >> op;
            if (op == 'I') {
                uint32_t H;
                double A, L;
                cin >> H >> A >> L;
                S.do_in_table((H - 100) * 1001 + (A * 10), [L](base_table &tr, uint32_t pos) {
                    tr.add(pos, L);
                });
            } else {
                uint32_t H1, H2;
                double A1, A2;
                cin >> H1 >> H2 >> A1 >> A2;
                if (H1 > H2) std::swap(H1, H2);
                if (A1 > A2) std::swap(A1, A2);
                struct Max {
                    double operator()(double x, double y) const { return x > y ? x : y; }
                };
                auto res = S.query<Max>(0, H1 - 100, H2 - 100, A1 * 10, A2 * 10);
                if (res)
                    cout << res << "\n";
                else
                    cout << "-1\n";
            }
        }
    }
}

int main() {
    solve_zkw2d();
    // solve_mdseg();
}