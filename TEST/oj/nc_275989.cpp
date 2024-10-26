#include "DS/MonoAVL.h"
#include "DS/MonoPairHeap.h"
#include "DS/MonoSplay.h"
#include "IO/FastIO.h"

/*
[旅途的终点](https://ac.nowcoder.com/acm/problem/275989)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/86639/G
 * 要动态维护前 k 小和
 * 可以简单粗暴用平衡树维护半群信息
 */

static constexpr uint32_t N = 200000;
void solve_heap() {
    uint32_t n, k;
    uint64_t m;
    cin >> n >> m >> k;

    using Heap = OY::VectorMonoSumPairHeap<uint64_t, uint64_t, 0, std::greater<>>;
    Heap::_reserve(n + 1);
    Heap h;
    uint64_t tot = 0;
    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint64_t x;
        cin >> x;
        tot += x;
        h.push(x);
        if (i >= k) h.pop();
        if (tot - h.query_all() < m)
            ans = i + 1;
        else
            break;
    }
    cout << ans;
}

uint64_t a[N];
void solve_avl() {
    uint32_t n, k;
    uint64_t m;
    cin >> n >> m >> k;
    for (uint32_t i = 0; i != n; i++) cin >> a[i];

    using AVL = OY::VectorMonoSumAVL<uint64_t, uint64_t, 0, false>;
    // using AVL = OY::VectorMonoSumSplay<uint64_t, uint64_t, 0, false>;
    AVL::_reserve(n + 1);
    AVL tr;
    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        tr.insert_by_comparator(a[i]);
        if (tr.size() <= k)
            ans = i + 1;
        else if (tr.query(0, tr.size() - k - 1) < m)
            ans = i + 1;
        else
            break;
    }
    cout << ans;
}

int main() {
    solve_heap();
    // solve_avl();
}