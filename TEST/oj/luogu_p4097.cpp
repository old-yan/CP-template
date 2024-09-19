#include "DS/LichaoSegTree.h"
#include "DS/LichaoZkwTree.h"
#include "IO/FastIO.h"

/*
[P4097 【模板】李超线段树 / [HEOI2013] Segment](https://www.luogu.com.cn/problem/P4097)
*/
/**
 * 本题为李超线段树模板题
 */
int main() {
    static constexpr uint32_t P = 39989;
    static constexpr uint32_t P2 = 1000000000;
    static constexpr double eps = 1e-10;
    uint32_t n;
    cin >> n;
    struct Value {
        double m_y;
        uint32_t m_index;
        bool operator<(const Value &rhs) const {
            return m_y + eps < rhs.m_y || (m_y < rhs.m_y + eps && m_index > rhs.m_index);
        }
    };
    struct Line {
        double k, b;
        uint32_t index;
        Line() : k(0), b(-1000000000), index(0) {}
        Line(double _k, double _b, uint32_t _index) : k(_k), b(_b), index(_index) {}
        Value calc(uint32_t i) const { return {k * i + b, index}; }
    };
    OY::LCZKW::Tree<Line> T(P + 1);
    // OY::LCSEG::Tree<Line, std::less<>, uint32_t> T(P + 1);
    uint32_t lastans = 0;
    uint32_t index = 0;
    for (uint32_t i = 1; i <= n; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t k;
            cin >> k;
            k = (k + lastans - 1) % P + 1;
            lastans = T.query(k).index;
            cout << lastans << endl;
        } else {
            uint32_t x0, y0, x1, y1;
            cin >> x0 >> y0 >> x1 >> y1;
            x0 = (x0 + lastans - 1) % P + 1;
            y0 = (y0 + lastans - 1) % P2 + 1;
            x1 = (x1 + lastans - 1) % P + 1;
            y1 = (y1 + lastans - 1) % P2 + 1;
            if (x0 == x1) {
                T.add(x0, x1, Line(0, std::max(y0, y1), ++index));
            } else {
                if (x0 > x1) std::swap(x0, x1), std::swap(y0, y1);
                double k = 1.0 * int(y1 - y0) / int(x1 - x0);
                double b = y0 - k * x0;
                T.add(x0, x1, {k, b, ++index});
            }
        }
    }
}