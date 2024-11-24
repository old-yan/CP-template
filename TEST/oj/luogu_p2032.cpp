#include "DS/CatTree.h"
#include "DS/ErasableMonoPairHeap.h"
#include "DS/MaskRMQ.h"
#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "DS/MonoZkwTree.h"
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

/*
[P2032 扫描](https://www.luogu.com.cn/problem/P2032)
*/

static constexpr uint32_t N = 250000;
void solve_window() {
    uint32_t n, k;
    cin >> n >> k;

    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto callback = [&](auto, auto, auto val) { cout << val << endl; };

    OY::WINDOW::solve_max<N>(n, k, read, callback);
    cout << endl;
}

uint32_t arr[N];
void solve_heap() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::ErasableMonoPairHeap<uint32_t> S;
    S._reserve(n * 2);
    for (uint32_t l = 0, r = 0; l + k <= n;) {
        while (r < l + k) S.push(arr[r++]);
        cout << S.top() << endl;
        S.erase(arr[l++]);
    }
}

void solve_avl() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    using Tree = OY::MonoAVLSequence<uint32_t, false>;
    Tree::_reserve(n + 1);
    Tree S;
    for (uint32_t l = 0, r = 0; r < n; l++) {
        while (r < l + k) S.insert_by_comparator(arr[r++], std::greater<>());
        cout << S.query(0) << endl;
        S.erase_by_comparator(arr[l], std::greater<>());
    }
}

void solve_splay() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    using Tree = OY::MonoSplaySequence<uint32_t, false>;
    Tree::_reserve(n + 1);
    Tree S;
    for (uint32_t l = 0, r = 0; r < n; l++) {
        while (r < l + k) S.insert_by_comparator(arr[r++], std::greater<>());
        cout << S.query(0) << endl;
        S.erase_by_comparator(arr[l], std::greater<>());
    }
}

void solve_ds() {
    uint32_t n, k;
    cin >> n >> k;
    auto get_max = [](auto x, auto y) { return x > y ? x : y; };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto S = OY::MaskRMQMaxValueTable<uint32_t, 14>(n, read);
    // auto S = OY::MonoMaxTree<uint32_t>(n, read);
    // auto S = OY::STMaxTable<uint32_t, 18>(n, read);
    // auto S = OY::CatMaxTable<uint32_t, 18>(n, read);
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S.query(l, r) << endl;
}

int main() {
    solve_window();
    // solve_heap();
    // solve_avl();
    // solve_splay();
    // solve_ds();
}