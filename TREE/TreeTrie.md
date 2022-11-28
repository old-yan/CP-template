### 一、模板类别

​	数据结构：字典树法树哈希。

### 二、模板功能

​	树的哈希有各种各样的方法。本哈希法的优势为，以较快的速度实现了**确定性**的哈希，即不依赖概率，只要树结构一模一样，结点编号随便乱改都一定可以保持哈希值相同；只要树结构不一样，哈希值一定不同。

#### 1.指定根，求树中每个子树的形态编号

1. 数据类型

   输入参数 `_Tree &__tree`​ ，表示树。

   输入参数 `uint32_t __root` ，表示指定的根。

   返回类型 `std::vector<uint32_t>` ，表示树中以每个结点为根的子树的形态编号。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

#### 2.无根树求形态编号

1. 数据类型

   输入参数 `_Tree &__tree`​ ，表示树。

   返回类型 `std::pair<uint32_t,uint32_t>` ，表示以重心为根建树，整棵树的形态编号。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   一棵树的重心可能有 `1~2` 个。如果只有一个，那么结果的 `second` 属性为 `-1` 。如果有两个，那么返回值升序排列。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/TreeTrie.h"

int main() {
    //一个无权树
    OY::Tree<10> T1(5);
    //加边
    T1.addEdge(2, 0);
    T1.addEdge(1, 3);
    T1.addEdge(4, 0);
    T1.addEdge(0, 3);
    T1.prepare();

    //再来一个无权树
    OY::Tree<10> T2(5);
    //加边
    T2.addEdge(1, 4);
    T2.addEdge(2, 3);
    T2.addEdge(2, 1);
    T2.addEdge(0, 1);
    T2.prepare();

    auto [hash11, hash12] = OY::getTreeTrieId(T1);
    cout << hash11 << ' ' << hash12 << endl;
    auto [hash21, hash22] = OY::getTreeTrieId(T2);
    cout << hash21 << ' ' << hash22 << endl;
}
```

```
#输出如下
2 4294967295
2 4294967295

```

