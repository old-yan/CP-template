#include "DS/BIT2D.h"
#include "DS/SegBIT.h"
#include "DS/SegTree2D.h"
#include "DS/GlobalHashBIT2D.h"
#include "IO/FastIO.h"

/*
[P4054 [JSOI2009] 计数问题](https://www.luogu.com.cn/problem/P4054)
*/
/**
 * 本题可以通过建立一百个二维树状数组来解决
 */

uint8_t color[300][300];
void solve_bit2d() {
    uint32_t m, n;
    cin >> m >> n;
    using bit2d = OY::BIT2D::Tree<uint32_t, false, 100 << 18>;
    std::vector<bit2d> grid(100);
    for (uint32_t i = 0; i < 100; i++) grid[i].resize(m, n);
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            uint32_t c;
            cin >> c;
            color[i][j] = c - 1;
            grid[c - 1].add(i, j, 1);
        }
    }

    uint32_t q;
    cin >> q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y, c;
            cin >> x >> y >> c;
            grid[color[x - 1][y - 1]].add(x - 1, y - 1, -1);
            grid[color[x - 1][y - 1] = c - 1].add(x - 1, y - 1, 1);
        } else {
            uint32_t x1, x2, y1, y2, c;
            cin >> x1 >> x2 >> y1 >> y2 >> c;
            cout << grid[c - 1].query(x1 - 1, x2 - 1, y1 - 1, y2 - 1) << endl;
        }
    }
}

void solve_seg2d() {
    uint32_t m, n;
    cin >> m >> n;
    using seg2d = OY::SegBIT::Tree<OY::SegBIT::BaseNode<uint32_t>, OY::SegBIT::Ignore, false, uint16_t, 60000, 1 << 23>;
    // using seg2d = OY::Seg2D::Tree<OY::Seg2D::BaseNode<uint32_t>, OY::Seg2D::Ignore, false, uint16_t, 60000, 1 << 23>;
    std::vector<seg2d> grid(100);
    for (uint32_t i = 0; i < 100; i++) grid[i].resize(m, n);
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            uint32_t c;
            cin >> c;
            color[i][j] = c - 1;
            grid[c - 1].add(i, j, 1);
        }
    }

    uint32_t q;
    cin >> q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y, c;
            cin >> x >> y >> c;
            grid[color[x - 1][y - 1]].add(x - 1, y - 1, -1);
            grid[color[x - 1][y - 1] = c - 1].add(x - 1, y - 1, 1);
        } else {
            uint32_t x1, x2, y1, y2, c;
            cin >> x1 >> x2 >> y1 >> y2 >> c;
            cout << grid[c - 1].query(x1 - 1, x2 - 1, y1 - 1, y2 - 1) << endl;
        }
    }
}

uint8_t color[300][300];
// 一般不推荐开这么多的 GlobalHashBIT2D，但是此处经尝试空间占用尚可接受
OY::GHashBIT2D<uint32_t, uint32_t, false, false, 16> GS[100];
void solve_hashbit2d() {
    uint32_t m, n;
    cin >> m >> n;
    for (uint32_t i = 0; i < 100; i++) GS[i].resize(m, n);
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            uint32_t c;
            cin >> c;
            color[i][j] = c - 1;
            GS[c - 1].add(i, j, 1);
        }
    }

    uint32_t q;
    cin >> q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y, c;
            cin >> x >> y >> c;
            GS[color[x - 1][y - 1]].add(x - 1, y - 1, -1);
            GS[color[x - 1][y - 1] = c - 1].add(x - 1, y - 1, 1);
        } else {
            uint32_t x1, x2, y1, y2, c;
            cin >> x1 >> x2 >> y1 >> y2 >> c;
            cout << GS[c - 1].query(x1 - 1, x2 - 1, y1 - 1, y2 - 1) << endl;
        }
    }
}

int main() {
    solve_bit2d();
    // solve_seg2d();
    // solve_hashbit2d();
}