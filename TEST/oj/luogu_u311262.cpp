#include "DS/MergeSortTree.h"
#include "DS/CompressedTree.h"
#include "DS/MonoZkwTree.h"
#include "DS/WaveLet.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[U311262 求区间后继](https://www.luogu.com.cn/problem/U311262)
*/
/**
 * 显然本题为类可持久化线段树模板题
 * 也可以通过离线解决
 */

static constexpr uint32_t N = 1000000;
void solve_offline() {
    uint32_t n, q;
    cin >> n >> q;
    struct pair {
        uint32_t val, idx;
    };
    std::vector<pair> arr(n + q);
    std::vector<pair> qs(q);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i].val, arr[i].idx = i;
    for (uint32_t i = 0; i != q; i++) {
        auto &[l, r] = qs[i];
        uint32_t v;
        cin >> l >> r >> v;
        l--, r--, arr[n + i].val = v, arr[n + i].idx = n + i;
    }
    std::sort(arr.begin(), arr.end(), [](auto &x, auto &y) {
        if (x.val != y.val)
            return x.val > y.val;
        else
            return x.idx > y.idx;
    });

    auto S = OY::MonoMinTree<uint32_t, 0x3f3f3f3f>(n);
    // auto S = OY::StaticCompressedMinTree<uint32_t, 0x3f3f3f3f, uint32_t, N * 2>();
    // auto S = OY::make_ZkwTree<uint32_t>(n, [](uint32_t x, uint32_t y) { return x < y ? x : y; }, [](auto...) { return 0x3f3f3f3f; });
    std::vector<uint32_t> ans(q);
    for (auto &[val, idx] : arr) {
        if (idx < n)
            S.modify(idx, val);
        else {
            auto &&[l, r] = qs[idx - n];
            ans[idx - n] = S.query(l, r);
        }
    }
    for (auto a : ans)
        if (a == 0x3f3f3f3f)
            cout << "-1\n";
        else
            cout << a << endl;
}

void solve_ds() {
    uint32_t n, q;
    cin >> n >> q;
    OY::MS::Tree<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    // OY::WaveLet::Tree<uint32_t> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::WaveLet::Table<uint32_t> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    auto mx = S.maximum(0, n - 1);
    while (q--) {
        uint32_t l, r, v, rk;
        cin >> l >> r >> v;
        if (v >= mx || (rk = S.rank(l - 1, r - 1, v + 1)) == r - l + 1)
            cout << "-1\n";
        else
            cout << S.quantile(l - 1, r - 1, rk) << endl;
    }
}

int main() {
    solve_offline();
    // solve_ds();
}