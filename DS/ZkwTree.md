### 一、模板类别

​	数据结构： `zkw` 线段树。

​	练习题目：

1. [#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
2. [P2572 [SCOI2010] 序列操作](https://www.luogu.com.cn/problem/P2572)
3. [P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
4. [P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)
5. [P4560 [IOI2014] Wall 砖墙](https://www.luogu.com.cn/problem/P4560)
6. [U187320 【模板】树状数组 3](https://www.luogu.com.cn/problem/U187320)
7. [U216697 线段树区间历史版本和](https://www.luogu.com.cn/problem/U216697)
8. [#164. 【清华集训2015】V](https://uoj.ac/problem/164)
9. [Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/233)
10. [Range Affine Point Get](https://judge.yosupo.jp/problem/range_affine_point_get)(https://github.com/yosupo06/library-checker-problems/issues/778)




### 二、模板功能

#### 1.建立线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Node` ，表示结点类型。

   构造参数 `size_type length` ，表示线段树的覆盖范围为 `[0, length)`。默认值为 `0` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `ZKW::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   基础线段树处理的问题为单点修改，单点/区间的查询。

   带懒惰标记的线段树处理的问题为单点/区间修改，单点/区间的查询。

   不同于之前版本的线段树设计，目前的线段树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让线段树工作。

   对于基础线段树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。

   至此，基础线段树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 定义静态函数 `map` ，接受一个修改值和一个结点指针，对该结点进行相应修改。如果没有定义本函数，则会把修改值和结点的旧值通过 `op` 函数聚合之后，赋给结点；
   2. 定义成员函数 `pushup` ，接受两个孩子结点的指针，聚合到当前结点；或者接受两个孩子结点的指针，以及一个表示当前结点区间大小的数字，聚合到当前结点。如果没有定义本函数，则会把两孩子结点的值通过 `op` 函数聚合之后，赋给当前结点。

   对于带懒惰标记的线段树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 定义静态函数 `map` ，接受一个修改值、一个结点指针、一个表示当前结点区间大小的数字，对该结点的值进行相应修改；
   4. 定义静态函数 `com` ，接受一个修改值和一个结点指针，对该结点的懒惰增量进行相应修改；
   5. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   6. 实现成员函数 `get` ，返回本结点的值；
   7. 实现成员函数 `get_lazy` ，返回本结点的懒惰增量；
   8. 实现成员函数 `clear_lazy` ，将本结点的懒惰增量清零；

   至此，带懒惰标记的线段树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 声明 `modify_type` 为修改类型。如果没有定义本类型，则修改类型等同于值类型；
   2. 定义静态常量 `init_clear_lazy` ，返回布尔值，表示在初始化时是否需要对所有结点的懒惰增量清零。由于全局变量、静态变量区的 `int` 等类型本身就为零，如果这和懒惰增量的清零状态一致的话，就不需要再强制清零；但是如果有特殊的需求，比如进行乘法修改的增量清零后须为 `1` ，则须声明本常量并返回 `true` ;
   3. 定义成员函数 `pushup` ，接受两个孩子结点的指针，聚合到当前结点；或者接受两个孩子结点的指针，以及一个表示当前结点区间大小的数字，聚合到当前结点。如果没有定义本函数，则会把两孩子结点的值通过 `op` 函数聚合之后，赋给当前结点；
   4. 定义成员函数 `has_lazy` ，返回布尔值，表示本结点是否含有懒惰增量。如果定义本函数，在下传懒惰增量的时候会先进行判断，如果没有增量就不操作。如果没定义，则无论何时都进行下传操作。
   
   一般的，我们称 `op` 函数执行的是聚合操作， `map` 函数执行的是增值操作， `com` 函数执行的是囤积操作。
   
   基础线段树要求区间操作函数的运算符满足**结合律**。
   
   带懒惰标记的线段树要求聚合函数、增值函数和囤积函数的运算符都满足**结合律**。
   
   **注意：**
   
   在上方介绍了两个不同的 `map` 函数，尽管这两个都是增值函数，但是出现在不同的场景里。前者仅仅出现在基础线段树中，用于对区间大小为 `1` 的结点进行增值，由于区间大小恒定，所以不需要再传递区间长度参数，而且本函数属于可有可无的函数；后者出现在带懒惰标记的线段树中，对各种区间长度的结点进行增值，所以需要传递区间长度参数，此函数为带懒惰标记的线段树的实现关键。
   
   **注意：**
   
   本模板对不符合交换律的运算符同样适用。
   
   **注意：**
   
   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。默认情况下， `mapping` 为 `ZKW::Ignore` 类，表示不进行初始化，比如要建立一颗空的求和线段树。
   
   **注意：**
   
   以下为线段树内的行为判定法则：
   
   类内的`modify_type`首先取决于结点的 `modify_type` 声明；若无此声明，则等同于 `value_type` ；
   
   非叶结点 `_apply`行为取决于 `node::map` （三参数）和 `node::com` 的实现；
   
   叶结点 `_apply`行为首先取决于 `node::map` （两参数）的实现；如果没有，取决于 `node::map` （三参数）的实现；如果还没有，取决于 `node::get` `node::op` `node::set` 的实现；
   
    `_pushup` 行为首先取决于 `node::pushup` （三参数）的实现；如果没有，取决于 `node::pushup` 两参数的实现；如果还没有，取决于 `node::get` `node::op` `node::set` 的实现；
   
    `modify` 行为取决于 `node::set` 的实现；
   
    `add` 行为取决于 `_apply` 的实现。
   
    `query` 、`min_left` 、`max_right` 、`kth` 函数访问结点时所访问的具体属性，由模板参数 `Getter` 决定，例如线段树中的基本元素都是一个 `int[8]` 短数组，而你想进行一次区间查询，获取区间内每个短数组的第一个元素的和。此时通过传递特定的 `Getter` 就可以只对每个短数组的第一个元素进行加和，理论复杂度变为八分之一。

#### 2.建立线段树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到线段树里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示线段树要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到线段树里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到线段树里。


#### 5.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点赋值的下标。

   输入参数 `const value_type &val​` ，表示赋的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示单点增值的下标。

   输入参数 `const modify_type &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.区间增值(add)

1. 数据类型

   输入参数 `size_type left​` ，表示增值的区间起始下标。

   输入参数 `size_type right` ，表示增值的区间结尾下标。（闭区间）

   输入参数 `const modify &modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.单点查询(query)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

   输入参数 `size_type i` ，表示查询的下标。

2. 时间复杂度

   $O(1)$ ，在基础线段树情况下；$O(\log n)$ ，在带懒惰标记的线段树情况下。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 9.区间查询(query)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 10.查询全部(query_all)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

2. 时间复杂度

   $O(1)$ 。

#### 11.树上二分查询右边界(max_right)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

   输入参数 `size_type left` ，表示左边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最大右边界。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`  ，均有 `judge(query(left, i))` 为真。而当 `i>r` 时，有 `judge(query(left, i))` 为假。显然，`r` 的最大值为 `m_size-1` 。

   如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`  。所以 `r` 的最小值为 `left-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 12.树上二分查询左边界(min_left)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

   输入参数 `size_type right` ，表示右边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最小左边界。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right))` 为真。而当 `i<l` 时，有 `judge(query(i, right))` 为假。显然，`l` 的最小值为 `0` 。

   如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`  。所以 `l` 的最大值为 `right+1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 13.查询第 $k$ 个元素(kth)

1. 数据类型

   模板参数 `typename Getter` ，表示结点属性访问器。

   输入参数 `typename Getter::value_type k​` ，表示要查询的元素从小到大的顺次。
   
   返回类型 `iterator` ，表示查询到的元素的下标位置和结点指针。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   当结点属性访问器的元素类型 `value_type` 为数字，且区间操作函数为加法的时候，本方法才有意义。

   顺次 `k​` 表示第 `k+1` 小，顺次 `0` 表示查询最小的元素。

   本函数没有进行参数检查，所以请自己确保 `k` 合法。（位于`[0, query_all())`）

#### 14.对叶子结点调用回调(do_for_node)

1. 数据类型

   模板参数 `bool ReadOnly` ，表示是否进行只读访问。

   输入参数 `size_type i` ，表示要访问的叶子结点。

   输入参数 `Call &&call` ，表示对叶结点调用的回调。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果只读不写，可以将 `ReadOnly` 设为 `true` ，以提高效率。

   当只读不写时，本方法返回回调函数的返回值。

#### 15.对所有位置的结点调用回调(do_for_each)

1. 数据类型

   输入参数 `Call &&call` ，表示在所有叶结点调用的回调。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   考虑将一颗线段树的每个位置的值输出，则需要依次调用若干次 `query` 函数，时间复杂度需要带个对数。

   此时可调用本方法，时间复杂度不带对数。

### 三、模板示例

```c++
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

void test_normal_tree() {
    // 线段树可以实现 st 表的所有统计功能，但是查询速度稍微慢一些
    // 考虑到线段树是常用数据结构，所以会尽量给出各种使用示范

    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

        // 建立一个区间最大值线段树
        // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto my_max = [](int x, int y) {
        return x > y ? x : y;
    };
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } my_max;
#endif
    auto tree_max = OY::make_ZkwTree(A, A + 10, my_max);
    cout << "max(A[3~6])     =" << tree_max.query(3, 6) << endl;

// 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间 gcd 树
    // 可以在参数框里写 lambda
    auto zkw_gcd = OY::make_ZkwTree(A, A + 10, [](auto x, auto y) { return std::gcd(x, y); });
    cout << zkw_gcd << endl;
    cout << "gcd(A[3~6])     =" << zkw_gcd.query(3, 6) << endl;
#else
    struct {
        int operator()(int x, int y) const { return std::gcd(x, y); }
    } mygcd;
    auto zkw_gcd = OY::make_ZkwTree(A, A + 10, mygcd);
    cout << zkw_gcd << endl;
    cout << "gcd(A[3~6])     =" << zkw_gcd.query(3, 6) << endl;
#endif

    // 建立一个区间位操作线段树
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto tree_bit_and = OY::make_ZkwTree(A, A + 10, std::bit_and<int>()); // 按位与
    cout << "bit_and(A[3~6]) =" << tree_bit_and.query(3, 6) << endl;

    auto tree_bit_or = OY::make_ZkwTree(A, A + 10, std::bit_or<int>()); // 按位或
    cout << "bit_or(A[3~6])  =" << tree_bit_or.query(3, 6) << endl;

    auto tree_bit_xor = OY::make_ZkwTree(A, A + 10, std::bit_xor<int>());
    cout << "bit_xor(A[3~6]) =" << tree_bit_xor.query(3, 6) << endl;

    // 建立一个区间乘 ST 表
    using MulNode = OY::ZKW::CustomNode<int64_t, std::multiplies<int64_t>>;
    OY::ZKW::Tree<MulNode> tree_mul(0);
    tree_mul.reset(A, A + 10);
    cout << "prod(A[3~6])    =" << tree_mul.query(3, 6) << endl;
    // 树上二分查询，从下标 3 开始，最多乘到哪个位置，乘积就会超过 2304
    int max_r = tree_mul.max_right(3, [](long long x) { return x <= 2304; });
    cout << "max_r = " << max_r << endl;
    // 树上二分查询，从下标 6 开始，最多向左乘到哪个位置，乘积就会超过 2304
    int min_l = tree_mul.min_left(6, [](long long x) { return x <= 2304; });
    cout << "min_l = " << min_l << endl;

    // 便利化措施：由于实际使用的时候，往往是求和树较多，所以无参构造为求和树
    OY::ZkwSumTree<int64_t> tree_default;
    tree_default.reset(A, A + 10);
    cout << "sum(A[0~9])     =" << tree_default.query(0, 9) << endl;
    cout << "A[4]            =" << tree_default.query(4) << endl;
    tree_default.modify(4, 18);
    cout << tree_default << endl;
    tree_default.add(4, 100);
    cout << tree_default << endl;
    // 查询排名第 0 的元素是谁
    cout << "A.kth(0)        =" << tree_default.kth(0).m_index << endl;
    // 查询排名第 15 的元素是谁
    cout << "A.kth(15)       =" << tree_default.kth(15).m_index << endl;
    // 查询排名第 16 的元素是谁
    cout << "A.kth(16)       =" << tree_default.kth(16).m_index << endl;
}

void test_lazy_tree() {
    // 带懒惰标记的线段树可以实现线段树的所有统计功能
    // 重复功能不再展示，主要展示构造方法和一些特有的功能

    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 默认无参构造就是日常用的最多的求和树
    OY::ZkwLazySumTree<int64_t> T(A, A + 10);
    cout << "sum(A[3~6])     =" << T.query(3, 6) << endl;
    // 对区间 [4,5] 赋予 10 的增量变化
    T.add(4, 5, 10);
    // 查看当前树的值
    cout << T << endl;
    cout << "sum(A[3~6])     =" << T.query(3, 6) << endl;
    // 查询排名第 54 的元素是谁
    cout << "A.kth(54)       =" << T.kth(54).m_index << endl;
    // 查询排名第 55 的元素是谁
    cout << "A.kth(55)       =" << T.kth(55).m_index << endl;

    // 增值函数、囤积函数可以和区间操作函数完全不同
    // 比如，统计用的是最大值函数，修改用的是加法
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto getmax = [](int x, int y) { return x > y ? x : y; };
    auto map = [](int x, int y, int) { return x + y; }; // 尤其注意 map 必须支持三个参数
    auto com = [](int x, int y) { return x + y; };
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; };
    } getmax;
    struct {
        int operator()(int x, int y, int) const { return x + y; };
    } map; // 尤其注意 map 必须支持三个参数
    struct {
        int operator()(int x, int y) const { return x + y; };
    } com;
#endif
    auto T_max_add = OY::make_lazy_ZkwTree<int, int, false>(A, A + 10, getmax, map, com);
    cout << "max(A[3~6])     =" << T_max_add.query(3, 6) << endl;
    // 对区间 [4,5] 赋予 10 的增量变化
    T_max_add.add(4, 5, 10);
    cout << T_max_add << endl;
    cout << "max(A[3~6])     =" << T_max_add.query(3, 6) << endl;

    // 维护一个整数序列，对其进行乘法、加法的修改，则可以把乘法加法的增量抽象为一个运算结点
#if CPP_STANDARD >= 202002L
    struct opNode {
        int mul = 1, add = 0;
    };
    auto op = [](int x, int y) { return x + y; };
    auto map2 = [](opNode x, int y, int size) { return y * x.mul + x.add * size; };
    auto com2 = [](opNode x, opNode y) { return opNode{y.mul * x.mul, y.add * x.mul + x.add}; };
    auto T2 = OY::make_lazy_ZkwTree<int, opNode, false>(A, A + 10, op, map2, com2, opNode());
    cout << T2 << endl;
    T2.add(2, 5, {1, 5});
    cout << T2 << endl;
    T2.add(4, 6, {2, 0});
    cout << T2 << endl;
    T2.add(3, 7, {1, 10});
    cout << T2 << endl;
#else
    struct opNode {
        int mul = 1, add = 0;
        static opNode make_opNode(int mul, int add) {
            opNode res;
            res.mul = mul, res.add = add;
            return res;
        };
    };
    struct {
        int operator()(int x, int y) const { return x + y; };
    } op;
    struct {
        int operator()(opNode x, int y, int size) const { return y * x.mul + x.add * size; };
    } map2; // 尤其注意 map2 必须支持三个参数
    struct {
        opNode operator()(opNode x, opNode y) const { return opNode::make_opNode(y.mul * x.mul, y.add * x.mul + x.add); }
    } com2;
    auto T2 = OY::make_lazy_ZkwTree<int, opNode, false>(A, A + 10, op, map2, com2);
    cout << T2 << endl;
    T2.add(2, 5, opNode::make_opNode(1, 5));
    cout << T2 << endl;
    T2.add(4, 6, opNode::make_opNode(2, 0));
    cout << T2 << endl;
    T2.add(3, 7, opNode::make_opNode(1, 10));
    cout << T2 << endl;
#endif
    cout << "sum(A[~])       =" << T2.query_all() << endl;
}

void tricks() {
    // 在 oj 做题时，往往要把一个连续数组构建成线段树。
    // 如果先存到 vector 再存线段树，未免有脱裤子放屁之嫌
    // 按我这么写即可在线段树内逐个输入数据，不需要外来的 vector
    OY::ZkwSumTree<int64_t> tree_by_cin(0, [](uint32_t) {
        int64_t num;
        cin >> num;
        return num;
    });
}

int main() {
    test_normal_tree();
    test_lazy_tree();
    tricks();
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
max(A[3~6])     =12
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
bit_xor(A[3~6]) =6
prod(A[3~6])    =2304
max_r = 6
min_l = 3
sum(A[0~9])     =84
A[4]            =8
[11, 5, 9, 12, 18, 4, 6, 15, 7, 7]
[11, 5, 9, 12, 118, 4, 6, 15, 7, 7]
A.kth(0)        =0
A.kth(15)       =1
A.kth(16)       =2
11 5 9 12 8 4 6 15 7 7
sum(A[3~6])     =30
[11, 5, 9, 12, 18, 14, 6, 15, 7, 7]
sum(A[3~6])     =50
A.kth(54)       =4
A.kth(55)       =5
max(A[3~6])     =12
[11, 5, 9, 12, 18, 14, 6, 15, 7, 7]
max(A[3~6])     =18
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
[11, 5, 14, 17, 13, 9, 6, 15, 7, 7]
[11, 5, 14, 17, 26, 18, 12, 15, 7, 7]
[11, 5, 14, 27, 36, 28, 22, 25, 7, 7]
sum(A[~])       =182

```

