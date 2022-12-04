### 一、模板类别

​	数据结构：图论分组器。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。默认值为 `bool` ，表示边不带权值。

   构造参数 `uint32_t __vertexNum` ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本模板将不连通的图，分割为若干个极大连通子图，并将所有的边送到相应子图里，且做好一应的离散化工作。

   本数据结构可以处理带负权的边。

   本数据结构可以接受重边和自环。

   **注意：**

   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。对于无向图，本参数须为图的边数的二倍。


#### 2.加边

1. 数据类型

   输入参数 `uint32_t __from`​ ，表示边的一端的结点编号。

   输入参数 `uint32_t __to` ，表示边的另一端的结点编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法仅在无权图中使用。

#### 3.加边

1. 数据类型

   输入参数 `uint32_t __from`​ ，表示边的一端的结点编号。

   输入参数 `uint32_t __to` ，表示边的另一端的结点编号。

   输入参数 `_Tp __info` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法仅在有权图中使用。

#### 4.预备

1. 数据类型

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法首先利用并查集信息，获取每个结点所在的组号；然后将所有的边按照所在的组，收集起来。

### 三、模板示例

```c++
#include "GRAPH/Splitter.h"
#include "IO/FastIO.h"

int main() {
    // 建立一个分组器
    OY::Splitter<bool> sp(6, 5);
    // 加边
    sp.addEdge(1, 3);
    sp.addEdge(3, 4);
    sp.addEdge(5, 2);
    sp.addEdge(4, 3);
    sp.addEdge(4, 3);
    // 预备
    sp.prepare();

    // 查看每个小组的成员
    auto &groups = sp.m_groups;
    for (int i = 0; i < groups.size(); i++) {
        cout << "vertexes in No." << i << " group: ";
        for (auto a : groups[i]) cout << a << ' ';
        cout << endl;
    }
    // 查看每个小组的边
    // 注意边已经采用了离散化的编号
    auto &edges = sp.m_edgesOfGroup;
    for (int i = 0; i < edges.size(); i++) {
        cout << "edges in No." << i << " group:\n";
        for (auto &[from, to] : edges[i]) {
            cout << from << " -> " << to << " (" << groups[i][from] << " -> " << groups[i][to] << ")\n";
        }
    }
    // 也可以快速查询某个老结点现在所在的分组，以及组内编号
    for (int i = 0; i < 6; i++) {
        cout << "vertex No." << i << " is now inside group No." << sp.m_belong[i] << '\n';
        cout << "its id in group is " << sp.m_idInGroup[i] << '\n';
    }
}
```

```
#输出如下
vertexes in No.0 group: 0 
vertexes in No.1 group: 2 5 
vertexes in No.2 group: 1 3 4 
edges in No.0 group:
edges in No.1 group:
1 -> 0 (5 -> 2)
edges in No.2 group:
0 -> 1 (1 -> 3)
1 -> 2 (3 -> 4)
2 -> 1 (4 -> 3)
2 -> 1 (4 -> 3)
vertex No.0 is now inside group No.0
its id in group is 0
vertex No.1 is now inside group No.2
its id in group is 0
vertex No.2 is now inside group No.1
its id in group is 0
vertex No.3 is now inside group No.2
its id in group is 1
vertex No.4 is now inside group No.2
its id in group is 2
vertex No.5 is now inside group No.1
its id in group is 1

```

