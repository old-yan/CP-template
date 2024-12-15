#include "DS/ColorManager.h"
#include "DS/LazyBitset.h"
#include "DS/LinkBucket.h"
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P3201 [HNOI2009] 梦幻布丁](https://www.luogu.com.cn/problem/P3201)
*/
/**
 * 本题需要维护区间连续段数
 * 颜色数有点多，离线一下
 */

uint32_t color_id[1000001];
void solve_cm() {
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

    std::vector<uint32_t> all_colors;
    all_colors.reserve(n + qcnt);
    all_colors.assign(color.begin(), color.end());
    for (uint32_t i = 0; i != m; i++)
        if (ops[i].from) all_colors.push_back(ops[i].from), all_colors.push_back(ops[i].to);
    std::ranges::sort(all_colors);
    uint32_t id = 0;
    for (uint32_t i = 0; i != all_colors.size(); i++) {
        if (i && all_colors[i] != all_colors[i - 1]) id++;
        color_id[all_colors[i]] = id;
    }

    OY::LBC::LinkBucket<uint32_t> pos(id + 1, n);
    std::vector<uint32_t> buc_size(id + 1);
    OY::CM::Table C(id + 1);
    for (uint32_t i = 0; i != n; i++) pos[color_id[color[i]]].push_front(i), buc_size[color_id[color[i]]]++;

    uint32_t gap = 0;
    for (uint32_t i = 1; i != n; i++) gap += color[i] != color[i - 1];
    for (uint32_t i = 0; i != m; i++) {
        if (ops[i].from) {
            if (ops[i].from == ops[i].to) continue;
            uint32_t x = color_id[ops[i].from], y = color_id[ops[i].to];
            if (!buc_size[x]) continue;
            if (!buc_size[y])
                pos.swap(x, y);
            else if (buc_size[y] < buc_size[x]) {
                for (int i : pos[y]) {
                    if (i && C.query(color_id[color[i - 1]]) == x) gap--;
                    if (i + 1 != n && C.query(color_id[color[i + 1]]) == x) gap--;
                }
                pos.merge(x, y), pos.swap(x, y);
            } else {
                for (int i : pos[x]) {
                    if (i && C.query(color_id[color[i - 1]]) == y) gap--;
                    if (i + 1 != n && C.query(color_id[color[i + 1]]) == y) gap--;
                }
                pos.merge(y, x);
            }
            C.modify(x, y);
            buc_size[y] += buc_size[x], buc_size[x] = 0;
        } else
            cout << gap + 1 << endl;
    }
}

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

    std::vector<uint32_t> all_colors;
    all_colors.reserve(n + qcnt);
    all_colors.assign(color.begin(), color.end());
    for (uint32_t i = 0; i != m; i++)
        if (ops[i].from) all_colors.push_back(ops[i].from), all_colors.push_back(ops[i].to);
    std::ranges::sort(all_colors);
    uint32_t id = 0;
    for (uint32_t i = 0; i != all_colors.size(); i++) {
        if (i && all_colors[i] != all_colors[i - 1]) id++;
        color_id[all_colors[i]] = id;
    }

    std::vector<OY::RangeManager<uint32_t>> rms(id + 1);
    uint32_t tot{};
    auto add_call = [&](auto...) { tot++; };
    auto remove_call = [&](auto...) { tot--; };
    for (uint32_t i = 0; i != n; i++) rms[color_id[color[i]]].add_range({i, i}, add_call, remove_call);
    for (uint32_t i = 0; i != m; i++) {
        if (ops[i].from) {
            if (ops[i].from == ops[i].to) continue;
            auto &v1 = rms[color_id[ops[i].from]], &v2 = rms[color_id[ops[i].to]];
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

    std::vector<uint32_t> all_colors;
    all_colors.reserve(n + qcnt);
    all_colors.assign(color.begin(), color.end());
    for (uint32_t i = 0; i != m; i++)
        if (ops[i].from) all_colors.push_back(ops[i].from), all_colors.push_back(ops[i].to);
    std::ranges::sort(all_colors);
    uint32_t id = 0;
    for (uint32_t i = 0; i != all_colors.size(); i++) {
        if (i && all_colors[i] != all_colors[i - 1]) id++;
        color_id[all_colors[i]] = id;
    }

    std::vector<OY::VectorLazyBitset<uint32_t, false>> rms(id + 1);
    for (uint32_t i = 0; i != rms.size(); i++) rms[i].resize(n);
    uint32_t tot{};
    for (uint32_t l = 0, r; l != n; l = r) {
        for (r = l + 1; r != n && color[r] == color[l]; r++) {}
        rms[color_id[color[l]]].set(l, r - 1);
        tot++;
    }
    for (uint32_t i = 0; i != m; i++) {
        if (ops[i].from) {
            if (ops[i].from == ops[i].to) continue;
            auto &v1 = rms[color_id[ops[i].from]], &v2 = rms[color_id[ops[i].to]];
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
    solve_cm();
    // solve_rm();
    // solve_bitset();
}