### 一、模板类别

​	数据结构：最长线段射线助手。

​	练习题目：

1. [P3304 [SDOI2013] 直径](https://www.luogu.com.cn/problem/P3304)
2. [P3596 [POI2015] MOD](https://www.luogu.com.cn/problem/P3596)
3. [U261055 树的直径](https://www.luogu.com.cn/problem/U261055)

### 二、模板功能

#### 1.预处理

1. 数据类型

   模板参数 `typename Tree` ，表示树的类型。

   模板参数 `typename Tp` ，表示直径的权值类型。

   模板参数 `size_type MAX_VERTEX` ，表示树中的最大结点数量。

   构造参数 `Tree *rooted_tree` ，表示要进行预处理的树，须已指定根。默认为 `nullptr` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   在下文中，涉及两个概念：一个是 `Ray` ，表示一条从当前结点出发的射线的长度，换句话说，这条路径的一端已经被固定；一个是 `Seg` ，表示线段的长度，不限定从当前结点出发。

   树本质上就是无向无环图。任意找一点作为根，可以确定树的上下方向。

   由于树中没有环，所以任何一个结点的度，就相当于它所连接的连通部分数。

   对于根结点，所有的连通部分都在自己下方。

   对于非根结点，一个连通部分在自己上方，剩余连通部分都在自己下方。

   在预处理后， `m_val[a].m_ray` 存储了 以 `a` 作为一端的线段的最长长度前三名。

    `m_val[a].m_seg` 存储了不经过 `a` 的线段的最长长度前两名。

    `m_val[a].m_down_ray` 存储了以 `a` 作为一端的向下的线段的最长长度。

    `m_val[a].m_down_seg` 存储了 `a` 下方的不越过 `a` 的线段的线段最长长度。注意，不越过，但是可以经过。

    `m_val[a].m_up_ray` 存储了 以 `a` 作为一端的向上的线段的最长长度。

    `m_val[a].m_up_seg` 存储了 `a` 上方的不越过 `a` 的线段的线段最长长度。注意，不越过，但是可以经过。
   
   **注意：**此处的前三名、前两名不允许某个连通部分重复占用。比如结点 `a` 有两个邻居，从第一个连通部分可以找到长度为 `10` 的 `ray` ，也可以找到长度为 `9` 的 `ray` ；从第二个联通部分可以找到长度为 `8` 的 `ray` ，那么 `m_val[i].m_ray[0]==10` ，`m_val[i].m_ray[1]==8` ，`m_val[i].m_ray[2]==0` ，也就是没有第三名。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree` ，表示要进行预处理的树，须已指定根。

2. 时间复杂度

    $O(n)$ 。

#### 3.查询树中某部分的最长射线线段(max_ray_seg)

1. 数据类型

   输入参数 `size_type a` ，表示树中某结点。

   输入参数 `const Tp &except_ray` ，表示屏蔽掉的部分所提供的最长 `ray` 。

   输入参数 `const Tp &except_seg` ，表示屏蔽掉的部分所提供的最长 `seg` 。要求不经过 `a` 。

   返回一个 `RaySeg` 结构体，第一部分是树中剩余部分从 `a` 出发的最长 `ray` ，一个是树中剩余部分的最长 `seg` 。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   对于不屏蔽的问题，直接取相应属性即可。

   对于带屏蔽的问题，如果屏蔽的是上方的连通部分，那么要查询的内容也可以直接取相应属性。

   如果屏蔽的是下方的某一连通部分，那么只能调用本函数解决。


#### 4.查询去掉某子树后的最长射线线段(max_ray_seg_except_son)

1. 数据类型

   输入参数 `size_type a` ，表示树中某结点。

   输入参数 `size_type except_son` ，表示被屏蔽掉的孩子结点编号。

   输入参数 `const Tp &except_son_edge` ，表示通往被屏蔽掉的孩子结点的边的权重。

   返回一个 `RaySeg` 结构体，第一部分是树中剩余部分从 `a` 出发的最长 `ray` ，一个是树中剩余部分的最长 `seg` 。

2. 时间复杂度

   $O(1)$ 。

#### 5.查询向上的最大Ray(query_up_ray)

1. 数据类型

   输入参数 `size_type a` ，表示查询的结点编号。

   返回类型 `Tp` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本函数返回从 `a` 出发，向上延伸所能获取的最长路径长度。

#### 6.查询向上的最大Seg(query_up_seg)

1. 数据类型

   输入参数 `size_type a` ，表示查询的结点编号。

   返回类型 `Tp` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本函数返回端点 `a` 上方，最长的路径长度。

   本函数并不限定路径一端为 `a` ；但是也允许路径一端为 `a` 。

#### 7.查询向下的最大Ray(query_down_ray)

1. 数据类型

   输入参数 `size_type a` ，表示查询的结点编号。

   返回类型 `Tp` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本函数返回从 `a` 出发，向下延伸所能获取的最长路径长度。

#### 8.查询向下的最大Seg(query_down_seg)

1. 数据类型

   输入参数 `size_type a` ，表示查询的结点编号。

   返回类型 `Tp` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本函数返回端点 `a` 下方，最长的路径长度。

   本函数并不限定路径一端为 `a` ；但是也允许路径一端为 `a` 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RaySeg.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> S(5);
    // 加边
    S.add_edge(2, 0);
    S.add_edge(1, 3);
    S.add_edge(4, 0);
    S.add_edge(0, 3);
    S.prepare(), S.set_root(3);

    // 预处理
    OY::RAYSEG::Table<decltype(S), int, 1000> sol(&S);
    // 屏蔽掉以 2 为根的子树，询问最长线段
    auto ray_seg = sol.max_ray_seg(0, sol.query_down_ray(2) + 1, sol.query_down_seg(2));
    cout << "longest ray from 0, length = " << ray_seg.m_ray << endl;
    cout << "longest seg, length = " << ray_seg.m_seg << endl;
}
```

```
#输出如下
longest ray from 0, length = 2
longest seg, length = 3

```

