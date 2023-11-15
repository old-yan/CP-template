#include "DS/ZkwTree2D.h"
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
};
struct Node {
    using value_type = MinMax;
    static value_type op(const value_type &x, const value_type &y) {
        return value_type{std::min(x.m_min, y.m_min), std::max(x.m_max, y.m_max)};
    }
    value_type m_val;
    const value_type &get() const { return m_val; }
    void set(uint32_t val) { m_val.m_min = m_val.m_max = val; }
    void set(const value_type &val) { m_val = val; }
};
int main() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        uint32_t n;
        cin >> n;
        OY::Zkw2D::Tree<Node, 1024 * 1024 * 4 * 5> S(n, n, [](auto...) {
            uint32_t x;
            cin >> x;
            return x;
        });
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