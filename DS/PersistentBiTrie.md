### 一、模板类别

​	数据结构：可持久化 `01` 字典树。

​	练习题目：

1. [2935. 找出强数对的最大异或值 II](https://leetcode.cn/problems/maximum-strong-pair-xor-ii/)
2. [P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)
3. [P5283 [十二省联考 2019] 异或粽子](https://www.luogu.com.cn/problem/P5283)



### 二、模板功能

​		本数据结构的功能与实现与 `01` 字典树几乎一致，在此之外仅仅添加了可持久化的功能。所以与 `01` 字典树相同部分的内容不再介绍。

​		本模板与 `01` 字典树一样，分成基础模板 `Tree` ，和封装后的 `CountTree` 。由于在可持久化问题中，往往需要差分处理问题，所以一般情况下都会使用 `CountTree` 。两颗 `CountTree` 做减法，得到一颗 `DiffTree` ，而 `DiffTree` 是两颗树的差信息，可以像一颗普通字典树一样调用各种成员方法。


#### 1.拷贝字典树(copy)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。

   **注意：**本函数名“拷贝”并不等价于上文中的数据拷贝。本函数只涉及根结点的拷贝；数据拷贝指的是从根到叶结点的链的拷贝。

### 三、模板示例

```c++
#include "DS/PersistentBiTrie.h"
#include "IO/FastIO.h"

int A[] = {9, 4, 6, 9, 2, 3, 5, 4, 4, 5};
void test_normal() {
    // 普通的可持久化字典树，仅仅是可以访问不同版本
    using PerTrie = OY::PerBiTrie::Tree<uint32_t, 5>;

    // 常用可持久化实现区间操作
    std::vector<PerTrie> pool;
    pool.push_back({});
    for (int a : A) {
        PerTrie copy = pool.back().copy();
        copy.insert(a);
        pool.push_back(copy);
    }
    for (int i = 0; i < pool.size(); i++) cout << "pool[" << i << "]:" << pool[i] << endl;

    // 查询是否包含某元素
    if (pool[7].contains(9))
        cout << "pool[7] contains 9\n";
    else
        cout << "pool[7] doesn't contain 9\n";
    if (pool[7].contains(6))
        cout << "pool[7] contains 6\n";
    else
        cout << "pool[7] doesn't contain 6\n";

    // 查询 [4,6,9,2,3,5,4] 与 8 的最大异或：6(00110) 和 8(01000) 最大程度不同
    auto res3 = pool[7].max_bitxor(8).second;
    cout << (res3 ^ 8) << " xor 8 = " << res3 << endl;

    // 查询 [4,6,9,2,3,5,4] 与 6 的最大异或：9(01001) 和 6(00110) 最大程度不同
    auto res4 = pool[7].max_bitxor(6).second;
    cout << (res4 ^ 6) << " xor 6 = " << res4 << endl
         << endl;
}

void test_info() {
    // 通过自定义 info 可以维护自定义信息
    // 比如这个 info 可以维护最大出现次数
    struct info {
        int m_max_cnt;
    };
    using PerTrie = OY::PerBiTrie::Tree<uint32_t, 5, info>;

    // 常用可持久化实现区间操作
    std::vector<PerTrie> pool;
    pool.push_back({});
    for (int a : A) {
        PerTrie copy = pool.back().copy();
        auto ptr = copy.insert(a);
        int cnt = ++(ptr->m_max_cnt);
        copy.trace(a, [&](PerTrie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
        pool.push_back(copy);
    }
    for (int i = 0; i < pool.size(); i++) cout << "pool[" << i << "]:" << pool[i] << endl;

    // 查询出现一次或一次以上的元素与 6 的最大异或：  9(01001) 和 6(00110) 最大程度不同
    auto judge1 = [](PerTrie::node *p) { return p->m_max_cnt >= 1; };
    auto res1 = pool.back().max_bitxor(6, judge1).second;
    cout << (res1 ^ 6) << " xor 6 = " << res1 << endl;

    // 查询出现两次或两次以上的元素与 6 的最大异或：  5(00101) 和 6(00110) 最大程度不同
    auto judge2 = [](PerTrie::node *p) { return p->m_max_cnt >= 2; };
    auto res2 = pool.back().max_bitxor(6, judge2).second;
    cout << (res2 ^ 6) << " xor 6 = " << res2 << endl;

    // 查询出现三次或三次以上的元素与 6 的最大异或：  4(00100) 和 6(00110) 最大程度不同
    auto judge3 = [](PerTrie::node *p) { return p->m_max_cnt >= 3; };
    auto res3 = pool.back().max_bitxor(6, judge3).second;
    cout << (res3 ^ 6) << " xor 6 = " << res3 << endl
         << endl;
}

void test_erasable() {
    // 可持久化字典树往往需要两颗字典树做差，在这个差上进行查询
    // 所以需要统计某个字符串的出现次数，用次数做差来作为搜索依据
    using PerTrie = OY::PerBiTrie::CountTree<uint32_t, int, 5>;

    // 常用可持久化实现区间操作
    std::vector<PerTrie> pool;
    pool.push_back({});
    for (int a : A) {
        PerTrie copy = pool.back().copy();
        copy.insert_one(a);
        pool.push_back(copy);
    }
    for (int i = 0; i < pool.size(); i++) cout << "pool[" << i << "]:" << pool[i] << endl;

    // 我们可以把两棵树的差，作为一个树视图拿出来
    auto diff_3_7 = pool[7] - pool[3];
    cout << "diff_3_7:" << diff_3_7 << endl;

    // 查询 [2,3,5,9] 与 8 的最大异或
    // 注意 pool[7] 包含一个 6，但是 pool[3] 也包含一个 6。两个抵消就没有 6 了
    auto res = diff_3_7.max_bitxor(8);
    cout << (res ^ 8) << " ^ 8 = " << res << endl;

    // 查询 [2,3,5,9] 与 6 的最大异或
    // 注意 pool[7] 包含两个 9， pool[3] 包含一个 9。抵消一个，还剩一个 9
    auto res2 = diff_3_7.max_bitxor(6);
    cout << (res2 ^ 6) << " ^ 6 = " << res2 << endl;

    auto diff_3_10 = pool[10] - pool[3];
    cout << "diff_3_10:" << diff_3_10 << endl;

    // 可以求出区间第 k 大异或
    // 注意，此处的 k 大异或是不去重的
    // 假如题目要求，查询 [9,2,3,5,4,4,5] 之内与 6 的第 k 大异或
    cout << "kth:\n";
    for (int k = 0; k < 7; k++) {
        auto res = diff_3_10.kth_bitxor(6, k);
        cout << "No." << k << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
    }

    // 甚至，还可以求出某个异或值是第几大异或
    // 注意，此处的 k 大异或是不去重的
    // 假如题目要求，查询 [9,2,3,5,4,4,5] 之内与 6 的异或是排第几
    cout << "rank:\n";
    for (int res = 0; res <= PerTrie::_mask(); res++)
        if (diff_3_10.contains(res ^ 6)) {
            auto rnk = diff_3_10.rank_bitxor(6, res);
            cout << "No." << rnk << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
        }
}

int main() {
    test_normal();
    test_info();
    test_erasable();
}
```

```
#输出如下
pool[0]:{}
pool[1]:{9}
pool[2]:{4, 9}
pool[3]:{4, 6, 9}
pool[4]:{4, 6, 9}
pool[5]:{2, 4, 6, 9}
pool[6]:{2, 3, 4, 6, 9}
pool[7]:{2, 3, 4, 5, 6, 9}
pool[8]:{2, 3, 4, 5, 6, 9}
pool[9]:{2, 3, 4, 5, 6, 9}
pool[10]:{2, 3, 4, 5, 6, 9}
pool[7] contains 9
pool[7] contains 6
6 xor 8 = 14
9 xor 6 = 15

pool[0]:{}
pool[1]:{9}
pool[2]:{4, 9}
pool[3]:{4, 6, 9}
pool[4]:{4, 6, 9}
pool[5]:{2, 4, 6, 9}
pool[6]:{2, 3, 4, 6, 9}
pool[7]:{2, 3, 4, 5, 6, 9}
pool[8]:{2, 3, 4, 5, 6, 9}
pool[9]:{2, 3, 4, 5, 6, 9}
pool[10]:{2, 3, 4, 5, 6, 9}
9 xor 6 = 15
9 xor 6 = 15
4 xor 6 = 2

pool[0]:{}
pool[1]:{9*1}
pool[2]:{4*1, 9*1}
pool[3]:{4*1, 6*1, 9*1}
pool[4]:{4*1, 6*1, 9*2}
pool[5]:{2*1, 4*1, 6*1, 9*2}
pool[6]:{2*1, 3*1, 4*1, 6*1, 9*2}
pool[7]:{2*1, 3*1, 4*1, 5*1, 6*1, 9*2}
pool[8]:{2*1, 3*1, 4*2, 5*1, 6*1, 9*2}
pool[9]:{2*1, 3*1, 4*3, 5*1, 6*1, 9*2}
pool[10]:{2*1, 3*1, 4*3, 5*2, 6*1, 9*2}
diff_3_7:{2*1, 3*1, 5*1, 9*1}
5 ^ 8 = 13
9 ^ 6 = 15
diff_3_10:{2*1, 3*1, 4*2, 5*2, 9*1}
kth:
No.0: 4 ^ 6 = 2
No.1: 4 ^ 6 = 2
No.2: 5 ^ 6 = 3
No.3: 5 ^ 6 = 3
No.4: 2 ^ 6 = 4
No.5: 3 ^ 6 = 5
No.6: 9 ^ 6 = 15
rank:
No.0: 4 ^ 6 = 2
No.2: 5 ^ 6 = 3
No.4: 2 ^ 6 = 4
No.5: 3 ^ 6 = 5
No.6: 9 ^ 6 = 15

```

