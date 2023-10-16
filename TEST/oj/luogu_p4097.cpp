#include "DS/LichaoZkwTree.h"
#include "DS/LichaoSegTree.h"
#include "IO/FastIO.h"

/*
[P4097 【模板】李超线段树 / [HEOI2013] Segment](https://www.luogu.com.cn/problem/P4097)
*/
/**
 * 本题为李超线段树模板题
*/
int main() {
    static constexpr int P = 39989;
    static constexpr int P2 = 1000000000;
    int n;
    cin >> n;
    struct Line {
        double k, b;
        int index;
        Line() : k(0), b(-1000000000), index(0) {}
        Line(double _k, double _b, int _index) : k(_k), b(_b), index(_index) {}
        double calc(int i) const { return k * i + b; }
    };
    struct Cmp {
        bool operator()(const Line &x, const Line &y, int i) const {
            auto xx = x.calc(i), yy = y.calc(i);
            if (std::abs(xx - yy) > 1e-10)
                return xx < yy;
            else
                return x.index > y.index;
        }
    };
    OY::LichaoZkw::Tree<Line, Cmp> T(P + 1);
    // OY::LichaoSeg::Tree<Line, Cmp> T(P + 1);
    int lastans = 0;
    int index = 0;
    for (int i = 1; i <= n; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            int k;
            cin >> k;
            k = (k + lastans - 1) % P + 1;
            lastans = T.query(k).index;
            cout << lastans << endl;
        } else {
            int x0, y0, x1, y1;
            cin >> x0 >> y0 >> x1 >> y1;
            x0 = (x0 + lastans - 1) % P + 1;
            y0 = (y0 + lastans - 1) % P2 + 1;
            x1 = (x1 + lastans - 1) % P + 1;
            y1 = (y1 + lastans - 1) % P2 + 1;
            if (x0 == x1) {
                T.add(x0, x1, Line(0, std::max(y0, y1), ++index));
            } else {
                if (x0 > x1) std::swap(x0, x1), std::swap(y0, y1);
                double k = 1.0 * (y1 - y0) / (x1 - x0);
                double b = y0 - k * x0;
                T.add(x0, x1, {k, b, ++index});
            }
        }
    }
}