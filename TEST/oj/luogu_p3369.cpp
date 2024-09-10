#include "DS/AVL.h"
#include "DS/BIT.h"
#include "DS/FHQCounter.h"
#include "DS/GlobalHashBIT.h"
#include "DS/SegCounter.h"
#include "DS/SegTree.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
*/
/**
 * 本题要维护一个有序集合，查询元素名次，根据名次查询元素
 * 为普通平衡树模板题
 */

OY::GHashBIT<uint32_t, uint32_t, false, false, 1 << 18> Gbit(1 << 25);
void solve_hash_bit() {
    static constexpr uint32_t M = 10000000;
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            int x;
            cin >> x;
            Gbit.add(x + M, 1);
        } else if (op == '2') {
            int x;
            cin >> x;
            Gbit.add(x + M, -1);
        } else if (op == '3') {
            int x;
            cin >> x;
            cout << Gbit.presum(x + M - 1) + 1 << endl;
        } else if (op == '4') {
            uint32_t x;
            cin >> x;
            cout << int(Gbit.kth(x - 1) - M) << endl;
        } else if (op == '5') {
            int x;
            cin >> x;
            cout << int(Gbit.kth(Gbit.presum(x + M - 1) - 1) - M) << endl;
        } else {
            int x;
            cin >> x;
            cout << int(Gbit.kth(Gbit.presum(x + M)) - M) << endl;
        }
    }
}

void solve_avl() {
    uint32_t n;
    cin >> n;
    OY::AVLMultiset<int, std::less<int>, 100000> S;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        int x;
        cin >> op >> x;
        if (op == '1')
            S.insert_by_key(x);
        else if (op == '2')
            S.erase_by_key(x);
        else if (op == '3')
            cout << S.rank(x) + 1 << endl;
        else if (op == '4')
            cout << S.kth(x - 1)->get() << endl;
        else if (op == '5')
            cout << S.smaller_bound(x)->get() << endl;
        else
            cout << S.upper_bound(x)->get() << endl;
    }
}

void solve_seg() {
    using SegTree = OY::Seg::Tree<OY::Seg::BaseNode<uint32_t>, OY::Seg::Ignore, false, uint32_t>;
    static constexpr uint32_t M = 10000001;
    SegTree S(M * 2 + 1);
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            int x;
            cin >> x;
            S.add(x + M, 1);
        } else if (op == '2') {
            int x;
            cin >> x;
            S.add(x + M, -1);
        } else if (op == '3') {
            int x;
            cin >> x;
            cout << S.query(0, x + M - 1) + 1 << endl;
        } else if (op == '4') {
            uint32_t x;
            cin >> x;
            cout << int(S.kth(x - 1).m_index - M) << endl;
        } else if (op == '5') {
            int x;
            cin >> x;
            cout << int(S.kth(S.query(0, x + M - 1) - 1).m_index - M) << endl;
        } else {
            int x;
            cin >> x;
            cout << int(S.kth(S.query(0, x + M)).m_index - M) << endl;
        }
    }
}

void solve_bit() {
    static constexpr uint32_t M = 10000001;
    OY::StaticBIT<uint32_t, false, 1 << 25> S(M * 2 + 1);
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            int x;
            cin >> x;
            S.add(x + M, 1);
        } else if (op == '2') {
            int x;
            cin >> x;
            S.add(x + M, -1);
        } else if (op == '3') {
            int x;
            cin >> x;
            cout << S.presum(x + M - 1) + 1 << endl;
        } else if (op == '4') {
            uint32_t x;
            cin >> x;
            cout << int(S.kth(x - 1) - M) << endl;
        } else if (op == '5') {
            int x;
            cin >> x;
            cout << int(S.kth(S.presum(x + M - 1) - 1) - M) << endl;
        } else {
            int x;
            cin >> x;
            cout << int(S.kth(S.presum(x + M)) - M) << endl;
        }
    }
}

void solve_segcounter() {
    static constexpr uint32_t M = 10000001;
    uint32_t n;
    cin >> n;
    OY::SEGCNT::Table<uint32_t, uint32_t, true, false, false, OY::StaticBufferWrapWithCollect<200000>::type> S;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            int x;
            cin >> x;
            S.add(x + M, 1);
        } else if (op == '2') {
            int x;
            cin >> x;
            S.add(x + M, -1);
        } else if (op == '3') {
            int x;
            cin >> x;
            cout << S.presum(x + M - 1) + 1 << endl;
        } else if (op == '4') {
            uint32_t x;
            cin >> x;
            cout << int(S.kth(x - 1)->key() - M) << endl;
        } else if (op == '5') {
            int x;
            cin >> x;
            cout << int(S.smaller_bound(x + M)->key() - M) << endl;
        } else {
            int x;
            cin >> x;
            cout << int(S.upper_bound(x + M)->key() - M) << endl;
        }
    }
}

void solve_fhqcounter() {
    uint32_t n;
    cin >> n;
    OY::FHQCNT::Table<int, uint32_t, true, false, 100000> S;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            int x;
            cin >> x;
            S.add(x, 1);
        } else if (op == '2') {
            int x;
            cin >> x;
            S.add(x, -1);
        } else if (op == '3') {
            int x;
            cin >> x;
            cout << S.presum(x - 1) + 1 << endl;
        } else if (op == '4') {
            uint32_t x;
            cin >> x;
            cout << S.kth(x - 1)->key() << endl;
        } else if (op == '5') {
            int x;
            cin >> x;
            cout << S.smaller_bound(x)->key() << endl;
        } else {
            int x;
            cin >> x;
            cout << S.upper_bound(x)->key() << endl;
        }
    }
}

int main() {
    solve_hash_bit();
    // solve_avl();
    // solve_seg();
    // solve_bit();
    // solve_segcounter();
    // solve_fhqcounter();
}