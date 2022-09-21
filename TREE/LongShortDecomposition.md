### 一、模板类别

​	数据结构：长短链（树链剖分）。

### 二、模板功能

#### 1.预处理

1. 数据类型

   模板参数 `typename _Tree` ，表示树的类型。

   构造参数 `_Tree &__tree`​ ，表示树。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   树链剖分指的是，将整个树分割成若干条自上而下的链，且该链在树的遍历序列里连续。当要对树上的某个路径进行处理时，可以将路径分割成若干条链，也就是树的遍历序列上的若干个区间，而在一维序列上进行区间操作，难度就降低很多。

   显然，一个子树对应的序列元素也是连续的。

   `m_size` 属性表示以每个结点为根的子树的大小。

   `m_height` 属性表示以每个结点为根的子树的高度，叶结点高度为 `1`。

   `m_depth` 属性表示每个结点距离根结点的距离。

   `m_heavySon` 属性表示每个结点的**重子结点** 。如果没有孩子，那么值为 `-1` 。

   `m_pos` 属性表示每个结点在树的遍历序列里的下标，位于 `[0, n)` 区间。

   `m_sequence` 用来存放树的遍历序列。

   `m_belong` 属性表示每个结点所在的链编号。

   `m_linkTop` 属性表示每个链的最高处的结点。

   `m_linkCount` 属性表示链的数量。

#### 2.获取树的遍历序列

1. 数据类型

   返回类型 `std::basic_string_view<uint32_t>` ，表示树的遍历序列。

2. 时间复杂度

   $O(1)$ 。

#### 3.获取某结点的祖先

1. 数据类型

   输入参数 `uint32_t __a` ，表示结点编号。

   输入参数 `uint32_t __n` ，表示要查询 `__n` 级祖先。
   
   返回类型 `uint32_t` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\sqrt n)$ 。
   
#### 4.查询两点间路径对应的区间

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个点。

   输入参数 `uint32_t __b` ，表示第二个点。

2. 时间复杂度

   $O(\sqrt n)$ 。

3. 备注

   所谓的“区间”指的是结点在树的遍历序列上的对应下标区间。

   返回的所有区间都是无重叠的。
   
   虽然长短链的时间复杂度劣于轻重链，但是由于常数极小，所以仍然很快。

#### 5.查询子树对应的区间

1. 数据类型

   输入参数 `uint32_t __a` ，表示子树的根。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   所谓的“区间”指的是结点在树的遍历序列上的对应下标区间。

#### 6.查询 LCA

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个结点。

   输入参数 `uint32_t __b` ，表示第二个结点。

   返回类型 `uint32_t` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\sqrt n)$ 。
   
3. 备注

   本数据结构并非专门求解 `LCA` ，所以效率略低于专门求解 `LCA` 的模板代码。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/LongShortDecomposition.h"

int main() {
    //一个无权树
    OY::Tree<10> T(5);
    //加边
    T.addEdge(2, 0);
    T.addEdge(1, 3);
    T.addEdge(4, 0);
    T.addEdge(0, 3);
    T.prepare();
    //定根
    T.setRoot(3);
    //轻重链预处理
    OY::LongShortDecomposition lsd(T);
    //查看链总数
    cout << "there are " << lsd.m_linkCount << " links\n";
    //查看每个点在树的遍历序列上的位置
    for (int i = 0; i < 5; i++) {
        cout << i << "'s position is " << lsd.m_pos[i] << endl;
    }
    //查看每个点所属的链
    for (int i = 0; i < 5; i++) {
        cout << i << "'s link is " << lsd.m_belong[i] << endl;
    }
    //查看 1 和 4 之间的路径所占的区间
    auto ranges = lsd.getLinks(1, 4);
    for (auto [l, r] : ranges) {
        cout << "from " << l << " to " << r << endl;
    }
    //查看以 0 为根的子树所占的区间
    auto range = lsd.getSubtree(0);
    auto [l, r] = range;
    cout << "from " << l << " to " << r << endl;
}
```

```
#输出如下
there are 3 links
0's position is 1
1's position is 4
2's position is 3
3's position is 0
4's position is 2
0's link is 0
1's link is 2
2's link is 1
3's link is 0
4's link is 0
from 4 to 4
from 0 to 2
from 1 to 3

```

