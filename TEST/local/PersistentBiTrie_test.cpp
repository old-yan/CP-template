#include "DS/PersistentBiTrie.h"
#include "IO/FastIO.h"

#include <vector>

int main() {
    // 本结点用来统计管辖着几个叶子结点
    struct node {
        int cnt;
    };
    using PerTrie = OY::PerBiTrie32<5, node, false, 1000>;
    using iterator = PerTrie::iterator;
    int A[] = {4, 9, 2, 3, 5, 8, 7, 1, 6};

    // 常用可持久化实现区间操作
    std::vector<PerTrie> pool;
    PerTrie Base;
    Base.init();
    pool.push_back(Base);
    for (int a : A) {
        PerTrie copy = pool.back().copy();
        copy.insert(a, [](iterator it) { it->cnt++; });
        pool.push_back(copy);
    }

    // 查询 [2,3,5,8] 与 8 的最大异或
    cout << PerTrie::reduce_max_bitxor(pool[2], pool[6], 8, [](iterator it1, iterator it2) {
        return it2->cnt > it1->cnt;
    });
}
/*
#输出如下
13
*/