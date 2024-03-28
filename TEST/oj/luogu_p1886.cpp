#include "DS/FHQTreap.h"
#include "DS/ZkwTree.h"
#include "DS/SegTree.h"
#include "DS/STTable.h"
#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
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
    OY::FHQTreap<int, std::less<int>, N + 1> S;
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

struct Pair {
    uint32_t index;
    int value;
};
Pair deque[N];
void solve_monodeque() {
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    uint32_t l = 0, r = 0, dl = 0, dr = 0;
    while (r < k) {
        auto x = arr[r];
        while (dl != dr && deque[dr - 1].value >= x) dr--;
        deque[dr++] = {r++, x};
    }
    while (true) {
        cout << deque[dl].value << ' ';
        if (r == n) break;
        auto x = arr[r];
        while (dl != dr && deque[dr - 1].value >= x) dr--;
        if (dl != dr && deque[dl].index == l) dl++;
        l++;
        deque[dr++] = {r++, x};
    }
    cout << endl;
    l = 0, r = 0, dl = 0, dr = 0;
    while (r < k) {
        auto x = arr[r];
        while (dl != dr && deque[dr - 1].value <= x) dr--;
        deque[dr++] = {r++, x};
    }
    while (true) {
        cout << deque[dl].value << ' ';
        if (r == n) break;
        auto x = arr[r];
        while (dl != dr && deque[dr - 1].value <= x) dr--;
        if (dl != dr && deque[dl].index == l) dl++;
        l++;
        deque[dr++] = {r++, x};
    }
}

void solve_ds() {
    uint32_t n, k;
    cin >> n >> k;
    auto S = OY::MaskRMQMinValueTable<int, uint64_t, 1 << 20>(
        n, [](auto...) {
        int x;
        cin >> x;
        return x; });
    // auto S = OY::make_ZkwTree<int, 1 << 21>(
    //     n, [](auto x, auto y) { return x < y ? x : y; }, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::make_SegTree<int, true, OY::Seg::Ignore, 1 << 21, uint32_t>(
    //     n, [](auto x, auto y) { return x < y ? x : y; }, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::STMinTable<int, 1 << 25>(
    //     n, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    // auto S = OY::CatMinTable<int, 1 << 25>(
    //     n, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x; });
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S.query(l, r) << ' ';
    cout << endl;
    auto S2 = OY::MaskRMQMaxValueTable<int, uint64_t, 1 << 20>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::make_ZkwTree<int, 1 << 21>(
    //     n, [](auto x, auto y) { return x > y ? x : y; }, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::make_SegTree<int, true, OY::Seg::Ignore, 1 << 21, uint32_t>(
    //     n, [](auto x, auto y) { return x > y ? x : y; }, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::STMaxTable<int, 1 << 25>(n, [&](uint32_t i) { return S.query(i); });
    // auto S2 = OY::CatMaxTable<int, 1 << 25>(n, [&](uint32_t i) { return S.query(i); });
    for (uint32_t l = 0, r = k - 1; r < n; l++, r++) cout << S2.query(l, r) << ' ';
}

int main() {
    solve_monodeque();
    // solve_window();
    // solve_ds();
}