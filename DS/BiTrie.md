### 一、模板类别

​	数据结构： `01` 字典树。

​	练习题目：

1. [1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
2. [1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)
3. [Set Xor-Min](https://judge.yosupo.jp/problem/set_xor_min)(https://github.com/yosupo06/library-checker-problems/issues/499)

### 二、模板功能


#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标类型。
   
   模板参数 `typename Tp` ，表示字典树中的数字类型，默认为 `uint32_t` 。

   模板参数 `size_type L` ，表示对每个数字的 `[0, L)` 位，从高位到低位进行考虑。默认为 `30` 。
   
   模板参数 `typename Info` ，表示结点维护的信息的类。默认为 `BaseInfo` 。 

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `01` 字典树本质和线段树一样，无非是在树中下降时，走左路还是走右路的选择比较自由。

#### 2.查询是否为空(empty)

1. 数据类型

   返回类型 `bool*` ，表示字典树是否为空。

2. 时间复杂度

   $O(1)$ 。
   
#### 3.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。
   
#### 4.预留空间(reserve)

1. 数据类型

   输入参数 `size_type capacity` ，表示预留的结点数。

2. 时间复杂度

   $O(n)$ 。
   
#### 5.根据插入元素数预留空间(reserve_by_count)

1. 数据类型

   输入参数 `size_type cnt` ，表示即将插入的元素数量。

2. 时间复杂度

   $O(n)$ 。

#### 6.插入元素(insert)

1. 数据类型

   输入参数 `Tp number` ，表示要插入的数字。

   输入参数 `Modify &&modify` ，表示从根到叶，对一路上的结点所做的操作。

   返回类型 `node*` ，表示插入元素后的叶结点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

#### 7.删除叶子结点(erase)

1. 数据类型

   输入参数 `Tp number` ，表示要删除的值。

   输入参数 `Judger &&judge` ，表示对叶子结点进行删除之前进行的检查。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数会从叶子一路上溯，一旦发现另一侧的孩子为空，则继续上溯删除；否则退出。
   
   如果树中无指定数字，则不做删除。
   
   **注意：**

    `judger` 参数提供了对叶子结点是否删除的一个条件判断。例如，当插入多个重复的数字，而只删除一个该数字时，并不会把叶子结点删除，而只会把叶子结点上的标记减一；直到叶子结点上的标记降到零，才会真正把这个叶子结点删除。


#### 8.追溯(trace)

1. 数据类型

   输入参数 `Tp number` ，表示要追溯的值。

   输入参数 `Modify &&modify` ，表示从叶到根，对一路上的结点执行的回调。默认为 `Ignore` ，表示不做操作。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数会从叶子一路上溯执行回调。

   如果树中无指定数字，会从最接近的祖先结点处上溯。

#### 9.查询最大相同值(query_max_same)

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   输入参数 `Judger &&judge` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<node*, Tp>` ，前者为适配的叶结点在模板内的下标，后者为适配情况下的逐位判定相同与否的 `mask` 。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中至少含有一个可选的元素。

#### 10.查询最大按位异或(query_max_bitxor)

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   输入参数 `Judger &&judge` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<node*, _Tp>` ，前者为适配的叶结点在模板内的下标，后者为适配情况下的按位异或值。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。


### 三、模板示例

```c++
#include "DS/BiTrie.h"
#include "IO/FastIO.h"

void test_normal() {
    OY::BiTrie32<5> S;

    // 插入数字
    S.insert(15);
    S.insert(1);
    S.insert(9);

    // 插入数字的返回值是一个迭代器，可以拿来做操作
    auto insert_leaf = S.insert(14);

    // 查询和 5 有最大相同值的数
    auto res1 = S.query_max_same(5);
    auto leaf1 = res1.first;
    auto mask1 = res1.second;
    cout << "5 and some number's"
         << " common bitmask :" << mask1 << endl;
    // 查询最大异或
    auto res2 = S.query_max_bitxor(5);
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "5 and some number's"
         << " bitxor result :" << mask2 << endl;
    S.erase(5 ^ mask2);
    auto res3 = S.query_max_bitxor(5);
    auto leaf3 = res3.first;
    auto mask3 = res3.second;
    cout << "5 and some number's"
         << " bitxor result :" << mask3 << endl;
}

void test_custom() {
    // 假定，现在我们在求最大异或、最大相同之外，还要添加条件: 要求必须在模 3 余数相同的群组内部进行查询
    struct Info {
        // 考虑 m_mask 的末三位。
        // 若 m_mask 有个位，说明子树中有模 3 余 0 的；
        // 若 m_mask 有十位，说明子树中有模 3 余 1 的；
        // 若 m_mask 有百位，说明子树中有模 3 余 2 的；
        uint32_t m_mask;
    };
    OY::BiTrie32<5, Info> S;
    using node = decltype(S)::node;
    // 插入数字时，记得修改一路上的 mask
    for (int val = 0; val < 10; val++) S.insert(val, [&](node *p) { p->m_mask |= 1 << (val % 3); });
    // 在模 3 余数相同的范围内，查询 19 的最大异或
    auto res = S.query_max_bitxor(19, [&](node *p) { return p->m_mask >> (19 % 3) & 1; });
    auto leaf = res.first;
    auto mask = res.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask << endl;
    // 不限范围，查询 18 的最大异或
    auto res2 = S.query_max_bitxor(19);
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask2 << endl;
    // 但是此时注意，字典树里删叶节点可不能乱删。得清空掉从叶到根的 m_mask 的残留影响。
    S.erase(19 ^ mask2);
    S.trace(19 ^ mask2, [&](node *p) {
        p->m_mask = 0;
        if (S._child_of(p, 0)) p->m_mask |= S._child_of(p, 0)->m_mask;
        if (S._child_of(p, 1)) p->m_mask |= S._child_of(p, 1)->m_mask;
    });
    // 再次不限范围，查询 18 的最大异或。注意参数可以省略
    auto res3 = S.query_max_bitxor(19);
    auto leaf3 = res3.first;
    auto mask3 = res3.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask3 << endl;
}

int main() {
    test_normal();
    test_custom();
}
```

```
#输出如下
5 and some number's common bitmask :27
5 and some number's bitxor result :12
5 and some number's bitxor result :11
19 and some number's bitxor result :23
19 and some number's bitxor result :27
19 and some number's bitxor result :26

```

