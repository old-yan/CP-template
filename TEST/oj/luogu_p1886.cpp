#include "DS/AVL.h"
#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
#include "DS/STTable.h"
#include "DS/SegTree.h"
#include "DS/WindowRMQ.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
*/

static constexpr uint32_t N = 1000000;
int arr[N], Mx[N];
void solve_window() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];

    auto S_min = OY::make_MinWindow(n, k, arr);
    S_min.extend_to(k - 2);
    for (uint32_t i = k - 1; i != n; i++) cout << S_min.next()->m_value << ' ';
    cout << endl;

    auto S_max = OY::make_MaxWindow(n, k, arr);
    S_max.extend_to(k - 2);
    for (uint32_t i = k - 1; i != n; i++) cout << S_max.next()->m_value << ' ';
    cout << endl;
}

void solve_avl() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    OY::AVLMultiset<int, std::less<int>, N> S;
    for (uint32_t l = 0, r = 0; r < n; l++) {
        while (r < l + k) {
            S.insert_by_key(arr[r++]);
        }
        cout << S.kth(0)->get() << ' ';
        Mx[l] = S.kth(S.size() - 1)->get();
        bool zero = false;
        S.erase_by_key(arr[l]);
    }
    cout << endl;
    for (uint32_t l = 0; l <= n - k; l++) cout << Mx[l] << ' ';
}

void solve_ds() {
    uint32_t n, k;
    cin >> n >> k;
    auto S = OY::MaskRMQMinValueTable<int, 14>(
        n, [](auto...) {
        int x;
        cin >> x;
        return x; });
    // auto S = OY::make_ZkwTree<int>(
    //     n, [](auto x, auto y) { return x < y ? x : y; }, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::make_SegTree<int, true, OY::Seg::Ignore, uint32_t>(
    //     n, [](auto x, auto y) { return x < y ? x : y; }, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::STMinTable<int, 20>(
    //     n, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::CatMinTable<int, 20>(
    //     n, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S.query(l, r) << ' ';
    cout << endl;
    auto S2 = OY::MaskRMQMaxValueTable<int, 14>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::make_ZkwTree<int>(
    //     n, [](auto x, auto y) { return x > y ? x : y; }, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::make_SegTree<int, true, OY::Seg::Ignore, uint32_t>(
    //     n, [](auto x, auto y) { return x > y ? x : y; }, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::STMaxTable<int, 20>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::CatMaxTable<int, 20>(n, [&](uint32_t i) { return S.query(i); });
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S2.query(l, r) << ' ';
}

int main() {
    solve_window();
    // solve_avl();
    // solve_ds();
}