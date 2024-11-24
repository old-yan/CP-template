#include "DS/CatTree.h"
#include "DS/ErasableMinMaxHeap.h"
#include "DS/MaskRMQ.h"
#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "DS/MonoZkwTree.h"
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

#include <limits>

/*
[P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
*/

static constexpr uint32_t N = 1000000;
int arr[N];
void solve_window() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];

    auto mapping = [&](uint32_t i) { return arr[i]; };
    auto callback = [&](uint32_t l, uint32_t r, int val) { cout << val << ' '; };

    OY::WINDOW::solve_min<N>(n, k, mapping, callback);
    cout << endl;
    OY::WINDOW::solve_max<N>(n, k, mapping, callback);
    cout << endl;
}

int Mx[N];
void solve_heap() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::EMMHeap::Heap<int> S;
    for (uint32_t l = 0, r = 0; l + k <= n;) {
        while (r < l + k) S.push(arr[r++]);
        cout << S.top_min() << ' ';
        Mx[l] = S.top_max();
        S.erase(arr[l++]);
    }
    cout << endl;
    for (uint32_t l = 0; l + k <= n; l++) cout << Mx[l] << ' ';
}

void solve_avl() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    using Tree = OY::MonoAVLSequence<int, false>;
    Tree::_reserve(n + 1);
    Tree S;
    for (uint32_t l = 0, r = 0; r < n; l++) {
        while (r < l + k) S.insert_by_comparator(arr[r++]);
        cout << S.query(0) << ' ';
        Mx[l] = S.query(S.size() - 1);
        S.erase_by_comparator(arr[l]);
    }
    cout << endl;
    for (uint32_t l = 0; l <= n - k; l++) cout << Mx[l] << ' ';
}

void solve_splay() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    using Tree = OY::MonoSplaySequence<int, false>;
    Tree::_reserve(k + 1);
    Tree S;
    for (uint32_t l = 0, r = 0; r < n; l++) {
        while (r < l + k) S.insert_by_comparator(arr[r++]);
        cout << S.query(0) << ' ';
        Mx[l] = S.query(S.size() - 1);
        S.erase_by_comparator(arr[l]);
    }
    cout << endl;
    for (uint32_t l = 0; l <= n - k; l++) cout << Mx[l] << ' ';
}

void solve_ds() {
    uint32_t n, k;
    cin >> n >> k;
    auto get_min = [](auto x, auto y) { return x < y ? x : y; };
    auto get_max = [](auto x, auto y) { return x > y ? x : y; };
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    auto S = OY::MonoMinTree<int>(n, read);
    // auto S = OY::MaskRMQMinValueTable<int, 14>(n, read);
    // auto S = OY::STMinTable<int, 20>(n, read);
    // auto S = OY::CatMinTable<int, 20>(n, read);
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S.query(l, r) << ' ';
    cout << endl;
    auto S2 = OY::MonoMaxTree<int>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::MaskRMQMaxValueTable<int, 14>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::STMaxTable<int, 20>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::CatMaxTable<int, 20>(n, [&](uint32_t i) { return S.query(i); });
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S2.query(l, r) << ' ';
}

int main() {
    solve_window();
    // solve_heap();
    // solve_avl();
    // solve_splay();
    // solve_ds();
}