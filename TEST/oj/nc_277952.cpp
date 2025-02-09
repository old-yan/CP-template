#include "DS/MappedMonoZkwTree.h"
#include "IO/FastIO.h"

/*
[小红的数组操作](https://ac.nowcoder.com/acm/problem/277952)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/88888/F
 * 本题使用单点修改线段树可以解决，每个叶子节点处有一个 multiset，multiset 的最小值作为叶子处的值
 * multiset 还可以进一步优化为一个懒删除堆
 * 
 * 当然，简便起见，也可以直接用一个子线段树当 multiset 
 */

void solve_mmzkw() {
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    using inner_tree = OY::MonoMinTree<uint32_t>;
    using monoid = inner_tree::group;
    struct Mapping {
        using container_type = inner_tree;
        static uint32_t get(const container_type &bucket) { return bucket.size() ? bucket.query_all() : monoid::identity(); }
    };
    uint32_t n = read();
    OY::MMZKW::Tree<monoid, Mapping> S(n);
    for (uint32_t i = 0; i != n; i++) S[i].resize(read(), read), S.update(i);
    uint32_t q = read();
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t i = read() - 1, j = read() - 1, x = read();
            S[i].modify(j, x);
            S.update(i);
        } else
            cout << S.query(0, read() - 1) << endl;
    }
}

int main() {
    solve_mmzkw();
}