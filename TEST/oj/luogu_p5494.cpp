#include "DS/FHQCounter.h"
#include "DS/SegCounter.h"
#include "DS/SegTree.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
*/
/**
 * 本题为线段树分裂/合并模板题
 * 涉及到分裂合并，只能使用动态开点版本，不能使用 Zkw
 */

static constexpr uint32_t N = 200000, M = 200000;
void solve_counter() {
    uint32_t n, m, cnt = 2;
    cin >> n >> m;
    using Counter = OY::SEGCNT::Table<uint32_t, uint64_t, true, false, false, OY::StaticBufferWrapWithCollect<(N + M) << 1>::type>;
    // using Counter = OY::FHQCNT::Table<uint32_t, uint64_t, true, false, OY::StaticBufferWrapWithCollect<N + M>::type>;
    std::vector<Counter> counter_pool(m + 2);
    for (uint32_t i = 0; i != n; i++) {
        uint64_t x;
        cin >> x;
        counter_pool[1].add(i, x);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') { // 加新
            uint32_t p, x, y;
            cin >> p >> x >> y;
            auto S3 = counter_pool[p].split_by_key(y);
            counter_pool[cnt++] = counter_pool[p].split_by_key(x - 1);
            counter_pool[p].merge(S3);
        } else if (op == '1') { // 合并
            uint32_t p, t;
            cin >> p >> t;
            counter_pool[p].merge(counter_pool[t]);
        } else if (op == '2') { // 添加
            uint32_t p, q;
            uint64_t x;
            cin >> p >> x >> q;
            counter_pool[p].add(q - 1, x);
        } else if (op == '3') { // 查询
            uint32_t p, x, y;
            cin >> p >> x >> y;
            cout << counter_pool[p].query(x - 1, y - 1) << endl;
        } else { // 查询
            uint32_t p;
            uint64_t k;
            cin >> p >> k;
            if (counter_pool[p].query_all() < k)
                cout << "-1\n";
            else
                cout << counter_pool[p].kth(k - 1)->key() + 1 << '\n';
        }
    }
}

using Seg = OY::Seg::Tree<OY::Seg::BaseNode<uint64_t>, OY::Seg::Ignore, false, uint32_t, OY::StaticBufferWrapWithCollect<1 << 22>::type>;
Seg seg_pool[M + 2];
void solve_seg() {
    uint32_t n, m, cnt = 2;
    cin >> n >> m;
    auto read = [](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    };
    seg_pool[1].resize(n, read);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') { // 加新
            uint32_t p, x, y;
            cin >> p >> x >> y;
            auto S3 = seg_pool[p].split_by_key(y);
            seg_pool[cnt++] = seg_pool[p].split_by_key(x - 1);
            seg_pool[p].merge(S3);
        } else if (op == '1') { // 合并
            uint32_t p, t;
            cin >> p >> t;
            seg_pool[p].merge(seg_pool[t]);
        } else if (op == '2') { // 添加
            uint32_t p, q;
            uint64_t x;
            cin >> p >> x >> q;
            seg_pool[p].add(q - 1, x);
        } else if (op == '3') { // 查询
            uint32_t p, x, y;
            cin >> p >> x >> y;
            cout << seg_pool[p].query(x - 1, y - 1) << endl;
        } else { // 查询
            uint32_t p;
            uint64_t k;
            cin >> p >> k;
            if (seg_pool[p].query_all() < k)
                cout << "-1\n";
            else
                cout << seg_pool[p].kth(k - 1).m_index + 1 << '\n';
        }
    }
}

int main() {
    solve_counter();
    // solve_seg();
}