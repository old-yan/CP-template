### 一、模板类别

​	数据结构： `AVL` 树。

​	练习题目：

1. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
2. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
3. [P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
4. [P3879 [TJOI2010] 阅读理解](https://www.luogu.com.cn/problem/P3879)
5. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
6. [P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
7. [P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
8. [数据结构](https://ac.nowcoder.com/acm/problem/276699)
9. [Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/242)


### 二、模板功能


#### 1.建立平衡树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号、子树大小的变量类型。

   模板参数 `template <typename> typename NodeWrapper` ，表示树中的结点结构体模板类，需传递一个 `CRTP` 基类。

   模板参数 `template <typename> typename BufferType` ，表示内存池类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：** 以下内容中注意**连接**和**合并**的不同。连接表示两棵树值域不相交，前树的最大值小于（等于）后树的最小值，这种情况下的树的融合。合并表示两棵树值域可能相交的情况下的树的融合。在代码中，称连接为 `join` ，称合并为 `merge` 。

   对于算法比赛来说，平衡二叉树可以有多种备选类型。接下来我们从多个方面进行分析：

   1. 论增删改查速度，应以 `RBT` ， `AVL` ， `SBT` 的速度最快，同为第一梯队。而替罪羊树、 `splay` 、有旋 `treap` 、无旋 `treap` 依次排在之后
   2. 论形态，只有 `splay` 的树形态会特别失衡，其他树的树高都会维持在对数高度。这表明 `splay` 难以进行可持久化，且难以进行细微的树中自由操作，很容易时间复杂度超标。
   3. 论分裂/连接功能， `splay` 和无旋 `treap` 支持对数时间复杂度的分裂和连接，而其他二叉树实现此功能均较为复杂（ `AVL` 也可以实现）。
   4. 论启发式合并速度，在启发式合并任务中，`splay` 和无旋 `treap` 的时间复杂度为 $O(n\log n)$ ，而其余平衡二叉树的时间复杂度为 $O(n\log^2 n)$ 。
   5. 论区间复制，无旋 `treap` 由于复制结点时会复制结点内的随机数，导致多次复制之后时间复杂度上升。而 `AVL` 不会有此问题。

   综上所述，各种平衡树各有优劣。最终选择 `AVL` 作为平衡二叉树模板。

   本模板类名为 `Tree` ，为可重集合。如果想实现去重集合的功能，应当在插入元素前进行元素有无的判断。

   模板参数 `NodeWrapper` 类型须包含一个用于传递子类类型的模板参数。对于基础平衡树来说，结点须满足以下要求：

   1. 声明 `key_type` 为键值类型；
   2. 实现成员函数 `set` ，接受一个 `key_type` 参数，将此值赋给本结点；
   3. 实现成员函数 `get` ，返回本结点的键值。
   
   除此之外可以自定义别的变量。当需要提供区间修改功能时，须添加 `pushdown` 方法；当需要提供区间查询功能时， `NodeWrapper` 类型须添加 `pushup` 方法；如果只想使用名次树，使用默认的 `OY::AVL::BaseNodeWrapper<Tp>::type` 即可。如果想自定义排序方式， `NodeWrapper` 类型需添加 `comp` 静态方法。

#### 2.从有序区间建立平衡树(from_sorted)

1. 数据类型

   输入参数 `Iterator first` ，表示区间头。

   输入参数 `Iterator last` ，表示区间尾。（开区间）

   输入参数 `Modify &&modify` ，表示插入结点后对结点的初始化修改。默认为 `Ignore` 。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本方法从一个元素已经有序的区间线性构建平衡树。

#### 3.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


#### 4.根据值插入(insert_by_key)

1. 数据类型：

   参数 `const key_type &key`​ ，表示要插入的键值。

   参数 `Modify &&modify` ，表示要对结点进行的修改。建议传递匿名函数。默认为无任何操作。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中插入一个新元素。
   
   如果树中已有同值的元素，新插入的元素会排在后面。
   
   参数 `modify` 是为了对新插入的结点进行一些自由的初始化。

#### 5.根据位置插入(insert_by_rank)

1. 数据类型：

   参数 `const key_type &key`​ ，表示要插入的值。

   参数 `size_type k` ，表示要插入的位置下标。

   参数 `Modify &&modify` ，表示要对结点进行的修改。建议传递匿名函数。默认为无任何操作。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法在树中插入一个新元素，使其成为中序遍历的 `k` 号元素。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()]` 。
   
   参数 `modify` 是为了对新插入的结点进行一些自由的初始化。

#### 6.根据值删除(erase_by_key)

1. 数据类型：

   参数 `const key_type &key`​ ，表示要删除的键值。

   返回类型 `bool` ，表示是否删除元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中找到并删除一个元素。

   如果找不到相应的元素，返回 `false` 。

#### 7.根据位置删除(erase_by_rank)

1. 数据类型：

   参数 `size_type k`​ ，表示要删除的位置下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法删除树的中序遍历的 `k` 号元素。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()-1]` 。

#### 8.根据值修改结点(modify_by_key)

1. 数据类型：

   参数 `const key_type &key`​ ，表示要修改的结点的值。

   参数 `Modify &&modify` ，表示要对结点进行的修改。建议传递匿名函数。

   返回类型 `bool` ，表示是否修改结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中找到并修改一个结点。

   如果找不到相应的元素，返回 `false` 。

   一般而言，本方法是对 `m_key` 之外的属性进行修改， `m_key` 本身作为排序的依据不应被修改。

#### 9.根据位置修改结点(modify_by_rank)

1. 数据类型：

   参数 `size_type k`​ ，表示要删除的位置下标。

   参数 `Modify &&modify` ，表示要对结点进行的修改。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法找到树的中序遍历的 `k` 号元素并进行修改。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()-1]` 。

#### 10.修改或者插入(modify_or_insert)

1. 数据类型：

   参数 `const key_type &key`​ ，表示要修改或插入的结点的值。

   参数 `Modify &&modify` ，表示要对结点进行的修改。建议传递匿名函数。

   返回类型 `bool` ，表示是否插入了结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中找到并修改一个结点。如果没找到，则插入一个新结点，再做修改。

   一般而言，本方法是对 `m_key` 之外的属性进行修改， `m_key` 本身作为排序的依据不应被修改。

#### 11.根据值分裂(split_by_key)

1. 数据类型

   参数 `const key_type &key` ，表示进行分裂处的值。

   返回类型 `Multiset` ，表示分裂得到的新树。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法在原树中保留小于 `val` 的元素，将所有大于等于 `val` 的元素分裂到新树中。

#### 12.根据位置分裂(split_by_rank)

1. 数据类型

   参数 `size_type k` ，表示进行分裂处的位置下标。

   返回类型 `Multiset` ，表示分裂得到的新树。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法将原树中序遍历的 `k` 号之间的元素保留，将 `k` 号及之后的元素分裂到新树中。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()]` 。

#### 13.连接另一棵树(join)

1. 数据类型

   参数 `tree_type &other` ，表示要连接的树。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法要求本树与要连接的树的值域范围不存在交叉；即，先进行本树的中序遍历，再进行要连接的树的中序遍历，所得序列不降。

   连接后， `other` 树失效。

#### 14.获取根结点(root)

1. 数据类型

   返回类型 `node*` ，表示根结点。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；空树时返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 15.获取树大小(size)

1. 数据类型

   返回类型 `size_type`，表示树大小。

2. 时间复杂度

    $O(1)$ 。

#### 16.查询树是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示树是否为空。

2. 时间复杂度

    $O(1)$ 。

#### 17.根据位置获取结点(kth)

1. 数据类型

   输入参数 `size_type k` ，表示要获取的结点的位置下标。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

#### 18.树上向左二分(min_left)

1. 数据类型

   模板参数 `typename Getter` ，表示属性获取器。

   输入参数 `size_type right` ，表示二分的起始右边界下标。

   输入参数 `Judger &&judge` ，表示二分过程中的判断函数。

   返回类型 `size_type` ，表示二分左边界下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   模板参数 `Getter` 需要定义 `value_type` ，表示二分的值的类型；重载圆括号，返回初始值； `tree` 方法一参为指针，二参为值类型，返回 `void` ，将指针的子树值添加到二参； `node` 方法一参为指针，二参为值类型，返回 `void` ，将指针的单点值添加到二参。

#### 19.树上向右二分(max_right)

1. 数据类型

   模板参数 `typename Getter` ，表示属性获取器。
   
   输入参数 `size_type left` ，表示二分的起始左边界下标。

   输入参数 `Judger &&judge` ，表示二分过程中的判断函数。

   返回类型 `size_type` ，表示二分右边界下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   模板参数 `Getter` 需要定义 `value_type` ，表示二分的值的类型；重载圆括号，返回初始值； `tree` 方法一参为值类型，二参为指针，返回 `void` ，将指针的子树值添加到一参； `node` 方法一参为值类型，二参为指针，返回 `void` ，将指针的单点值添加到一参。

#### 20.获取某值的排名(rank)

1. 数据类型

   输入参数 `const Tp &val` ，表示值。

   返回类型 `size_type` ，表示值对应的排名。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   返回的排名基于 `0` ，取值范围为 `[0, size()]` 。

#### 21.根据值获取前驱结点(smaller_bound)

1. 数据类型

   输入参数 `const key_type &key` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无前驱结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 22.根据值获取lower_bound(lower_bound)

1. 数据类型

   输入参数 `const key_type &key` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无相应结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 23.根据值获取upper_bound(upper_bound)

1. 数据类型

   输入参数 `const key_type &key` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无相应结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 24.对某子树调用回调(do_for_subtree)

1. 数据类型

   输入参数 `size_type left` ，表示子树的最左元素的排名。

   输入参数 `size_type right` ，表示子树的最右元素的排名。

   输入参数 `Callback &&call` ，表示对子树的修改。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法实际上是对其它方法的封装，可以理解为先通过两次 `split` 取出中间的树，（对其根结点）进行修改后，再把三棵树合并回去。

#### 25.原地对子树调用回调(do_for_subtree_inplace)

1. 数据类型

   输入参数 `size_type left` ，表示子树的最左元素的排名。

   输入参数 `size_type right` ，表示子树的最右元素的排名。

   输入参数 `NodeCallback &&node_call` ，表示对单个结点的修改。

   输入参数 `TreeCallback &&tree_call` ，表示对子树的修改。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法效果等同于 `do_for_subtree` ，不过并不需要把要操作的区间从原树上拆下来再操作，所以效率会更高。

   正因为是原地操作，所以并不能只传递一个回调函数，而是要传递两种回调函数，一种是单独修改单个结点的，一种是修改子树的。试想，如果将元素 `a` 到 `g` 共七个元素插入到树中，且树形态平衡。以元素 `d` 为根，左、右个三个元素。此时若想对 `a` 到 `d` 进行一个区间修改，那么可以通过对 `d` 进行单独的修改、对 `b` 进行一个子树修改来完成。

#### 26.对所有结点调用回调(do_for_each)

1. 数据类型

   输入参数 `Call &&call` ，表示在所有结点处要调用的回调。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   考虑将一颗树的所有元素输出，则需要依次调用若干次 `kth` 函数，时间复杂度需要带个对数。

   显然的，我们可以进行一次全树的中序遍历，在中序遍历的同时进行输出，这样时间复杂度不带对数。

   当然，本函数不仅仅适用于输入元素，可以填写任何回调做任何事。

#### 27.合并另一棵树(merge)

1. 数据类型

   输入参数 `tree_type &other` ，表示要合并的树。
   
   输入参数 `Func &&func` ，表示要对值相同的结点进行的合并操作。默认为 `Ignore` 类型的默认实例。

2. 时间复杂度

   可能为 $O(\log n\sim \log^2 n)$ 。

3. 备注

   本方法允许本树与要合并的树的值域范围存在交叉。
   
   当 `Func &&func` 为 `Ignore` 类型时，不会对值相同的结点进行合并，换句话说树为 `multiset` ；否则，要求输入的两颗树为 `set` ，且本函数会对输入的两棵树的值相同的结点进行合并，以保证结果仍为 `set` 。

   合并后， `other` 树清空。

### 三、模板示例

```c++
#include "DS/AVL.h"
#include "IO/FastIO.h"

/*
 * 普通名次树用法
 */
void test() {
    cout << "test of normal avl:\n";
    // 想指定自定义的元素排序规则和结点总数的话，就传递参数。否则可以直接用默认的
    OY::AVLMultiset<int, std::less<int>> S;
    using node = decltype(S)::node;
    S.insert_by_key(400);
    S.insert_by_key(300);
    S.insert_by_key(200);
    S.insert_by_key(100);
    S.insert_by_key(200);
    S.insert_by_key(200);
    S.insert_by_key(500);
    S.insert_by_key(400);
    cout << S << '\n';

    // 如果使用 insert_by_rank，可以在指定位置插入元素。但是可能会破坏树的有序性质
    S.insert_by_rank(999, 2);
    cout << S << '\n';

    // 删除元素也有按照值和按照位置两种方式
    S.erase_by_rank(2);
    cout << S << '\n';

    S.erase_by_key(200);
    cout << S << '\n';

    // 树的分裂也有按照值和按照位置两种方式
    auto S2 = S.split_by_rank(3);
    cout << S << ' ' << S2 << '\n';
    // 注意， join 之后，S2 就不可用了
    S.join(S2);
    cout << S << '\n';

    auto S3 = S.split_by_key(400);
    cout << S << ' ' << S3 << '\n';
    // 注意， join 之后，S3 就不可用了
    S.join(S3);
    cout << S << '\n';

    // 如果两棵树的值域有交错，可以使用 merge 进行合并
    decltype(S) S4;
    S4.insert_by_key(50);
    S4.insert_by_key(250);
    S4.insert_by_key(550);
    cout << S << ' ' << S4 << '\n';
    // 注意， merge 之后，S4 就不可用了
    S4.do_for_each([&](node *p) {
        p->m_lc = p->m_rc = 0;
        S.insert_node_by_key(p);
    });
    cout << S << '\n';

    // root 获取根节点
    auto root = S.root();
    if (!root->is_null()) cout << "root = " << root->get() << '\n';

    // kth 按照位置获取结点
    auto p = S.kth(4);
    cout << "kth(4) = " << p->get() << '\n';

    // rank 按照值获取排名
    auto k = S.rank(250);
    cout << "rank(250) = " << k << '\n';

    // smaller_bound 按照值获取前驱结点
    auto sb = S.smaller_bound(250);
    if (!sb->is_null()) cout << "smaller_bound(250) = " << sb->get() << '\n';

    // lower_bound 按照值获取前驱结点
    auto lb = S.lower_bound(250);
    if (!lb->is_null()) cout << "lower_bound(250) = " << lb->get() << '\n';

    // upper_bound 按照值获取前驱结点
    auto ub = S.upper_bound(250);
    if (!ub->is_null()) cout << "upper_bound(250) = " << ub->get() << '\n';

    // 线性遍历全树
    int index = 0;
    S.do_for_each([&](node *p) { cout << "No." << index++ << ": " << p->get() << endl; });

    cout << '\n';
}

template <typename Node>
struct node_pushup {
    // 必须声明 key_type
    using key_type = int;
    int m_key;
    int m_sum;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 这个方法名字必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_key;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
};
/*
 * 如何利用 pushup 进行区间查询
 */
void test_pushup() {
    cout << "test of pushup avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushup> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想查询 {2000,4000,3000} 的和
    auto S3 = S.split_by_rank(5);
    auto S2 = S.split_by_rank(2);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    cout << "sum of " << S2 << " = " << S2.root()->m_sum << '\n';
    // 查询完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct node_pushdown {
    // 必须声明 key_type 和 value_type
    using key_type = int;
    int m_key;
    int m_inc;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 这个方法名字必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->m_key += m_inc, lchild->m_inc += m_inc;
            if (!rchild->is_null()) rchild->m_key += m_inc, rchild->m_inc += m_inc;
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushdown 进行区间查询
 */
void test_pushdown() {
    cout << "test of pushdown avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->m_inc += 100, S2.root()->m_key += 100;
    // 修改完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct node_pushup_pushdown {
    // 必须声明 key_type
    using key_type = int;
    int m_key;
    int m_inc;
    int m_sum;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 方便起见，多写一个 add 方法。本方法名字当然可以随便起
    void add(int v) {
        m_key += v, m_inc += v, m_sum += v * ((Node *)this)->m_sz;
    }
    // 这个方法名字必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_key;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
    // 这个方法名字必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushup+pushdown 进行区间修改和查询
 */
void test_pushup_pushdown() {
    cout << "test of pushup+pushdown avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushup_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->add(100);
    // 修改完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    // 假如我们想查询 {2000,4000,3000} 的和
    S3 = S.split_by_rank(5);
    S2 = S.split_by_rank(2);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    cout << "sum of " << S2 << " = " << S2.root()->m_sum << '\n';
    // 查询完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct count_node {
    // 必须声明 key_type
    using key_type = std::string;
    std::string m_key;
    int m_count = 0;
    // 必须有 get set 方法
    void set(std::string key) { m_key = key; }
    std::string get() const { return m_key; }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const count_node<Node> &x) {
        return out << "(" << x.m_key << "," << x.m_count << ")";
    }
};
/*
 * 如何利用 avl 写一个字典/哈希表/counter
 */
void test_counter() {
    cout << "test of avl counter:\n";
    // 假如我们的这个字典统计水果数量
    OY::AVL::Tree<count_node> S;
    using node = decltype(S)::node;
    S.insert_by_key("apple");
    S.insert_by_key("orange");
    S.insert_by_key("banana");
    cout << S << '\n';
    // 默认插入时 m_count 为零，需要手动修改
    S.modify_by_key("apple", [](node *p) { p->m_count = 10; });
    S.modify_by_key("orange", [](node *p) { p->m_count = 5; });
    S.modify_by_key("banana", [](node *p) { p->m_count = 8; });
    cout << S << '\n';

    OY::AVL::Tree<count_node> S2;
    S2.insert_by_key("peach", [](node *p) { p->m_count = 20; });
    S2.insert_by_key("melon", [](node *p) { p->m_count = 3; });
    S2.insert_by_key("apple", [](node *p) { p->m_count = 1; });
    S2.insert_by_key("orange", [](node *p) { p->m_count = 7; });
    cout << S2 << '\n';

    // 我们希望两个 counter 合并的时候，同类的 m_count 相加
    cout << S << ' ' << S2 << '\n';
    S2.do_for_each([&](node *p) {
        p->m_lc = p->m_rc = 0;
        if (!S.modify_by_key(p->get(), [&](node *q) { q->m_count += p->m_count; })) S.insert_node_by_key(p);
    });
    cout << S << '\n';
    cout << '\n';
}

/*
借用 make_AVL 制造具有类似线段树功能的平衡树，但是区间修改和区间查询只针对 key 属性
*/
void test_custom() {
    /*
    类似普通线段树
    这是一颗乘法统计树
    */
    auto S = OY::make_AVL<int, std::less<int>>(std::multiplies<int>());
    S.insert_by_rank(30, 0);
    S.insert_by_rank(20, 1);
    S.insert_by_rank(50, 2);
    S.insert_by_rank(10, 3);
    S.insert_by_rank(7, 2);
    auto S2 = S.split_by_rank(2);
    cout << S << ' ' << S.root()->m_info << '\n';
    cout << S2 << ' ' << S2.root()->m_info << '\n';
    S.join(S2);
    cout << S << ' ' << S.root()->m_info << '\n';

    /*
    类似带懒惰标记的线段树
    这是一颗支持区间乘的乘积树
    */

    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto op = [](double x, double y) { return x * y; };
    auto map = [](double x, double y, int size) { return y * std::pow(x, size); };
    auto com = [](double x, double y) { return x * y; };
    auto S3 = OY::make_lazy_AVL<double, double, true, std::less<double>>(op, map, com, 1);
#else
    struct {
        double operator()(double x, double y) const { return x * y; }
    } op;
    struct {
        double operator()(double x, double y, int size) const { return y * std::pow(x, size); }
    } map;
    struct {
        double operator()(double x, double y) const { return x * y; }
    } com;
    auto S3 = OY::make_lazy_AVL<double, double, true, std::less<double>>(op, map, com, 1);
#endif
    S3.insert_by_rank(3.0, 0);
    S3.insert_by_rank(2.0, 1);
    S3.insert_by_rank(5.0, 2);
    S3.insert_by_rank(1.0, 3);
    S3.insert_by_rank(7.0, 2);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
    auto S4 = S3.split_by_rank(2);
    S4.root()->modify(2.0);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
    cout << S4 << ' ' << S4.root()->m_info << '\n';
    S3.join(S4);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
}

int main() {
    test();
    test_pushup();
    test_pushdown();
    test_pushup_pushdown();
    test_counter();
    test_custom();
}
```

```
#输出如下
test of normal avl:
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 999, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200} {300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300} {400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500} {50, 250, 550}
{50, 100, 200, 200, 250, 300, 400, 400, 500, 550}
root = 300
kth(4) = 250
rank(250) = 4
smaller_bound(250) = 200
lower_bound(250) = 250
upper_bound(250) = 300
No.0: 50
No.1: 100
No.2: 200
No.3: 200
No.4: 250
No.5: 300
No.6: 400
No.7: 400
No.8: 500
No.9: 550

test of pushup avl:
{5000, 1000} {2000, 4000, 3000} {2000}
sum of {2000, 4000, 3000} = 9000
{5000, 1000, 2000, 4000, 3000, 2000}

test of pushdown avl:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}

test of pushup+pushdown avl:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}
{5000, 1100} {2100, 4100, 3000} {2000}
sum of {2100, 4100, 3000} = 9200
{5000, 1100, 2100, 4100, 3000, 2000}

test of avl counter:
{(apple,0), (banana,0), (orange,0)}
{(apple,10), (banana,8), (orange,5)}
{(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,10), (banana,8), (orange,5)} {(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,11), (banana,8), (melon,3), (orange,12), (peach,20)}

{30, 20} 600
{7, 50, 10} 3500
{30, 20, 7, 50, 10} 2100000
{3.000000, 2.000000, 7.000000, 5.000000, 1.000000} 210.000000
{3.000000, 2.000000} 6.000000
{14.000000, 10.000000, 2.000000} 280.000000
{3.000000, 2.000000, 14.000000, 10.000000, 2.000000} 1680.000000

```

