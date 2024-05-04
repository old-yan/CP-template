### 一、模板类别

​	数据结构：动态树（ `LCT` ）。

​	练习题目：

1. [P1501 [国家集训队] Tree II](https://www.luogu.com.cn/problem/P1501)
2. [P2056 [ZJOI2007] 捉迷藏](https://www.luogu.com.cn/problem/P2056)
3. [P2147 [SDOI2008] 洞穴勘测](https://www.luogu.com.cn/problem/P2147)
4. [P2486 [SDOI2011] 染色](https://www.luogu.com.cn/problem/P2486)
5. [P3178 [HAOI2015] 树上操作](https://www.luogu.com.cn/problem/P3178)
6. [P3203 [HNOI2010] 弹飞绵羊](https://www.luogu.com.cn/problem/P3203)
7. [P3384 【模板】重链剖分/树链剖分](https://www.luogu.com.cn/problem/P3384)
8. [P3690 【模板】动态树（LCT）](https://www.luogu.com.cn/problem/P3690)
9. [P3703 [SDOI2017] 树点涂色](https://www.luogu.com.cn/problem/P3703)
10. [P3950 部落冲突](https://www.luogu.com.cn/problem/P3950)
11. [P3979 遥远的国度](https://www.luogu.com.cn/problem/P3979)
12. [P4211 [LNOI2014] LCA](https://www.luogu.com.cn/problem/P4211)
13. [P4219 [BJOI2014] 大融合](https://www.luogu.com.cn/problem/P4219)
14. [P4299 首都](https://www.luogu.com.cn/problem/P4299)
15. [P4719 【模板】"动态 DP"&动态树分治](https://www.luogu.com.cn/problem/P4719)
16. [P4751 【模板】"动态DP"&动态树分治（加强版）](https://www.luogu.com.cn/problem/P4751)
17. [P5649 Sone1](https://www.luogu.com.cn/problem/P5649)
18. [U159634 [BZOJ3786]星系探索](https://www.luogu.com.cn/problem/U159634)
19. [U177650 【模板】Euler - Tour - Tree](https://www.luogu.com.cn/problem/U177650)
20. [Dynamic Tree Vertex Add Path Sum](https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum)(https://github.com/yosupo06/library-checker-problems/issues/223)
21. [Dynamic Tree Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/229)
22. [Dynamic Tree Vertex Set Path Composite](https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/307)
23. [Dynamic Tree Subtree Add Subtree Sum](https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/521)

### 二、模板功能

#### 1.构造动态树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `template <typename> typename NodeWrapper` ，表示树中的结点结构体模板类，需传递一个 `CRTP` 基类。

   模板参数 `bool MakeRoot` ，表示本模板是否需要支持换根操作。

   模板参数 `bool UpdateSubtree` ，表示本模板是否要支持虚孩子树。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1 << 20` 。

   构造参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

   构造参数 `Modify &&modify` ，表示在初始化时，对每个结点调用的回调函数。默认为 `Ignore` 类对象，表示对结点不做操作。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板名为动态树，实则维护的是树或者森林。

   在初始化时，所有结点之间均为割裂状态，所以此时模板中存在 `n` 个大小为 `1` 的单结点树。

   模板参数 `NodeWrapper` 类型须包含一个用于传递子类类型的模板参数。

   如果想支持链上信息查询，结点须满足以下要求：

   1. 实现成员函数 `pushup` ，接受左、右孩子结点的指针作为参数，更新以获取链上聚合信息。

   如果想支持链上修改，结点须满足以下要求：

   1. 实现成员函数 `pushdown` ，接受左、右孩子结点的指针作为参数，以下放修改信息。

   如果想支持子树查询，结点需满足以下要求：

   1. 实现成员函数 `pushup` ，接受左、右、虚孩子结点的指针作为参数，更新以获取子树聚合信息。
   2. 实现成员函数 `vpushup` ，接受虚孩子树种，左、右孩子结点的指针作为参数，更新以获取虚孩子树聚合信息。

   如果想支持子树修改，结点须满足以下要求：

   1. 实现成员函数 `pushdown` ，接受左、右、虚孩子结点的指针作为参数，以下放修改信息。
   2. 实现成员函数 `vpushdown` ，接受虚孩子树左、右结点的指针作为参数，以下放修改信息。

   显然， `LCT` 对于结点的要求并不高，因为 `LCT` 本身的操作比起结点的定义要更复杂，

   **注意：**

   要想灵活写出各种 `LCT` 的配套结点，必须对 `LCT` 的运作的基本概念有一些了解。

   要想学习 `LCT` ，需要先了解掌握重链剖分/长链剖分的知识。

   长链剖分和重链剖分的区别在于重儿子的选择标准。 `LCT` 使用了一种更加大胆的重儿子标准——想选哪个儿子就选哪个儿子。或者说，本次操作要针对哪个结点 `x` ，就从这个结点 `x` 一路上溯，每当遇到当前结点并非父结点的“偏爱儿子”时，就让父结点取消之前的偏爱儿子，并开始偏爱当前儿子，然后继续上溯。这个操作叫做 `access` ，这就是 `LCT` 的核心操作。下文称此操作为**"拉起"**。

   对于一个结点，选择了一个偏爱儿子，剩余子结点都是该结点的虚孩子。

   特别的，在选择偏爱儿子时，甚至可以不选任何孩子——也就是，偏爱儿子选择空，所有的子结点都是虚孩子。

   显然，在 `LCT` 中，子结点永远可以找到父结点，但是父结点并非任何时候都能找到所有的儿子，它只能找到偏爱儿子。

   以上是关于 `LCT` 和重/长链剖分在选择”偏爱儿子“上的区别。同时， `LCT` 在链的保存上也有区别。 `LCT` 把一条链以一棵 `splay` 的形态保存，这棵 `splay` 树的中序遍历顺序恰好是链的从高到低的遍历顺序。基于此，链信息由 `splay` 加以维护。在 `access` 操作时，假设本次操作要针对结点 `x` ，那么除了要把 `x` 到根的路”打通“，还要把 `x` 的偏爱儿子设置为空，这样从 `x` 到根，恰好为一棵 `splay` ，通过这棵 `splay` 可以实现对这个路径的修改和查询。

   基础 `LCT` 无法实现对子树信息的维护，因为父结点无法找到所有的儿子，就无法实现修改信息的下放。如果在每个结点处，再建一棵平衡二叉树以保存所有的虚孩子，就可以找到儿子们了，进而实现子树修改和子树查询。方便起见，这颗平衡二叉树也采用 `splay` 的结构，但是这个 `splay` 和前一个 `splay` 并非一个概念。

   **注意：**

   在讨论时，原树结构是一个概念，原树经过（实链）剖分所得链呈现出的 `splay` 是另一个概念，后者只对应前者的一条自上而下的链。

   本模板中也并非只存在一棵原树，比如在刚刚初始化后，未经加边，那么本模板中有 `n` 棵树。

   为避免混淆，下文称呼前者为连通块，后者为 `splay` 。

   **注意：**

   下文会用到一个概念叫**”维护子树“** 。显然，当 `UpdateSubtree` 为真时，本模板会维护子树；但是，即使当 `UpdateSubtree` 为假时，如果所传的结点存在添加/移除虚孩子时的回调函数 `add_vtree/remove_vtree` ，本模板虽然没有维护虚孩子树，但是同样有维护子树的功能，虽然只能起到不带修的子树查询的功能。

   **注意：**

   以下各种方法均要求结点编号从 `0` 开始。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

   输入参数 `Modify &&modify` ，表示在初始化时，对每个结点调用的回调函数。默认为 `Ignore` 类对象，表示对结点不做操作。

2. 时间复杂度

   $O(n)$ 。

#### 3.获取某结点(get_node)

1. 数据类型

   输入参数 `size_type i` ，表示结点编号。

   返回类型 `node *` ，表示结点指针。

2. 时间复杂度

    $O(1)$ .

3. 备注

   本方法获取编号对应的结点指针，但不保证结点已经进行 `access` 操作。换言之，如果在本结点上方有未下放的修改量，则对本结点的信息读取会引发错误。

#### 4.获取结点编号(get_id)

1. 数据类型

   输入参数 `node *x` ，表示要查询的结点。

   返回类型 `size_type` ，表示结点的编号。

2. 时间复习度

    $O(1)$ 。

3. 备注

   本方法可以查询 `LCT` 中，某结点对应的结点编号。

#### 5.拉起(access)

1. 数据类型

   输入参数 `size_type a` ，表示要拉起的结点编号。

   返回类型 `size_type` ，表示拉起所得的 `splay` 根结点编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法可以将结点 `a` 到其所在的连通块的根之间的路径打通，呈现一棵 `splay` ，并把 `splay` 的根结点编号返回。

   **注意：**

   经过拉起操作之后，结点 `a` 一定在这棵 `splay` 的最靠右的结点，但不一定是 `splay` 的根结点。

#### 6.拉起并做根(rooted_access)

1. 数据类型

   输入参数 `size_type a` ，表示要拉起的结点编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法不仅将结点 `a` 到其所在的连通块的根之间的路径打通，呈现一棵 `splay` ，并且会把 `a` 转到 `splay` 的根的位置。

   显然，经过本操作之后，结点 `a` 一定没有右孩子。

#### 7.拉出路径(access)

1. 数据类型

   输入参数 `size_type a` ，表示要拉出路径的一端的结点。

   输入参数 `size_type b` ，表示要拉出路径的另一端的结点。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法不做参数检查，使用时请保证结点 `a` 和结点 `b` 在同一连通块内。

   本方法首先把结点 `a` 设置为本连通块的根，然后对结点 `b` 执行拉起操作。

   本操作需要模板支持换根，执行本操作之后连通块的根为结点 `a` 。

#### 8.换根(make_root)

1. 数据类型

   输入参数 `size_type a` ，表示要换的根结点。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先拉出结点 `a` 到连通块的根之间的路径，然后打一个翻转标记；而 `splay` 的左右翻转恰好相当于原树的链的上下翻转，从而实现了连通块换根。

   **注意：**

   当 `splay` 上的聚合信息与子树的结合顺序有关时，需要在结点中设计好翻转时的结点变化方法。详见例题【染色】。

#### 9.对结点调用回调(do_for_node)

1. 数据类型

   输入参数 `size_type a` ，表示要操作的结点的编号。

   输入参数 `Callback &&call` ，表示要对结点进行的操作函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先将结点 `a` 到其所在的连通块的根之间的路径打通，呈现一棵 `splay` ，然后把 `a` 转到 `splay` 的根的位置。接着对结点 `a` 调用回调函数，然后进行聚合信息更新。

   显然，在回调函数中不需要考虑维护聚合信息，只需要读写结点自身的信息。

#### 10.对路径调用回调(do_for_path)

1. 数据类型

   输入参数 `size_type a` ，表示路径的最低端的结点。

   输入参数 `Callback &&call` ，表示要对路径进行的操作函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先将结点 `a` 到连通块的根结点之间的路径打通，呈现一棵 `splay` ，然后在这棵 `splay` 的根的位置调用回调函数。

   显然，在回调函数中需要考虑维护聚合信息。

#### 11.对子树调用回调(do_for_subtree)

1. 数据类型

   输入参数 `size_type a` ，表示要进行操作的子树的根。

   输入参数 `Callback &&call` ，表示要对子树进行的操作函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先将结点 `a` 到所在连通块的根之间的路径打通，呈现一棵 `splay` ，然后分情况讨论。

   如果在 `splay` 里结点 `a` 有前驱，就把结点 `a` 的前驱结点转到根的位置。此时，结点 `a` 必然为根结点的右孩子（可以思考这是为什么），且结点 `a` 没有左、右孩子。在结点 `a` 上调用回调函数，对以 `a` 为根的子树进行操作；

   如果在 `splay` 里结点 `a` 没有前驱，说明结点 `a` 是连通块的总根，那么直接在结点 `a` 上调用回调，执行对整个连通块的操作。。

   显然，在回调函数中需要考虑维护聚合信息。

   尽管在调用回调时，结点 `a` 不一定是 `splay` 的根，但是在回调函数中并不需要考虑维护结点 `a` 的父结点的聚合信息。

#### 12.对路径调用回调(do_for_path)

1. 数据类型

   输入参数 `size_type a` ，表示路径的一端的结点。

   输入参数 `size_type b` ，表示路径的另一端的结点。

   输入参数 `Callback &&call` ，表示要对路径进行的操作函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法不会检查参数合法性，调用时须保证结点 `a` 和结点 `b` 位于同一连通块内。

   本方法首先将结点 `a` 到结点 `b` 之间的路径打通，呈现一棵 `splay` ，然后在这棵 `splay` 的根的位置调用回调函数。

   显然，在回调函数中需要考虑维护聚合信息。

   本操作需要模板支持换根。

#### 13.查询最近公共祖先(lca)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先拉出结点 `a` 到根结点的路径，再拉出结点 `b` 到根结点的路径。根据 `LCT` 的性质，可以发现，最终得到的 `splay` 的根结点恰好为结点 `a` 和结点 `b` 的最近公共祖先。

#### 14.连接两个结点(connect)

1. 数据类型

   模板参数 `bool Check` ，表示是否检查两个结点的连通性。

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   若 `Check` 为真，则会先检查两个结点的连通性，如果连通就不做操作，返回 `false` ；如果不连通，就连通之，返回 `true` 。

   如果本模板不维护子树，本方法会将结点 `a` 设为其连通块的根，然后将结点 `a` 作为结点 `b` 的虚孩子（即将结点 `a` 的父亲指向 `b` ）。

   如果本模板维护子树，本方法会将结点 `a` 设为其连通块的根，令结点 `b` 设为其连通块的根。将结点 `a` 作为结点 `b` 的虚孩子（即将结点 `a` 的父亲指向 `b` ），并执行一应的更新操作。显然，本操作之后，连通块的根为结点 `b` 。

#### 15.断开两个结点(disconnect)

1. 数据类型

   模板参数 `bool Check` ，表示是否检查两个结点的连通性。

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   若 `Check` 为真，则会先检查两个结点之间是否有一条直连边，如果没有就不做操作，返回 `false` ；如果有，就断开之，返回 `true` 。

   本模板首先将结点 `a` 设为连通块的根，然后拉起结点 `b` ，并把结点 `b` 转到 `splay` 的根的位置。如果结点 `a` 和结点 `b` 直接相连，那么拉起 `b` 所得的链长度必为 `2` ，只需把结点 `b` 的左孩子设为空，即可断开。

#### 16.向上连接(connect_above)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type p` ，表示另一个结点的编号。

2. 时间复杂度

   若不维护子树， $O(1)$；否则为均摊 $O(\log n)$ 。

3. 备注

   本方法不检查参数的合法性，传入时请保证结点 `a` 和结点 `p` 不连通，且结点 `a` 为所在连通块的根，且亦为所在 `splay` 的根。

   如果本模板不维护子树，本方法会将结点 `a` 作为结点 `p` 的虚孩子（即将结点 `a` 的父亲指向 `p` ）。

   如果本模板维护子树，本方法会拉起结点 `p` ，并把结点 `p` 转到 `splay` 的根的位置，将结点 `a` 作为结点 `p` 的虚孩子（即将结点 `a` 的父亲指向 `p` ），并执行一应的更新操作。显然，本操作之后，连通块的根为结点 `p` 。

#### 17.向上断开(disconnect_above)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法会拉起结点 `a` ，并把结点 `a` 转到 `splay` 的根的位置，将结点 `a` 与左孩子断开，并执行一应的更新操作。显然，本操作之后，连通块的根为结点 `a` 。

#### 18.查询连通块的根(find_root)

1. 数据类型

   输入参数 `size_type a` ，表示要查询的结点编号。

   返回类型 `size_type` ，表示查询到的根结点的编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法会拉起结点 `a` ，并把 `splay` 中最靠左的元素转到根的位置，也即连通块的根。

#### 19.查询父结点(find_parent)

1. 数据类型

   输入参数 `size_type a` ，表示要查询的结点编号。

   返回类型 `size_type` ，表示查询到的父结点的编号编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法会拉起结点 `a` 。显然，结点 `a` 的父结点就是 `splay` 中结点 `a` 的前驱结点，也即 `splay` 的倒数第二个结点。

   如果有父结点，将父结点转到 `splay` 的根的位置，返回父结点；否则返回 `-1` 。

#### 20.查询某方向的子结点(find_son)

1. 数据类型

   输入参数 `size_type a` ，表示要查询的结点编号。

   输入参数 `size_type b` ，表示要查询的另一结点编号。

   返回类型 `size_type` ，表示查询到的子结点的编号编号。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法不会检查参数合法性，使用时须保证结点 `b` 为结点 `a` 的子孙。

#### 21.是否在同一连通块(in_same_group)

1. 数据类型

   输入参数 `size_type a` ，表示要查询的结点编号。

   输入参数 `size_type b` ，表示要查询的另一结点编号。

   返回类型 `bool` ，表示是否在同一连通块。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法要求模板支持换根。

#### 22.二分找最高位置(bisect_highest)

1. 数据类型

   输入参数 `size_type a` ，表示要进行二分的链的根结点。

   输入参数 `Judge &&judge` ，表示检查函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先将结点 `a` 到所在连通块的根之间的路径打通，呈现一棵 `splay` ，然后在树上二分找到 `judge` 返回 `true` 的最高结点。

#### 23.二分找最高位置(bisect_lowest)

1. 数据类型

   输入参数 `size_type a` ，表示要进行二分的链的根结点。

   输入参数 `Judge &&judge` ，表示检查函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法首先将结点 `a` 到所在连通块的根之间的路径打通，呈现一棵 `splay` ，然后在树上二分找到 `judge` 返回 `true` 的最低结点。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/LCT.h"

// 只维护树结构的 lct
void test_no_makeroot() {
    cout << "lct no make root:\n";
    // 不支持换根的树，必须通过 connect_above 加边
    OY::LCTTree<false, false, 1000> S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    // 此时，[1 ,2, 3, 4, 5, 6, 7, 8] 所在连通块根为 4
    // 此时，[0, 9] 所在连通块跟为 9

    // 通过 get_id 可以获取结点的编号
    for (int i = 0; i < 10; i++) cout << S.get_id(S.get_node(i)) << " \n"[i == 9];

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl;

    // 查询子结点
    cout << "son of 2 to 6 = " << S.find_son(2, 6) << endl;

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 查询是否在同一连通块
    if (S.in_same_group(0, 1))
        cout << "0 and 1 are in same group\n\n";
    else
        cout << "0 and 1 are not in same group\n\n";
}

// 可以换根，换父亲的 lct
void test_makeroot() {
    cout << "lct with make root:\n";
    // 支持换根的树，可以通过 connect 加边
    OY::LCTTree<true, false, 1000> S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(7, 2);
    S.connect<false>(0, 9);
    // 此时，[1 ,2, 3, 4, 5, 6, 7, 8] 所在连通块根为 4
    // 此时，[0, 9] 所在连通块跟为 9

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 把 6 7 之间的边拆开，6 连到 0，7 连到 9，这样全图连通了
    S.disconnect<false>(6, 7);
    S.connect<false>(6, 0);
    S.connect<false>(7, 9);

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl;

    // 查询子结点
    cout << "son of 7 to 1 = " << S.find_son(7, 1) << endl;

    // 查询是否在同一连通块
    if (S.in_same_group(0, 1))
        cout << "0 and 1 are in same group\n\n";
    else
        cout << "0 and 1 are not in same group\n\n";
}

// 维护路径的和，支持路径增值的 lct
template <typename Node>
struct NodeWrap_link {
    int m_val, m_size_on_link, m_inc_on_link, m_sum_on_link;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc_on_link += inc;
        m_sum_on_link += inc * m_size_on_link;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_sum_on_link = m_val, m_size_on_link = 1;
        if (!lchild->is_null()) {
            m_sum_on_link += lchild->m_sum_on_link;
            m_size_on_link += lchild->m_size_on_link;
        }
        if (!rchild->is_null()) {
            m_sum_on_link += rchild->m_sum_on_link;
            m_size_on_link += rchild->m_size_on_link;
        }
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc_on_link) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc_on_link);
            if (!rchild->is_null()) rchild->add_some_value(m_inc_on_link);
            m_inc_on_link = 0;
        }
    }
};
void test_link() {
    cout << "lct to maintain link:\n";
    using Tree = OY::LCT::Tree<NodeWrap_link, true, false, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_val << " \n"[i == 9];

    S.do_for_path(5, 6, [](node *p) {
        cout << "sum from 5 to 6 = " << p->m_sum_on_link << endl;
    });

    S.do_for_path(5, 6, [](node *p) {
        p->add_some_value(10);
    });

    S.do_for_path(5, 6, [](node *p) {
        cout << "sum from 5 to 6 = " << p->m_sum_on_link << endl
             << endl;
    });
}

// 维护子树的和，支持子树增值的 lct
// 表面看起来代码很长，但是这里是对结点判断是否为空之后再去加
// 其实，空结点值等于 0，所以如果不判空，直接加，代码就短多了
template <typename Node>
struct NodeWrap_subtree {
    int m_val, m_size, m_inc, m_sum;
    int m_virtual_size, m_virtual_inc, m_virtual_sum;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc += inc;
        m_sum += inc * m_size;
    }
    void vadd_some_value(int inc) {
        m_virtual_inc += inc;
        m_virtual_sum += inc * m_virtual_size;
    }
    // 此处必须考虑虚孩子们
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_sum = m_val, m_size = 1;
        if (!lchild->is_null()) {
            m_sum += lchild->m_sum;
            m_size += lchild->m_size;
        }
        if (!rchild->is_null()) {
            m_sum += rchild->m_sum;
            m_size += rchild->m_size;
        }
        if (!vroot->is_null()) {
            m_sum += vroot->m_virtual_sum;
            m_size += vroot->m_virtual_size;
        }
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_sum = m_sum, m_virtual_size = m_size;
        if (!vlchild->is_null()) {
            m_virtual_sum += vlchild->m_virtual_sum;
            m_virtual_size += vlchild->m_virtual_size;
        }
        if (!vrchild->is_null()) {
            m_virtual_sum += vrchild->m_virtual_sum;
            m_virtual_size += vrchild->m_virtual_size;
        }
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc);
            if (!rchild->is_null()) rchild->add_some_value(m_inc);
            if (!vroot->is_null()) vroot->vadd_some_value(m_inc);
            m_inc = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_inc) {
            if (!vlchild->is_null()) vlchild->vadd_some_value(m_virtual_inc);
            if (!vrchild->is_null()) vrchild->vadd_some_value(m_virtual_inc);
            this->add_some_value(m_virtual_inc);
            m_virtual_inc = 0;
        }
    }
};
void test_subtree() {
    cout << "lct to maintain subtree:\n";
    using Tree = OY::LCT::Tree<NodeWrap_subtree, true, true, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    cout << "root of 1 = " << S.find_root(1) << endl;

    S.do_for_subtree(2, [](node *p) {
        cout << "sum from subtree(2) = " << p->m_sum << endl;
    });

    S.do_for_subtree(2, [](node *p) {
        p->add_some_value(10);
    });

    S.do_for_subtree(2, [](node *p) {
        cout << "sum from subtree(2) = " << p->m_sum << endl
             << endl;
    });
}

// 如果只是维护子树大小，那么其实不需要有虚孩子树
// 只要在“实孩子”“虚孩子”切换的时候，给父结点汇报一下，就可以了
template <typename Node>
struct NodeWrap_size {
    int m_size, m_vsize;
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + m_vsize + lchild->m_size + rchild->m_size;
    }
    void add_vtree(Node *to_add) {
        m_size += to_add->m_size;
        m_vsize += to_add->m_size;
    }
    void remove_vtree(Node *to_remove) {
        m_size -= to_remove->m_size;
        m_vsize -= to_remove->m_size;
    }
};
void test_size() {
    cout << "lct to maintain tree size:\n";
    using Tree = OY::LCT::Tree<NodeWrap_size, true, false, 1000>;
    using node = Tree::node;
    Tree S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(7, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(0, 9);

    cout << "root of 1 = " << S.find_root(1) << endl;

    S.do_for_subtree(2, [](node *p) {
        cout << "size of subtree(2) = " << p->m_size << endl
             << endl;
    });
}

// 在维护子链大小的 lct 里试试二分
template <typename Node>
struct NodeWrap_bisect {
    int m_size;
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + lchild->m_size + rchild->m_size;
    }
};
void test_bisect() {
    cout<<"lct to bisect:\n";
    using Tree = OY::LCT::Tree<NodeWrap_bisect, true, false, 1000>;
    using node = Tree::node;
    Tree S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(7, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(0, 9);

    cout << "root of 1 = " << S.find_root(1) << endl;

    // 求 6 的 2 代祖宗，实际上就是求 splay 上右边的 size 大于 2 的临界点
    cout << "father of father of 6 = " << S.bisect_lowest(S.access(6), [right = 0](node *p) mutable {
        if (right + p->rchild()->m_size >= 2) return true;
        right += p->rchild()->m_size + 1;
        return false;
    }) << endl;

    // 求根到 6 的路上的第三层结点，就是求 splay 上左边的 size 大于 2 的临界点
    cout << "dep-3 node = " << S.bisect_highest(S.access(6), [left = 0](node *p) mutable {
        if (left + p->lchild()->m_size >= 2) return true;
        left += p->lchild()->m_size + 1;
        return false;
    }) << endl;
}

int main() {
    test_no_makeroot();
    test_makeroot();
    test_link();
    test_subtree();
    test_size();
    test_bisect();
}
```

```
#输出如下
lct no make root:
0 1 2 3 4 5 6 7 8 9
lca of 5 and 6 = 2
father of 7 = 2
son of 2 to 6 = 7
root of 7 = 4
0 and 1 are not in same group

lct with make root:
root of 7 = 4
root of 7 = 9
lca of 5 and 6 = 9
father of 7 = 9
son of 7 to 1 = 2
0 and 1 are in same group

lct to maintain link:
100 200 300 400 500 600 700 800 900 1000
sum from 5 to 6 = 2400
sum from 5 to 6 = 2440

lct to maintain subtree:
root of 1 = 4
sum from subtree(2) = 3700
sum from subtree(2) = 3760

lct to maintain tree size:
root of 1 = 4
size of subtree(2) = 6

lct to bisect:
root of 1 = 4
father of father of 6 = 2
dep-3 node = 7

```

