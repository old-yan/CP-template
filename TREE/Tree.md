### 一、模板类别

​	数据结构：树。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `uint32_t _MAXN` ，表示树中点数可能的最大值。

   模板参数 `typename _Tp` ，表示树中边权的类型。默认值为 `bool` ，表示边不带权值。

   构造参数 `uint32_t __vertexNum`​ ，表示树中的实际点数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本数据结构为树结构。

   显然，添加的边数必须恰比点数少一。

   **注意：**

   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。

   以下各种方法均要求结点编号从 `0` 开始。

#### 2.从父结点数组构造

1. 数据类型

   输入参数 `const std::vector<int>&__parent` ，表示每个点的父结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在力扣等平台，经常会给出一个父结点数组，这个数组的长度表示点的数目，数组中的值表示每个点的父结点编号。为方便起见，提供本静态方法，直接从父结点数组构造树。

   **注意：**本方法要求结点编号从 `0` 开始。

#### 3.从边数组构造

1. 数据类型

   输入参数 `const std::vector<int>&__edges` ，表示树中的所有边。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在力扣等平台，经常会给出一个边数组。为方便起见，提供本静态方法，直接从边数组构造树。

   **注意：**本方法要求结点编号从 `0` 开始。

#### 4.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示边的一端的结点编号。

   输入参数 `uint32_t __b` ，表示边的另一端的结点编号。

2. 时间复杂度

   $O(1)$。

3. 备注

   在添加边时，不关注哪端在上哪端在下。
   
   本方法仅在无权树中使用。

#### 5.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示边的一端的结点编号。

   输入参数 `uint32_t __b` ，表示边的另一端的结点编号。

   输入参数 `_Tp __distance` ，表示边权。

2. 时间复杂度

   $O(1)$。

3. 备注

   在添加边时，不关注哪端在上哪端在下。

   本方法仅在有权树中使用。

#### 6.预备

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 7.设置根

1. 数据类型

   输入参数 `uint32_t __root` ，表示要设置为的根结点编号。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   通过本方法，可以方便地调整整棵树的根。
   
   在调用本方法之前，本树为无根树。

#### 8.获取父结点

1. 数据类型

   输入参数 `uint32_t __i` ，表示要获取父结点的结点。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法之前，必须先指定根。

   根节点的父结点为 `-1` 。

#### 9.获取距离数组

1. 数据类型

   输入参数 `uint32_t __source` ，表示要获取距离的起点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，不需要先指定根。

#### 10.获取子树累计值

1. 数据类型

   输入参数 `_Mapping __map` ，表示结点到值的映射函数。

   输入参数 `_Operation __op` ，表示子树值之间的累计函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，必须先指定根。

   使用本方法可以轻松获取每个点的大小、高度。

   可以令映射类型为 `std::bitset<_MAXN>` ，以 $O(n^2)$ 的时间复杂度获取每个子树所包含的节点。

#### 11.获取距离和数组

1. 数据类型

   输入参数 `_Mapping __map` ，表示结点到权值的映射函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，必须先指定根。

   使用本方法可以获取每个点作为中心时，其他所有点到中心的加权距离和。

#### 12.获取重心

1. 数据类型

   返回类型 `std::pair<uint32_t,uint32_t>` ，表示重心。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，不需要先指定根。

   一棵树的重心可能有 `1~2` 个。如果只有一个，那么结果的 `second` 属性为 `-1` 。

#### 13.输出查看

1. 数据类型

   输入参数 `_Ostream&__out` ，表示输出流。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，需要先指定根。

   将输出的字符串粘贴到 [在线画树](http://mshang.ca/syntree/) ，就可以对小型的树进行观察。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/Tree.h"
#include <bitset>

int main() {
    //一个无权树
    OY::Tree<10> T1(6);
    //加边
    T1.addEdge(0, 1);
    T1.addEdge(0, 2);
    T1.addEdge(0, 3);
    T1.addEdge(0, 4);
    T1.addEdge(0, 5);
    T1.prepare();
    //定根
    T1.setRoot(0);
    //输出观察树形(粘贴到http://mshang.ca/syntree/)
    cout << "T1:" << T1 << endl;
    //获取每个子树所包含的结点
    auto include = T1.getSubtreeValues([](uint32_t i) {std::bitset<10>b;b.set(i);return b; },
                                       [](std::bitset<10> &x, std::bitset<10> &y) { return x | y; });
    if (include[0][3])
        cout << "0 is ancestor of 3\n";
    else
        cout << "0 isn't ancestor of 3\n";
    //换根
    T1.setRoot(1);
    //获取 4 的父结点
    cout << "father of 4: " << T1.getParent(4) << endl;
    //获取每个子树的大小
    auto sizes = T1.getSubtreeValues([](uint32_t) { return 1; },
                                     [](uint32_t x, uint32_t y) { return x + y; });
    cout << "sizes of subtrees:";
    for (uint32_t i = 0; i < 6; i++) cout << sizes[i] << ' ';
    cout << endl;
    //获取每个子树的高度
    auto heights = T1.getSubtreeValues([](uint32_t) { return 1; },
                                       [](uint32_t x, uint32_t y) { return std::max(x, y + 1); });
    cout << "heights of subtrees:";
    for (uint32_t i = 0; i < 6; i++) cout << heights[i] << ' ';
    cout << endl;

    //从 parent 数组构造一个无权树
    std::vector<int> parent{2, 0, 4, 4, -1};
    auto T2 = OY::Tree<10>::fromParentArray(parent);
    //注意此时该树无根
    //找重心
    auto [r1, r2] = T2.getCentroid();
    if (~r2)
        cout << "centroid of T2: " << r1 << ", " << r2 << endl;
    else
        cout << "centroid of T2: " << r1 << endl;

    //有权树
    OY::Tree<10, long long> T3(6);
    T3.addEdge(0, 1, 5);
    T3.addEdge(0, 2, 20);
    T3.addEdge(0, 3, 10);
    T3.addEdge(0, 4, 20);
    T3.addEdge(0, 5, 10);
    T3.prepare();
    //假如每个点住着一定的人数，问车站建在哪里，使大家到车站总距离最短？
    //这时就需要计算距离和
    T3.setRoot(0);
    int people[] = {10, 98, 22, 5, 20, 5};
    auto distance_sum = T3.getDistanceSum([&](uint32_t i) { return people[i]; });
    cout << "distance_sum to each vertex:";
    for (uint32_t i = 0; i < 6; i++) cout << distance_sum[i].upSum + distance_sum[i].downSum << ' ';
    cout << endl;
    //事实证明，建在点 1 最佳
}
```

```
#输出如下
T1:[0[1][2][3][4][5]]
0 is ancestor of 3
father of 4: 0
sizes of subtrees:5 6 1 1 1 1 
heights of subtrees:2 3 1 1 1 1 
centroid of T2: 2
distance_sum to each vertex:1430 1250 3750 2930 3830 2930 

```

