#include "DS/CompressedTree.h"
#include "DS/Deque.h"
#include "IO/FastIO.h"

/*
[P3088 [USACO13NOV] Crowded Cows S](https://www.luogu.com.cn/problem/P3088)
*/

static constexpr uint32_t N = 50000;
void solve_window() {
    uint64_t n, d;
    cin >> n >> d;

    struct Cow {
        uint64_t pos, h;
    };
    std::vector<Cow> cows(n);
    for (auto &[pos, h] : cows) cin >> pos >> h;
    std::ranges::sort(cows, [](auto &x, auto &y) { return x.pos < y.pos; });

    std::vector<bool> bad(n);
    OY::GlobalDeque<OY::DEQ::NoOp<Cow>, N * 2> q;
    for (uint32_t i = 0; i != n; i++) {
        auto [pos, h] = cows[i];
        while (!q.empty() && q.back().h <= h) q.pop_back();
        q.push_back(cows[i]);
        while (q.front().pos + d < pos) q.pop_front();
        if (q.front().h >= h * 2) bad[i] = true;
    }

    q = OY::GlobalDeque<OY::DEQ::NoOp<Cow>, N * 2>();
    uint32_t ans = 0;
    for (uint32_t i = n - 1; ~i; i--) {
        auto [pos, h] = cows[i];
        while (!q.empty() && q.back().h <= h) q.pop_back();
        q.push_back(cows[i]);
        while (q.front().pos > pos + d) q.pop_front();
        if (q.front().h >= h * 2 && bad[i]) ans++;
    }

    cout << ans;
}

void solve_ds() {
    uint32_t n, d;
    cin >> n >> d;

    struct Cow {
        uint32_t pos, h;
    };
    std::vector<Cow> cows(n);
    for (auto &[pos, h] : cows) cin >> pos >> h;

    using Tree = OY::VectorCompressedMaxTree<uint32_t, 0, uint32_t>;
    Tree::_reserve(200000);
    Tree S;
    for (auto &[pos, h] : cows) S.add(pos, h);

    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        auto [pos, h] = cows[i];
        if (S.query(pos >= d ? pos - d : 0, pos - 1) >= h * 2)
            if (S.query(pos + 1, pos + d) >= h * 2) ans++;
    }
    cout << ans;
}

int main() {
    solve_window();
    // solve_ds();s
}