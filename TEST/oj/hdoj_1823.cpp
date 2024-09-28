#include "DS/MultiDimSegTree.h"
#include "DS/MonoZkwTree.h"
#include "DS/MonoZkwTree2D.h"
#include "IO/FastIO.h"

/*
[Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
*/
/**
 * 本题为二维线段树模板题，单点修改+区域查询
 */

static constexpr uint32_t M = 101, N = 1001;
void solve_zkw2d() {
    cout.precision(1);
    uint32_t q;
    while (true) {
        cin >> q;
        if (!q) break;
        OY::MonoMaxTree2D<double, 0.> S(M, N);
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
                    cout << res << endl;
                else
                    cout << "-1\n";
            }
        }
    }
}

void solve_mdseg() {
    uint32_t q;
    while (true) {
        cin >> q;
        if (!q) break;
        // 由于题目说浮点数只有一位小数，所以干脆乘十，避免浮点数运算
        using base_table = OY::MonoMaxTree<uint32_t, 0>;
        OY::MonoMaxMDSeg<uint32_t, base_table, 2, true> S(M * N);
        for (uint32_t i = 0; i != M; i++)
            for (uint32_t j = 0; j != N; j++) S.add_point(0, i, j);
        S.prepare();
        auto read_double_mul_10 = [] {
            std::string num;
            cin >> num;
            if (num.find('.') == num.npos) {
                uint32_t x{};
                for (char c : num) x = x * 10 + (c - '0');
                return x * 10;
            } else {
                uint32_t x{};
                for (char c : num)
                    if (c != '.') x = x * 10 + (c - '0');
                return x;
            }
        };
        auto print_double_div_10 = [](uint32_t x) {
            if (!x)
                cout << "-1\n";
            else
                cout << x / 10 << '.' << x % 10 << endl;
        };
        while (q--) {
            char op;
            cin >> op;
            if (op == 'I') {
                uint32_t H;
                cin >> H;
                uint32_t A = read_double_mul_10(), L = read_double_mul_10();
                S.do_in_table((H - 100) * N + A, [L](base_table &tr, uint32_t pos) {
                    tr.add(pos, L);
                });
            } else {
                uint32_t H1, H2;
                cin >> H1 >> H2;
                uint32_t A1 = read_double_mul_10(), A2 = read_double_mul_10();
                if (H1 > H2) std::swap(H1, H2);
                if (A1 > A2) std::swap(A1, A2);
                print_double_div_10(S.query(H1 - 100, H2 - 100, A1, A2));
            }
        }
    }
}

int main() {
    solve_zkw2d();
    // solve_mdseg();
}