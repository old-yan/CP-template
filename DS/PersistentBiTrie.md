### 一、模板类别

​	数据结构：可持久化 `01` 字典树。

​	练习题目：

1. [2935. 找出强数对的最大异或值 II](https://leetcode.cn/problems/maximum-strong-pair-xor-ii/)
2. [P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)



### 二、模板功能

​		本数据结构的功能与实现与 `01` 字典树几乎一致，在此之外仅仅添加了可持久化的功能。所以与 `01` 字典树相同部分的内容不再介绍。

​		在可持久化数据结构中，不同版本的数据结构可能会共享很多数据，因此，当某一个版本进行写操作时，必须将涉及到的数据拷贝出来一份再进行修改，以避免对其他版本造成影响。然而，在进行单纯的读操作时，并不需要将涉及到的数据拷贝出来再进行读。所以，本数据结构新增了模板参数 `bool Lock` ，表示是否含有锁，如果没有锁，那么无论读还是写都会引发拷贝操作；如果有锁，那么可以通过静态函数 `lock` 和 `unlock` 进行状态切换，当 `lock` 之后，树上操作不会进行数据拷贝； `unlock` 之后，树上操作会引起数据拷贝。默认状态为已上锁，请在写操作前解锁，并在写操作完成后上锁。

#### 1.初始化(init)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   由于 `01` 字典树在默认构造时，默认会添加一个根结点；这样一个全局的可持久化 `01` 字典树数组就会占用很多结点。所以在可持久化版本中，构造时不再添加根结点，需要在构造之后手动调用 `init` 以添加根结点。

#### 2.拷贝线段树(copy)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。

   **注意：**本函数名“拷贝”并不等价于上文中的数据拷贝。本函数只涉及根结点的拷贝；数据拷贝指的是从根到叶结点的链的拷贝。

#### 3.版本间的差分树上求最大异或值(reduce_max_bitxor)

1. 数据类型

    输入参数 `const Tree<Tp, L, Info, Lock, MAX_NODE> &base` ，表示差分前的树。

    输入参数 `const Tree<Tp, L, Info, Lock, MAX_NODE> &end` ，表示差分后的树。

    输入参数 `Tp number` ，表示要查询的数字。

    输入参数 `Judger &&judger` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。

    返回类型 `Tp` ，表示适配情况下的按位异或值。

2. 时间复杂度

    $O(L)$ ，即关注的二进制长度。

3. 备注

    调用此函数时，默认差分树中含有元素。

### 三、模板示例

```c++
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
```

```
#输出如下
13
```

