#include "DS/BIT2D.h"
#include "DS/GlobalHashBIT2D.h"
#include "DS/OfflineRectAddPointSumMaintainer2D.h"
#include "DS/RectAddPointSumCounter2D.h"
#include "DS/RectAddPointSumMaintainer2D.h"
#include "IO/FastIO.h"

/*
[#134. 二维树状数组 2：区间修改，单点查询](https://loj.ac/p/134)
*/
/**
 * 本题为二维树状数组模板题
 */

void solve_bit2d() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT2D::Tree<int64_t, false> S(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            auto f = [&](uint32_t v1, uint32_t v2) { return (v1 | (std::bit_ceil((v1 ^ v2) + 1) - 1)); };
            const uint32_t rend = f(r1 - 1, r2), cend = f(c1 - 1, c2);
            for (uint32_t r = r1 - 1; r != rend && r < n; r += S._lowbit(r + 1)) {
                uint32_t cursor = r * m;
                for (uint32_t c = c1 - 1; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] += v;
                for (uint32_t c = c2; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] -= v;
            }
            for (uint32_t r = r2; r != rend && r < n; r += S._lowbit(r + 1)) {
                uint32_t cursor = r * m;
                for (uint32_t c = c1 - 1; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] -= v;
                for (uint32_t c = c2; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] += v;
            }
        } else {
            uint32_t r, c;
            cin >> r >> c;
            cout << S.presum(r - 1, c - 1) << endl;
        }
    }
}

OY::GHashBIT2D<uint32_t, int64_t, false, false, 4194319> GS;
void solve_hashbit2d() {
    uint32_t n, m;
    cin >> n >> m;
    GS.resize(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            auto f = [&](uint32_t v1, uint32_t v2) { return (v1 | (std::bit_ceil((v1 ^ v2) + 1) - 1)); };
            const uint32_t rend = f(r1 - 1, r2), cend = f(c1 - 1, c2);
            for (uint32_t r = r1 - 1; r != rend && r < n; r += GS._lowbit(r + 1)) {
                for (uint32_t c = c1 - 1; c != cend && c < m; c += GS._lowbit(c + 1)) GS.m_map.insert({r, c}).m_ptr->m_mapped += v;
                for (uint32_t c = c2; c != cend && c < m; c += GS._lowbit(c + 1)) GS.m_map.insert({r, c}).m_ptr->m_mapped -= v;
            }
            for (uint32_t r = r2; r != rend && r < n; r += GS._lowbit(r + 1)) {
                for (uint32_t c = c1 - 1; c != cend && c < m; c += GS._lowbit(c + 1)) GS.m_map.insert({r, c}).m_ptr->m_mapped -= v;
                for (uint32_t c = c2; c != cend && c < m; c += GS._lowbit(c + 1)) GS.m_map.insert({r, c}).m_ptr->m_mapped += v;
            }
        } else {
            uint32_t r, c;
            cin >> r >> c;
            cout << GS.presum(r - 1, c - 1) << endl;
        }
    }
}

void solve_OFFLINERAPSM2D() {
    uint32_t n, m;
    cin >> n >> m;
    OY::OFFLINERAPSM2D::Solver<uint32_t, int> S;
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            S.add_rect(r1, r2, c1, c2, v);
        } else {
            uint32_t r, c;
            cin >> r >> c;
            S.add_query(r, c);
        }
    }
    for (auto a : S.solve<int64_t>()) cout << a << endl;
}

void solve_RAPSM2D() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RAPSM2D::Table<uint32_t, int, int64_t, 128> S;
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            S.add_rect(r1, r2, c1, c2, v);
        } else {
            uint32_t r, c;
            cin >> r >> c;
            cout << S.query(r, c) << endl;
        }
    }
}

void solve_RAPSC2D() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RAPSC2D::Table<uint32_t, int, int64_t, true> S;
    struct event {
        bool isq;
        union {
            struct {
                int val;
            };
            struct {
                uint32_t r, c;
            };
        };
    };
    std::vector<event> es;
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            S.add_rect(r1, r2, c1, c2, 0);
            event e;
            e.isq = false, e.val = v;
            es.push_back(e);
        } else {
            uint32_t r, c;
            cin >> r >> c;
            event e;
            e.isq = true, e.r = r, e.c = c;
            es.push_back(e);
        }
    }
    S.prepare();
    uint32_t id = 0;
    for (auto &e : es)
        if (e.isq)
            cout << S.query(e.r, e.c) << endl;
        else
            S.add_rect_value(id++, e.val);
}

int main() {
    solve_bit2d();
    // solve_hashbit2d();
    // solve_OFFLINERAPSM2D();
    // solve_RAPSM2D();
    // solve_RAPSC2D();
}