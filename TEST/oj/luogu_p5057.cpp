#include "DS/BIT.h"
#include "DS/DynamicBitset.h"
#include "DS/GlobalHashBIT.h"
#include "DS/LazyBitset.h"
#include "DS/MonoBIT.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)
*/
/**
 * 本题为 Bitset 模板题
 * 由于状态只有开关两种，也可以重载树状数组的符号为异或，用差分来实现F
 * 然而，目前最快的代码应当是用 GNU 扩展所打造的 WTREE 模板
 */

static constexpr uint32_t N = 100000;
void solve_wtree() {
    uint32_t n, m;
    cin >> n >> m;
    // 注意：WTree 区间大小为 n 时，是可以在下标 n 处进行 add 的
    OY::WXorTree<uint8_t> S(n);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l - 1, {1});
            S.add(r, {1});
        } else {
            uint32_t i;
            cin >> i;
            cout << S.presum(i - 1) << endl;
        }
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::DynamicBitset S(n);
    // OY::VectorLazyBitset<uint32_t, false> S(n);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            S.flip(l - 1, r - 1);
        } else {
            uint32_t i;
            cin >> i;
            cout << S[i - 1] << endl;
        }
    }
}

void solve_bit() {
    uint32_t n, m;
    cin >> n >> m;
    OY::MonoBitXorBIT<uint32_t> S(n + 1);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l - 1, {1});
            S.add(r, {1});
        } else {
            uint32_t i;
            cin >> i;
            cout << S.presum(i - 1) << endl;
        }
    }
}

int main() {
    solve_wtree();
    // solve_bitset();
    // solve_bit();
}