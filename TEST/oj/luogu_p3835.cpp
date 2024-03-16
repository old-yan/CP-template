#include "DS/PersistentFHQTreap.h"
#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

/*
[P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
*/
/**
 * 本题为 P3369 可持久化版本
 * 为可持久化平衡树模板题
 */

using PerFHQ = OY::PerFHQTreap<int, std::less<int>, true, 20000000>;
PerFHQ fhq_pool[500001];
void solve_perfhq() {
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            fhq_pool->unlock();
            fhq_pool[i] = fhq_pool[v].copy();
            fhq_pool[i].insert_by_key(x);
            fhq_pool->lock();
        } else if (opt == '2') {
            fhq_pool->unlock();
            fhq_pool[i] = fhq_pool[v].copy();
            fhq_pool[i].erase_by_key(x);
            fhq_pool->lock();
        } else {
            fhq_pool[i] = fhq_pool[v];
            int res = 0;
            if (opt == '3')
                res = fhq_pool[i].rank(x) + 1;
            else if (opt == '4')
                res = fhq_pool[i].kth(x - 1)->get();
            else if (opt == '5') {
                auto p = fhq_pool[i].smaller_bound(x);
                res = p->is_null() ? -2147483647 : p->get();
            } else {
                auto p = fhq_pool[i].upper_bound(x);
                res = p->is_null() ? 2147483647 : p->get();
            }
            cout << res << endl;
        }
    }
}

using PerSeg = OY::PerSeg::Tree<OY::PerSeg::BaseNode<uint32_t>, OY::PerSeg::Ignore, false, false, uint32_t, 40000000>;
PerSeg seg_pool[500001];
void solve_perseg() {
    static constexpr int M = 1000000001;
    uint32_t n;
    cin >> n;
    int ans = 0;
    seg_pool[0].resize(M * 2 + 1);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            seg_pool->unlock();
            seg_pool[i] = seg_pool[v].copy();
            seg_pool[i].add(x + M, 1);
            seg_pool->lock();
        } else if (opt == '2') {
            auto val = seg_pool[v].query(x + M);
            if (val) {
                seg_pool->unlock();
                seg_pool[i] = seg_pool[v].copy();
                seg_pool[i].add(x + M, -1);
                seg_pool->lock();
            } else
                seg_pool[i] = seg_pool[v];
        } else {
            seg_pool[i] = seg_pool[v];
            int res = 0;
            if (opt == '3') {
                res = seg_pool[i].query(0, x + M - 1) + 1;
            } else if (opt == '4') {
                res = seg_pool[i].kth(x - 1) - M;
            } else if (opt == '5') {
                uint32_t rank = seg_pool[i].query(0, x + M - 1);
                res = rank ? seg_pool[i].kth(rank - 1) - M : -2147483647;
            } else {
                uint32_t rank = seg_pool[i].query(0, x + M);
                res = rank < seg_pool[i].query_all() ? seg_pool[i].kth(rank) - M : 2147483647;
            }
            cout << res << endl;
        }
    }
}

int main() {
    solve_perfhq();
    // solve_perseg();
}