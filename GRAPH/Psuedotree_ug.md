### 一、模板类别

​	数据结构：无向基环树（森林）。

​	练习题目：

1. [Question for the Leader](https://acm.hdu.edu.cn/showproblem.php?pid=5329)
2. [LCP 21. 追逐游戏](https://leetcode.cn/problems/Za25hA/)
3. [#2669. 「NOI2013」快餐店](https://loj.ac/p/2669)
4. [#2774. 「BalticOI 2018」多角恋](https://loj.ac/p/2774)
5. [P1399 [NOI2013] 快餐店](https://www.luogu.com.cn/problem/P1399)
6. [P1453 城市环路](https://www.luogu.com.cn/problem/P1453)
7. [P4381 [IOI2008] Island](https://www.luogu.com.cn/problem/P4381)
8. [P5022 [NOIP2018 提高组] 旅行](https://www.luogu.com.cn/problem/P5022)
9. [P5049 [NOIP2018 提高组] 旅行 加强版](https://www.luogu.com.cn/problem/P5049)
10. [P6037 Ryoku 的探索](https://www.luogu.com.cn/problem/P6037)
11. [#126. 【NOI2013】快餐店](https://uoj.ac/problem/126)


### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示边权类型。
   
   模板参数 `typename SumType` ，表示边权的和的类型，默认为边权类型 `Tp` 。
   
   模板参数 `bool MaintainAncestor` ，表示是否维护环外的树边的祖先关系。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   无向基环树，指的是每个连通块内只含有一个环的有向图结构。一般而言，点数恰等于边数；即使整个图不连通，即为基环树森林，仍然满足点数等于边数。特殊情况下，可能边数小于点数，可以将没有边的点给自己连个自环，从而适用本模板。
   
   基环树问题，都是基于环来解决的。由于图可能不保证连通，所以可能存在多个环；每个环作为连通块的中心，在环上的每个点可能会向环外延伸出一棵树。
   
   **注意：**
   
   无向基环树没有定向。对于环外树部分，每条边的两端仍然有靠近环和远离环的区别；对于环上的点，可以从两种方向去游走。为了方便起见，所以对每个环钦定了一个方向，从而唯一地描述环上的结点序列。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

   输入参数 `Tp dis` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注
   
   在边权为 `bool` 时，无需传递第三个参数。

#### 4.预备(prepare)

1. 数据类型

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本方法在加边结束后进行预处理，处理出所有的环的点数、周长、所在连通块点数、环上的点序列、环上的点在环上的距离，并计算出每个点的所在环编号、到环的步数、距离、树根在环上的序号。
   
   当 `MaintainAncestor` 为 `true` 时，也会处理出环外的树上的点的祖孙关系。

#### 5.查询内向边(in_edge)

1. 数据类型

   输入参数 `size_type v` ，表示起点编号。

   返回类型 `const edge&` ，表示出边。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法返回查询点的出边。显然，出边是唯一的。
   
   对于环外的结点，返回指向环的边；对于环上的结点，返回钦定方向后，指向环上的后继结点的边。
#### 6.对所有外向邻居结点调用回调(do_for_out_edges)

1. 数据类型

   输入参数 `size_type v` ，表示起点编号。

   输入参数 `Callback &&call` ，表示对所有外向邻居调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   所谓的外向邻居，即满足 `x` 到 `v` 存在有向边，且 `x` 不在环上的点。
   
    `x` 的后继结点只有 `v` ，但是 `v` 的前驱（也就是外向邻居）可能有多个。

#### 7.对所有邻居结点调用回调(do_for_edges)

1. 数据类型

   输入参数 `size_type v` ，表示起点编号。

   输入参数 `Callback &&call` ，表示对所有外向邻居调用的回调函数。

2. 时间复杂度

   $O(n)$ 。


#### 8.获取后继点(next)

1. 数据类型
   
   输入参数 `size_type v` ，表示起点编号。

   返回类型 `size_type` ，表示后继结点编号。

2. 时间复杂度

   $O(1)$ 。

#### 9.获取后继点(next)

1. 数据类型
   
   输入参数 `size_type v` ，表示起点编号。
   
   输入参数 `uint64_t step` ，表示后继步数。

   返回类型 `size_type` ，表示后继结点编号。

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   本方法只有在 `MaintainAncestor` 为 `true` 时可用。
   
   对于环上的点，知道步数和环长之后可以很快计算后继点；对于环外树上的点，可以使用重链剖分、长链剖分、转 `RMQ` 等方式去解决 `k` 级祖先问题，甚至可以优化到 $O(1)$ 。
   
   然而，在基环树问题里，往往求 `k` 级祖先只是一个配菜，不值得动用上述科技。本模板采用 `MenghaniMatani` 算法解决 `k` 级祖先问题，复杂度带对数，但是实际速度极快。

#### 10.获取最近交点(join)

1. 数据类型
   
   输入参数 `size_type a_off_cycle` ，表示环外的某个结点。
   
   输入参数 `size_type b_off_cycle` ，表示环外的另一个结点。

   返回类型 `size_type` ，表示最近交点编号。

2. 时间复杂度

   $O(\log^2 n)$ 。
   
3. 备注

   本方法只有在 `MaintainAncestor` 为 `true` 时可用。
   
   本方法要求传入的两个结点位于同一个环上，且位于环上同一个结点延伸出的环外树上。尽管参数名字写着 `off_cycle` ，但是即便传递树的根结点（在环上）也是合法的。

   对于环上的点，知道步数和环长之后可以很快计算后继点；对于环外树上的点，可以使用重链剖分、长链剖分、转 `RMQ` 等方式去解决 `lca` 问题，甚至可以优化到 $O(1)$ 。
   
   然而，在基环树问题里，往往求 `lca` 只是一个配菜，不值得动用上述科技。本模板采用 `MenghaniMatani` 算法解决 `k` 级祖先问题，复杂度为对数平方，但是实际速度极快。

#### 11.查询环数量(cycle_count)

1. 数据类型
   
   返回类型 `size_type` ，表示环的数量。
   
2. 时间复杂度

    $O(1)$ 。
    
#### 12.查询结点信息(get_vertex_info)

1. 数据类型
   
   输入参数 `size_type v` ，表示要查询的结点编号。
   
   返回类型 `const vertex_info &` ，表示查询结果。
   
2. 时间复杂度

    $O(1)$ 。
    
3. 备注

   查询结果包含点的所在环编号(cycle_id)、到环的步数(step_to_cycle)、距离(dis_to_cycle)、树根在环上的序号(cycle_join)。
   
   由于边权类型不一定是 `bool` ，所以到环的距离可能不等于到环的步数。

#### 13.查询环信息(get_cycle_info)

1. 数据类型
   
   输入参数 `size_type cycle_id` ，表示要查询的环的编号。
   
   返回类型 `const cycle_info &` ，表示查询结果。
   
2. 时间复杂度

    $O(1)$ 。
    
3. 备注

   查询结果包含环的点数(size)、周长(length)、所在连通块点数(component_size)、环上的点序列、环上的点在环上的距离(position)。
   
   由于边权类型不一定是 `bool` ，所以到环的距离可能不等于到环的步数。
   
   连通块点数等于环上的点数加上以环上的点为根的环外树的点数。
   
   环上的点序列为任取一点为起点，环绕一周所得的环上结点序列。可以通过下标来获取。
   
    `position` 方法返回从起点正向到达某个结点的所需距离。
   

### 三、模板示例

```c++
#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

void test_cycle() {
    // 建立一个多个环图，看看 cycle 是怎么划分的
    OY::PsuedoUG::Graph<bool> G(9);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(3, 0);
    G.add_edge(4, 4);
    G.add_edge(5, 7);
    G.add_edge(6, 7);
    G.add_edge(7, 8);
    G.add_edge(8, 6);
    G.prepare();

    cout << "there are " << G.cycle_count() << " cycles\n";
    for (int id = 0; id < G.cycle_count(); id++) {
        auto cycle = G.get_cycle_info(id);
        cout << "No." << id + 1 << " cycle:\n";
        cout << "cycle size = " << cycle.size() << endl;
        cout << "component size = " << cycle.component_size() << endl;
        cout << "sequences: ";
        for (auto v : cycle) cout << v << ' ';
        cout << endl;
    }
    cout << endl;
}

void test_tree() {
    // 建立一个单环图，看看环外树是怎么维护的
    OY::PsuedoUG::Graph<bool, uint32_t, true> G(20);
    // 10~19 形成环
    for (int i = 10; i <= 19; i++) G.add_edge(i, i == 19 ? 10 : i + 1);
    // 0~5 形成一棵树，依附在环上的结点 11 之下
    G.add_edge(0, 1);
    G.add_edge(2, 1);
    G.add_edge(3, 4);
    G.add_edge(5, 4);
    G.add_edge(1, 11);
    G.add_edge(4, 11);

    // 6~9 形成一条链，依附在环上的结点 14 之下
    G.add_edge(9, 8);
    G.add_edge(8, 7);
    G.add_edge(7, 6);
    G.add_edge(6, 14);
    G.prepare();

    for (int step = 0; step <= 20; step++)
        cout << "from 9 walk " << step << (step > 1 ? " steps:" : " step :") << G.next(9, step) << endl;

    cout << "lca of 2 and 5 = " << G.join(2, 5) << endl;
    cout << "lca of 2 and 0 = " << G.join(2, 0) << endl;

    // 2 在环外树上，8 也在环外树上
    auto cycle = G.get_cycle_info(0);
    cout << "distance from 2 to cycle = " << G.get_vertex_info(2).step_to_cycle() << endl;
    cout << "distance from 8 to cycle = " << G.get_vertex_info(8).step_to_cycle() << endl;

    // 如果 2 和 8 相会在 8 的入环口，分别要走多少步？
    cout << "to join at 14:\n";
    auto join_2 = G.get_vertex_info(2).cycle_join();
    auto join_8 = G.get_vertex_info(8).cycle_join();
    cout << "2 needs to walk " << G.get_vertex_info(2).step_to_cycle() + cycle.position(join_8) - cycle.position(join_2) << endl;
    cout << "8 needs to walk " << G.get_vertex_info(8).step_to_cycle() << endl;
    // 如果 2 和 8 相会在 2 的入环口，分别要走多少步？
    cout << "to join at 11:\n";
    cout << "2 needs to walk " << G.get_vertex_info(2).step_to_cycle() << endl;
    cout << "8 needs to walk " << G.get_vertex_info(8).step_to_cycle() + (cycle.length() + cycle.position(join_2) - cycle.position(join_8)) << endl;
}

int main() {
    test_cycle();
    test_tree();
}
```

```
#输出如下
there are 3 cycles
No.1 cycle:
cycle size = 4
component size = 4
sequences: 0 1 2 3 
No.2 cycle:
cycle size = 1
component size = 1
sequences: 4 
No.3 cycle:
cycle size = 3
component size = 3
sequences: 6 7 8 

from 9 walk 0 step :9
from 9 walk 1 step :8
from 9 walk 2 steps:7
from 9 walk 3 steps:6
from 9 walk 4 steps:14
from 9 walk 5 steps:15
from 9 walk 6 steps:16
from 9 walk 7 steps:17
from 9 walk 8 steps:18
from 9 walk 9 steps:19
from 9 walk 10 steps:10
from 9 walk 11 steps:11
from 9 walk 12 steps:12
from 9 walk 13 steps:13
from 9 walk 14 steps:14
from 9 walk 15 steps:15
from 9 walk 16 steps:16
from 9 walk 17 steps:17
from 9 walk 18 steps:18
from 9 walk 19 steps:19
from 9 walk 20 steps:10
lca of 2 and 5 = 11
lca of 2 and 0 = 1
distance from 2 to cycle = 2
distance from 8 to cycle = 3
to join at 14:
2 needs to walk 5
8 needs to walk 3
to join at 11:
2 needs to walk 2
8 needs to walk 10

```
