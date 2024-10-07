#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/SequenceHash.h"

/*
[P4036 [JSOI2008] 火星人](https://www.luogu.com.cn/problem/P4036)
*/
/**
 * 本题须结合字符串哈希与平衡树
 * lcp 的查询可以参考 HashLCP 模板的实现
 */

using mint = OY::mintu32;
using hash_type = OY::SEQHASH::SeqHash<mint>;
struct Monoid {
    using value_type = hash_type;
    static value_type identity() { return {}; }
    static value_type op(const value_type &x, const value_type &y) { return x.append_right(y); }
};
using Tree = OY::MONOAVL::Tree<Monoid, false>;
// using Tree = OY::MONOSPLAY::Tree<Monoid, false>;
using node = Tree::node;
int main() {
    std::string s;
    uint32_t m;
    cin >> s >> m;
    Tree::_reserve(s.size() + m);

    uint32_t maxlen = std::min<uint32_t>(100000, s.size() + m);
    hash_type::s_info.set_base(131);
    hash_type::s_info.prepare_unit(maxlen), hash_type::s_info.prepare_unit_inv(maxlen);
    auto S = Tree::from_mapping(s.size(), [&](uint32_t i) { return hash_type::single(s[i]); });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t x, y;
            cin >> x >> y;
            uint32_t len1 = S.size() - x + 1, len2 = S.size() - y + 1;
            // 此处二分与 HashLCP 模板中的实现相同
            uint32_t low = 0, high = std::min(len1, len2);
            while (low < high)
                if (uint32_t mid = (low + high + 1) / 2; S.query(x - 1, x + mid - 2) == S.query(y - 1, y + mid - 2))
                    low = mid;
                else
                    high = mid - 1;
            cout << low << endl;
        } else if (op == 'R') {
            uint32_t x;
            char d;
            cin >> x >> d;
            S.modify(x - 1, hash_type::single(d));
        } else {
            uint32_t x;
            char d;
            cin >> x >> d;
            S.insert(x, hash_type::single(d));
        }
    }
}