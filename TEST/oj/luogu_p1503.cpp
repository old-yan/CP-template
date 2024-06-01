#include "DS/AVL.h"
#include "DS/DynamicBitset.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P1503 鬼子进村](https://www.luogu.com.cn/problem/P1503)
*/
/**
 * 显然，本题为位集模板题，用位集模拟 set
 * 也可以用平衡树模拟 set
 */

void solve_avl() {
    uint32_t n, m;
    cin >> n >> m;
    OY::AVLMultiset<uint32_t, std::less<uint32_t>, 50001> S;
    S.insert_by_key(n + 1);
    std::vector<uint32_t> stack;
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == 'D') {
            uint32_t x;
            cin >> x;
            stack.push_back(x);
            S.insert_by_key(x);
        } else if (op == 'R') {
            S.erase_by_key(stack.back());
            stack.pop_back();
        } else {
            uint32_t x;
            cin >> x;
            auto rk = S.rank(x);
            uint32_t nxt = S.lower_bound(x)->get();
            if (nxt == x)
                cout << "0\n";
            else {
                uint32_t pre = S.smaller_bound(x)->get();
                cout << nxt - pre - 1 << endl;
            }
        }
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LazyBitset::Tree<uint32_t, false, 100000> B(n);
    // OY::DynamicBitset B(n);
    B.set();
    std::vector<uint32_t> stack;
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == 'D') {
            uint32_t x;
            cin >> x;
            stack.push_back(--x);
            B.reset(x);
        } else if (op == 'R') {
            B.set(stack.back());
            stack.pop_back();
        } else {
            uint32_t x;
            cin >> x;
            if (!B[--x])
                cout << "0\n";
            else {
                uint32_t pre = B.prev_zero(x);
                uint32_t nxt = B.next_zero(x);
                cout << (~nxt ? nxt : n) - pre - 1 << endl;
            }
        }
    }
}

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RangeManager<uint32_t> rm;
    rm.add_range({1, n});
    std::vector<uint32_t> stack;
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == 'D') {
            uint32_t x;
            cin >> x;
            stack.push_back(x);
            rm.remove_range({x, x});
        } else if (op == 'R') {
            rm.add_range({stack.back(), stack.back()});
            stack.pop_back();
        } else {
            uint32_t x;
            cin >> x;
            auto it = rm.any_of({x, x});
            if (it == rm.end())
                cout << "0\n";
            else
                cout << it->second - it->first + 1 << endl;
        }
    }
}

int main() {
    solve_avl();
    // solve_bitset();
    // solve_rm();
}