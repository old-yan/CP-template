#include "DS/CompressedTree.h"
#include "DS/GlobalHashBIT.h"
#include "DS/MergeSortTree.h"
#include "DS/MonoAVL.h"
#include "DS/MonoPairHeap.h"
#include "DS/MonoSplay.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P1801 黑匣子](https://www.luogu.com.cn/problem/P1801)
*/
/**
 * 本题为模板题
 * 由于查找的 kth 是连续变化的，所以可以用对顶堆维护
 * 也可以简单粗暴用平衡树或者线段树找 kth
 */

int arr[200000];
void solve_heap() {
    uint32_t n, q;
    cin >> n >> q;
    OY::MonoPairHeap<int, std::less<>, OY::StaticBufferWrapWithCollect<200000>::type> smaller;
    OY::MonoPairHeap<int, std::greater<>, OY::StaticBufferWrapWithCollect<200000>::type> bigger;
    auto insert = [&](auto &&e) {
        smaller.push(e), bigger.push(smaller.top()), smaller.pop();
    };
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t cur = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        while (cur != x) insert(arr[cur++]);
        smaller.push(bigger.top()), bigger.pop();
        cout << smaller.top() << endl;
    }
}

void solve_avl() {
    uint32_t n, q;
    cin >> n >> q;
    OY::MonoAVLSequence<int, false, OY::StaticBufferWrapWithCollect<200000>::type> tr;
    // OY::MonoSplaySequence<int, false, OY::StaticBufferWrapWithCollect<200000>::type> tr;
    auto insert = [&](auto &&e) {
        tr.insert_by_comparator(e);
    };
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t cur = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        while (cur != x) insert(arr[cur++]);
        cout << tr.query(i) << endl;
    }
}

void solve_segcnt() {
    uint32_t n, q;
    cin >> n >> q;
    OY::SEGCNT::Table<uint64_t, uint32_t, true, false, false, OY::StaticBufferWrapWithCollect<400000>::type> tr;
    auto insert = [&](int64_t e) {
        tr.add(e + 2000000000, 1);
    };
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t cur = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        while (cur != x) insert(arr[cur++]);
        cout << int(uint32_t(tr.kth(i)->key()) - 2000000000) << endl;
    }
}

void solve_cptree() {
    uint32_t n, q;
    cin >> n >> q;
    OY::CPTREE::Tree<OY::CPTREE::BaseMonoid<uint32_t, 0, std::plus<uint32_t>>, uint64_t, OY::StaticBufferWrapWithCollect<400000>::type> tr;
    auto insert = [&](int64_t e) {
        tr.add(e + 2000000000, 1);
    };
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t cur = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        while (cur != x) insert(arr[cur++]);
        cout << int(uint32_t(tr.max_right(0, [&](auto cnt) { return cnt <= i; }) + 1) - 2000000000) << endl;
    }
}

OY::GBIT::Tree<uint64_t, uint32_t, false, false, 2000003> Gbit(4000000001);
void solve_gbit() {
    uint32_t n, q;
    cin >> n >> q;
    auto insert = [&](int64_t e) {
        Gbit.add(e + 2000000000, 1);
    };
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t cur = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        while (cur != x) insert(arr[cur++]);
        cout << int(uint32_t(Gbit.kth(i)) - 2000000000) << endl;
    }
}

void solve_wavelet() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    OY::MS::Tree<int> S(n, read);
    // OY::WaveLet::Tree<int> S(n, read);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t x;
        cin >> x;
        cout << S.quantile(0, x - 1, i) << endl;
    }
}

int main() {
    solve_heap();
    // solve_avl();
    // solve_segcnt();
    // solve_cptree();
    // solve_gbit();
    // solve_wavelet();
}