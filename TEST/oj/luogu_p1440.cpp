#include "DS/CatTree.h"
#include "DS/ErasableMonoPairHeap.h"
#include "DS/MaskRMQ.h"
#include "DS/MonoZkwTree.h"
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

/*
[P1440 求m区间内的最小值](https://www.luogu.com.cn/problem/P1440)
*/

static constexpr uint32_t N = 2000000;
void solve_window() {
    uint32_t n, k;
    cin >> n >> k;

    auto read = [&](uint32_t i) -> uint32_t {
        if (i < k - 1) return -1;
        uint32_t x;
        cin >> x;
        return x;
    };
    auto callback = [&](auto, auto, auto val) { cout << val << endl; };

    cout << "0\n";
    OY::WINDOW::solve_min<N>(n + k - 2, k, read, callback);
    cout << endl;
}

uint32_t arr[N];
void solve_heap() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::ErasableMonoPairHeap<uint32_t, std::greater<>> S;
    S._reserve(n * 2);
    for (uint32_t i = 0; i != n; i++) {
        if (i > k) S.erase(arr[i - k - 1]);
        cout << S.top() << endl;
        S.push(arr[i]);
    }
}

void solve_ds() {
    uint32_t n, k;
    cin >> n >> k;
    auto get_min = [](auto x, auto y) { return x < y ? x : y; };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto S = OY::MaskRMQMinValueTable<uint32_t, 16>(n, read);
    // auto S = OY::MonoMinTree<uint32_t>(n, read);
    // auto S = OY::STMinTable<uint32_t, 21>(n, read);
    // auto S = OY::CatMinTable<uint32_t, 21>(n, read);
    cout << "0\n";
    for (uint32_t i = 1; i != n; i++) cout << S.query(i >= k ? i - k : 0, i - 1) << endl;
}

int main() {
    solve_window();
    // solve_heap();
    // solve_avl();
    // solve_splay();
    // solve_ds();
}