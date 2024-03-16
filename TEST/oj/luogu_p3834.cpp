#include "DS/DivideTree.h"
#include "DS/PersistentSegTree.h"
#include "DS/SegBIT.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
*/
/**
 * 本题类似 P2617 P3380，但是不带修改
 * 因为不带修改，所以不需要使用二维线段树再进行多行查询
 * 如果每一行都是在上一行的基础上，添加一个数字，那么查询就可以只针对单行
 * 这样就可以使用可持久化线段树
 *
 * 然而，本题的问题，使用划分树或者小波树最佳
 */

void solve_ds() {
    // 使用小波树或者划分树，可以直接解决本问题
    uint32_t n, m;
    cin >> n >> m;
    auto S = OY::WaveLet::Table<uint32_t>(
        n, [](auto...) {
            uint32_t x;
            cin >> x;
            return x;
        });
    // auto S = OY::make_DivideTree<uint32_t, std::less<uint32_t>, 1 << 24>(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::WaveLet::Tree<uint32_t>(
    //     n, [](auto...) {
    //         uint32_t x;
    //         cin >> x;
    //         return x;
    //     });
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << S.quantile(l - 1, r - 1, k - 1) << endl;
    }
}

using PerSeg = OY::PerSeg::Tree<OY::PerSeg::BaseNode<uint32_t>, OY::PerSeg::Ignore, false, true, uint32_t, 1 << 25>;
PerSeg seg_pool[1000001];
void solve_perseg() {
    uint32_t n, m;
    cin >> n >> m;
    seg_pool->unlock();
    seg_pool[0].resize(1000000001);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        seg_pool[i] = seg_pool[i - 1].copy();
        seg_pool[i].add(x, 1);
    }
    seg_pool->lock();
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << PerSeg::reduce_kth(seg_pool[l - 1], seg_pool[r], k - 1) << endl;
    }
}

void solve_segbit() {
    using SegBIT = OY::SegBIT::Tree<OY::SegBIT::BaseNode<uint32_t>, OY::SegBIT::Ignore, false, uint32_t, 200001, 20000000>;
    static constexpr uint32_t M = 1000000000;
    uint32_t n, m;
    cin >> n >> m;
    SegBIT S(n + 1, M * 2 + 1);
    for (uint32_t i = 1; i <= n; i++) {
        int x;
        cin >> x;
        S.add(i, x + M, 1);
    }
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << int(S.kth(l, r, k - 1) - M) << endl;
    }
}

int main() {
    solve_ds();
    // solve_perseg();
    // solve_segbit();
}