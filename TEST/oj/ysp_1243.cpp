#include "DS/AVL.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"

/*
[Ordered Set](https://judge.yosupo.jp/problem/ordered_set)(https://github.com/yosupo06/library-checker-problems/issues/1243)
*/
/**
 * 本题为平衡树模板题
 * 由于允许离线进行，所以离散化一下，套 01 树状数组最快
 * 标准在线做法是平衡树
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto S = OY::AVLMultiset<uint32_t, std::less<>, OY::StaticBufferWrapWithoutCollect<1000000>::type>::from_mapping(n, read);
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0')
            S.modify_or_insert(read(), [](auto...) {});
        else if (op == '1')
            S.erase_by_key(read());
        else if (op == '2')
            if (auto k = read(); k > S.size())
                cout << "-1\n";
            else
                cout << S.kth(k - 1)->get() << endl;
        else if (op == '3')
            cout << S.rank(read() + 1) << endl;
        else if (op == '4')
            if (auto p = S.smaller_bound(read() + 1); p->is_null())
                cout << "-1\n";
            else
                cout << p->get() << endl;
        else if (auto p = S.lower_bound(read()); p->is_null())
            cout << "-1\n";
        else
            cout << p->get() << endl;
    }
}
