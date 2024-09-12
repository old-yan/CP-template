#include "DS/MonoZkwTree.h"
#include "DS/MonoZkwTree2D.h"
#include "DS/MultiDimSegTree.h"
#include "IO/FastIO.h"

/*
[Mosaic](https://acm.hdu.edu.cn/showproblem.php?pid=4819)
*/
/**
 * 本题为二维线段树模板题，单点修改+区域查询
 */

static constexpr uint32_t N = 800;
struct MinMax {
    uint32_t m_min, m_max;
    MinMax operator+(const MinMax &rhs) const {
        MinMax res;
        res.m_min = std::min(m_min, rhs.m_min);
        res.m_max = std::max(m_max, rhs.m_max);
        return res;
    }
    bool operator!=(const MinMax &rhs) const {
        return m_min != rhs.m_min || m_max != rhs.m_max;
    }
};
constexpr MinMax id{1000000000, 0};
void solve_zkw2d() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        uint32_t n;
        cin >> n;
        auto read = [](auto...) {
            uint32_t x;
            cin >> x;
            return MinMax{x, x};
        };
        OY::MonoSumTree2D<MinMax, id> S(n, n, read);
        uint32_t q;
        cin >> q;
        cout << "Case #" << i << ":\n";
        while (q--) {
            uint32_t x, y, L;
            cin >> x >> y >> L;
            uint32_t row1 = x >= L / 2 + 1 ? x - L / 2 - 1 : 0;
            uint32_t row2 = x + L / 2 - 1 < n ? x + L / 2 - 1 : n - 1;
            uint32_t column1 = y >= L / 2 + 1 ? y - L / 2 - 1 : 0;
            uint32_t column2 = y + L / 2 - 1 < n ? y + L / 2 - 1 : n - 1;
            auto min_max = S.query(row1, row2, column1, column2);
            uint32_t ave = min_max.m_max + min_max.m_min >> 1;
            S.modify(x - 1, y - 1, {ave, ave});
            cout << ave << endl;
        }
    }
}

void solve_mdseg() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        uint32_t n;
        cin >> n;
        using base_table = OY::MonoSumTree<MinMax, id>;
        using monoid = base_table::group;
        OY::MDSEG::Tree<uint32_t, monoid, base_table, 2, true> S(n * n);
        for (uint32_t i = 0; i < n; i++)
            for (uint32_t j = 0; j < n; j++) {
                uint32_t x;
                cin >> x;
                S.add_point({x, x}, i, j);
            }
        S.prepare();

        uint32_t q;
        cin >> q;
        cout << "Case #" << i << ":\n";
        while (q--) {
            uint32_t x, y, L;
            cin >> x >> y >> L;
            uint32_t row1 = x >= L / 2 + 1 ? x - L / 2 - 1 : 0;
            uint32_t row2 = x + L / 2 - 1 < n ? x + L / 2 - 1 : n - 1;
            uint32_t column1 = y >= L / 2 + 1 ? y - L / 2 - 1 : 0;
            uint32_t column2 = y + L / 2 - 1 < n ? y + L / 2 - 1 : n - 1;
            auto min_max = S.query(row1, row2, column1, column2);
            uint32_t ave = min_max.m_max + min_max.m_min >> 1;
            S.do_in_table((x - 1) * n + (y - 1), [&](base_table &tr, uint32_t pos) {
                tr.modify(pos, {ave, ave});
            });
            cout << ave << endl;
        }
    }
}

int main() {
    solve_zkw2d();
    // solve_mdseg();
}