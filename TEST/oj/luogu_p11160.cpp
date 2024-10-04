#include "DS/ReversedBiTrie.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"

/*
[P11160 【MX-X6-T6】機械生命体](https://www.luogu.com.cn/problem/P11160)
*/
/**
 * 本题需要实现一个数据结构，从低位到高位进行存储，同时支持对键值的懒标记加
 * 由于修改了键值，改变了树的结构，所以最好的实现策略是，把子树 split 下来，再 merge 回去
 */

constexpr uint32_t L = 32;
int main() {
    OY::REVBITRIE::CountTree<uint32_t, uint32_t, L, OY::StaticBufferWrapWithoutCollect<10000000>::type> S;
    uint32_t q;
    cin >> q;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.insert_one(x);
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            S.erase_one(x);
        } else if (op == '3') {
            uint32_t x, k, v;
            cin >> x >> k >> v;
            S.add_by_lowbits(x, k, v);
        } else {
            uint32_t x;
            cin >> x;
            auto [it, len] = S.find_greedy(x);
            cout << uint64_t(1ULL << len) << endl;
        }
    }
}
