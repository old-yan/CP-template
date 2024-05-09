### 一、模板类别

​	数据结构： `Menghani-Matani` 算法。

​	练习题目：

1. [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
2. [P5903 【模板】树上 K 级祖先](https://www.luogu.com.cn/problem/P5903)
3. [U428478 祖孙询问](https://www.luogu.com.cn/problem/U428478)


### 二、模板功能

#### 1.预处理

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号类型。

   模板参数 `typename Tree` ，表示树的类型。

   构造参数 `Tree *rooted_tree`​ ，表示要处理的树，需要指定根，默认为 `nullptr`。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法来自 [An Analysis of the Level Ancestor Problem](https://celebration.tcnj.edu/wp-content/uploads/sites/115/2021/04/Holmes-Carolyne-Slides.pdf) ，也被称为 `Google-Facebook Algorithm` 。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree`​ ，表示要处理的树，需要指定根。

2. 时间复杂度

   $O(n)$ 。

#### 3.查询某结点的祖先(get_ancestor)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type n` ，表示要查询 `n` 级祖先。
   
   返回类型 `size_type` ，表示祖先编号。当祖先不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。

#### 4.查询某结点的父结点(find_parent)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   返回类型 `size_type` ，表示父结点编号。当父结点不存在时返回 `-1` 。

2. 时间复杂度

   $O(\log n)$ 。

#### 5.查询某结点在某子孙方向上的子结点(find_son)

1. 数据类型

   输入参数 `size_type a` ，表示结点编号。

   输入参数 `size_type b` ，表示结点 `a` 的某个子孙结点的编号。

   返回类型 `size_type` ，表示此方向的子结点编号。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法要求 `a` 结点非叶子结点， `b` 结点必须为 `a` 结点的子孙。所以答案必然存在。

#### 6.查询结点深度(get_depth)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   返回类型 `size_type` ，表示结点的深度。

2. 时间复杂度

   $O(1)$ 。

#### 7.查询最近公共祖先(calc)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   返回类型 `size_type` ，表示最近公共祖先结点的编号。

2. 时间复杂度

   $O(\log^2 n)$ 。

3. 备注

   尽管时间复杂度很高，但是实际运行效率也很高，与重链剖分基本持平。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/MenghaniMatani.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(5);
    // 加边
    T.add_edge(2, 0);
    T.add_edge(1, 3);
    T.add_edge(4, 0);
    T.add_edge(0, 3);
    T.prepare();
    T.set_root(3);
    cout << T << endl;
    // LCA 预处理
    OY::MenghaniMatani::Table<decltype(T)> LCA(&T);
    // 查询 祖先
    cout << "father of father of 4: " << LCA.get_ancestor(4, 2) << endl;
    // 查询父结点
    cout << "father of 4: " << LCA.find_parent(4) << endl;
    // 查询子结点
    cout << "son of 3(in the direction of 4): " << LCA.find_son(3, 4) << endl;
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
}
```

```
#输出如下
[3[1][0[2][4]]]
father of father of 4: 3
father of 4: 0
son of 3(in the direction of 4): 0
lca of 2 and 4: 0

```

