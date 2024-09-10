#include "DS/Discretizer.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P3201 [HNOI2009] 梦幻布丁](https://www.luogu.com.cn/problem/P3201)
*/
/**
 * 本题需要维护区间连续段数
 * 颜色数有点多，离线一下
 */

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<uint32_t> color(n);
    for (auto &a : color) cin >> a;
    struct Operation {
        uint32_t from, to;
    };
    std::vector<Operation> ops(m);
    uint32_t qcnt = 0;
    for (uint32_t i = 0; i != m; i++) {
        char c;
        cin >> c;
        if (c == '1')
            cin >> ops[i].from >> ops[i].to;
        else
            qcnt++;
    }

    OY::Discretizer<uint32_t> D;
    D.reserve(n + qcnt);
    D.assign(color.begin(), color.end());
    for (uint32_t i = 0; i != m; i++)
        if (ops[i].from) D << ops[i].from << ops[i].to;
    D.prepare();

    std::vector<OY::RangeManager<uint32_t>> rms(D.size());
    uint32_t tot{};
    auto add_call = [&](auto...) { tot++; };
    auto remove_call = [&](auto...) { tot--; };
    for (uint32_t i = 0; i != n; i++) rms[D.rank(color[i])].add_range({i, i}, add_call, remove_call);
    for (uint32_t i = 0; i != m; i++) {
        if (ops[i].from) {
            if (ops[i].from == ops[i].to) continue;
            auto &v1 = rms[D.rank(ops[i].from)], &v2 = rms[D.rank(ops[i].to)];
            if (v1.size() > v2.size()) std::swap(v1, v2);
            for (auto &&pair : v1) v2.add_range(pair, add_call, remove_call);
            tot -= v1.size(), v1.clear();
        } else
            cout << tot << endl;
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<uint32_t> color(n);
    for (auto &a : color) cin >> a;
    struct Operation {
        uint32_t from, to;
    };
    std::vector<Operation> ops(m);
    uint32_t qcnt = 0;
    for (uint32_t i = 0; i != m; i++) {
        char c;
        cin >> c;
        if (c == '1')
            cin >> ops[i].from >> ops[i].to;
        else
            qcnt++;
    }

    OY::Discretizer<uint32_t> D;
    D.reserve(n + qcnt);
    D.assign(color.begin(), color.end());
    for (uint32_t i = 0; i != m; i++)
        if (ops[i].from) D << ops[i].from << ops[i].to;
    D.prepare();

    std::vector<OY::VectorLazyBitset<uint32_t, false>> rms(D.size());
    for (uint32_t i = 0; i != rms.size(); i++) rms[i].resize(n);
    uint32_t tot{};
    for (uint32_t l = 0, r; l != n; l = r) {
        for (r = l + 1; r != n && color[r] == color[l]; r++) {}
        rms[D.rank(color[l])].set(l, r - 1);
        tot++;
    }
    for (uint32_t i = 0; i != m; i++) {
        if (ops[i].from) {
            if (ops[i].from == ops[i].to) continue;
            auto &v1 = rms[D.rank(ops[i].from)], &v2 = rms[D.rank(ops[i].to)];
            if (v1.count() > v2.count()) std::swap(v1, v2);
            v1.enumerate_range([&](uint32_t l, uint32_t r) {
                if (l && v2[l - 1]) tot--;
                if (r + 1 < n && v2[r + 1]) tot--;
            });
            v2 |= v1;
        } else
            cout << tot << endl;
    }
}

int main() {
    solve_rm();
    // solve_bitset();
}