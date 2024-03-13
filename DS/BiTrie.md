### 一、模板类别

​	数据结构： `01` 字典树。

​	练习题目：

1. [1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
2. [1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)

### 二、模板功能


#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标类型。
   
   模板参数 `typename Tp` ，表示字典树中的数字类型，默认为 `uint32_t` 。

   模板参数 `size_type L` ，表示对每个数字的 `[0, L)` 位，从高位到低位进行考虑。默认为 `30` 。
   
   模板参数 `typename Info` ，表示结点维护的信息的类。默认为 `BaseInfo` 。 

   模板参数 `size_type MAX_NODE` ，表示节点内存池大小。默认为 `1<<20` 。 

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `01` 字典树本质和线段树一样，无非是在树中下降时，走左路还是走右路的选择比较自由。基于此，本数据结构中设计了迭代器 `iterator` 这一树中的类，通过迭代器可以较为方便地在树中移动、读写结点值、进行各种自由操作。


#### 2.插入元素(insert)

1. 数据类型

   输入参数 `Tp number` ，表示要插入的数字。

   输入参数 `Modify &&modify` ，表示从根到叶，对一路上的结点所做的操作。默认为 `Ignore` ，表示无操作。

   返回类型 `iterator` ，表示插入元素后的结果点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

#### 3.删除叶子结点(erase)

1. 数据类型

   输入参数 `Tp number` ，表示要删除的值。

   输入参数 `Judger &&judger` ，表示从叶到根，对一路上的结点进行删除之前进行的检查。默认为 `BaseEraseJudger` ，表示检查结点是否没有左孩子也没有右孩子。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数会从叶子一路上溯，一旦发现另一侧的孩子为空，则继续上溯删除；否则退出。
   
   如果树中无指定数字，则不做删除。

#### 4.追溯(trace)

1. 数据类型

   输入参数 `Tp number` ，表示要追溯的值。

   输入参数 `Modify &&modify` ，表示从叶到根，对一路上的结点执行的回调。默认为 `Ignore` ，表示不做操作。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数会从叶子一路上溯执行回调。

   如果树中无指定数字，会从最接近的祖先结点处上溯。

#### 5.查询最大相同值(query_max_same)

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   输入参数 `Judger &&judger` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<iterator, Tp>` ，前者为适配的叶结点，后者为适配情况下的逐位判定相同与否的 `mask` 。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中至少含有一个可选的元素。

#### 6.查询最大按位异或(query_max_bitxor)

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   输入参数 `Judger &&judger` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<iterator, _Tp>` ，前者为适配的叶结点，后者为适配情况下的按位异或值。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

### 三、模板示例

```c++
#include "DS/BiTrie.h"
#include "IO/FastIO.h"

void test_normal() {
    OY::BiTrie32<5, OY::BiTrie::BaseInfo, 1000> S;

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
    OY::BiTrie32<5, Info, 1000> S;
    using iterator = decltype(S)::iterator;
    // 插入数字时，记得修改一路上的 mask
    for (int val = 0; val < 10; val++) S.insert(val, [&](iterator it) { it->m_mask |= 1 << (val % 3); });
    // 在模 3 余数相同的范围内，查询 19 的最大异或
    auto res = S.query_max_bitxor(19, [&](iterator it) { return it && (it->m_mask >> (19 % 3) & 1); });
    auto leaf = res.first;
    auto mask = res.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask << endl;
    // 不限范围，查询 18 的最大异或
    auto res2 = S.query_max_bitxor(19, [&](iterator it) { return it; });
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask2 << endl;
    // 但是此时注意，字典树里删叶节点可不能乱删。得清空掉从叶到根的 m_mask 的残留影响。
    S.erase(19 ^ mask2);
    S.trace(19 ^ mask2, [](iterator p) {
        p->m_mask = p.child(0)->m_mask | p.child(1)->m_mask;
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

