### 一、模板类别

​	数据结构： `FHQTreap` 树（无旋 `treap` ）。

### 二、模板功能


#### 1.建立平衡树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号、子树大小的变量类型。

   类型设定 `priority_type = uint32_t` ，表示树中结点的随机权重的变量类型。

   模板参数 `typename Tp` ，表示树中的键类型。

   模板参数 `typename Node` ，表示树中的结点结构体类型，默认为 `OY::Treap::BaseNode<Tp>` 。

   模板参数 `typename Compare` ，表示比较函数的类型，默认为 `std::less<Tp>` 。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1<<19` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：** 以下内容中注意**连接**和**合并**的不同。连接表示两棵树值域不相交，前树的最大值小于（等于）后树的最小值，这种情况下的树的融合。合并表示两棵树值域可能相交的情况下的树的融合。在代码中，我称连接为 `join` ，称合并为 `merge` 。

   对于算法比赛来说，平衡二叉树可以有多种备选类型。接下来我们从多个方面进行分析：

   1. 论速度，应以 `RBT` ， `AVL` ， `SBT` 的速度最快，同为第一梯队。而替罪羊树、 `splay` 、有旋 `treap` 、无旋 `treap` 依次排在之后。然而，最快的和最慢的树，速度也达不到两倍，实现良好的无旋 `treap` 速度甚至可以反超标准库的 `RBT` ，这表明各种树的速度影响程度并不大。
   2. 论形态，只有 `splay` 的树形态会特别失衡，其他树的树高都会维持在对数高度。这表明 `splay` 难以进行可持久化，且难以进行细微的树中自由操作，很容易时间复杂度超标。
   3. 论功能，只有 `splay` 和无旋 `treap` 支持对数时间复杂度的分裂和连接。在启发式合并任务中，`splay` 和无旋 `treap` 的时间复杂度为 $O(n\log n)$ ，而其余平衡二叉树的时间复杂度为 $O(n\log^2 n)$ 。

   综上所述，无旋 `treap` 是最适合算法比赛的平衡二叉树模板。

   本模板类名为 `Multiset` ，即可重集合。如果想实现去重集合的功能，应当在插入元素前进行元素有无的判断。

   模板参数 `Node` 类型必须包含一个名为 `m_val` 的 `Tp` 类型的成员变量，除此之外可以自定义别的变量。当需要提供区间修改功能时， `Node` 类型须添加 `pushdown` 方法；当需要提供区间查询功能时， `Node` 类型须添加 `pushup` 方法；如果只想使用名次树，使用默认的 `OY::Treap::BaseNode<Tp>` 即可。

#### 2.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


#### 3.根据值插入(insert_by_val)

1. 数据类型：

   参数 `const Tp &val`​ ，表示要插入的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中插入一个新元素。
   
   如果树中已有同值的元素，新插入的元素会排在后面。

#### 4.根据位置插入(insert_by_rank)

1. 数据类型：

   参数 `const Tp &val`​ ，表示要插入的值。

   参数 `size_type k` ，表示要插入的位置下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法在树中插入一个新元素，使其成为中序遍历的 `k` 号元素。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()]` 。

#### 5.根据值插入结点(insert_node_by_val)

1. 数据类型：

   参数 `node *x`​ ，表示要插入的结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中插入一个新元素。

   如果树中已有同值的元素，新插入的元素会排在后面。

   **注意：**本方法与 `insert_by_val` 方法作用相同，适用于想对结点的属性进行一些自定义初始化的情况。结点可以通过 `Multiset::_create` 方法获取，在修改属性之后进行 `insert_node_by_val` 。

#### 6.根据位置插入结点(insert_node_by_rank)

1. 数据类型：

   参数 `node *x`​ ，表示要插入的结点。

   参数 `size_type k` ，表示要插入的位置下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法在树中插入一个新元素，使其成为中序遍历的 `k` 号元素。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()]` 。

   **注意：**本方法与 `insert_by_rank` 方法作用相同，适用于想对结点的属性进行一些自定义初始化的情况。结点可以通过 `Multiset::_create` 方法获取，在修改属性之后进行 `insert_node_by_rank` 。

#### 7.根据值删除(erase_by_val)

1. 数据类型：

   参数 `const Tp &val`​ ，表示要删除的值。

   返回类型 `bool` ，表示是否删除元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中找到并删除一个元素。

   如果找不到相应的元素，返回 `false` 。

#### 8.根据位置删除(erase_by_rank)

1. 数据类型：

   参数 `size_type k`​ ，表示要删除的位置下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法删除树的中序遍历的 `k` 号元素。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()-1]` 。

#### 9.根据值修改结点(modify_by_val)

1. 数据类型：

   参数 `const Tp &val`​ ，表示要修改的结点的值。

   参数 `Modify modify` ，表示要对结点进行的修改。建议传递匿名函数。

   返回类型 `bool` ，表示是否修改结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照二叉搜索树的规则，在树中找到并修改一个结点。

   如果找不到相应的元素，返回 `false` 。

   一般而言，本方法是对 `m_val` 之外的属性进行修改， `m_val` 本身作为排序的依据不应被修改。

#### 10.根据位置修改结点(modify_by_rank)

1. 数据类型：

   参数 `size_type k`​ ，表示要删除的位置下标。

   参数 `Modify modify` ，表示要对结点进行的修改。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法找到树的中序遍历的 `k` 号元素并进行修改。

   下标 `k` 基于 `0` ，取值范围为 `[0, size()-1]` 。

#### 11.根据值分裂(split_by_val)

1. 数据类型

   参数 `const Tp &val` ，表示进行分裂处的值。

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

   参数 `Multiset other` ，表示要连接的树。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法要求本树与要连接的树的值域范围不存在交叉；即，先进行本树的中序遍历，再进行要连接的树的中序遍历，所得序列不降。

   连接后， `other` 被清空。

#### 14.合并另一棵树(merge)

1. 数据类型

   参数 `Multiset other` ，表示要合并的树。

   参数 `Func func` ，表示对同样键值的结点的处理方式，默认为 `Ignore` 。

2. 时间复杂度

   似为均摊 $O(\log n)$ 。理论上本方法的复杂度与两树值域的交错情况相关；而两树值域的交错，完全来源于插入元素和分裂操作，所以复杂度为均摊 $O(\log n)$ 。

3. 备注

   本方法对值域范围无要求。

   合并后， `other` 被清空。
   
   如果树中允许重复键值，参数 `func` 为默认的 `Ignore` ，不会对两棵树里的相同键值的结点采取特殊处理，而是统统合并到一颗树里。
   
   如果树中不允许重复键值，参数 `func` 需要传递一个函数，对两棵树里的相同键值的结点采取特殊处理，然后只保留其中的一个。

#### 15.获取根结点(root)

1. 数据类型

   返回类型 `node*` ，表示根结点。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；空树时返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 16.获取树大小(size)

1. 数据类型

   返回类型 size_type，表示树大小。

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

#### 18.获取某值的排名(rank)

1. 数据类型

   输入参数 `const Tp &val` ，表示值。

   返回类型 `size_type` ，表示值对应的排名。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   返回的排名基于 `0` ，取值范围为 `[0, size()]` 。

#### 19.根据值获取前驱结点(smaller_bound)

1. 数据类型

   输入参数 `const Tp &val` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无前驱结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 20.根据值获取lower_bound(lower_bound)

1. 数据类型

   输入参数 `const Tp &val` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无相应结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

#### 21.根据值获取upper_bound(upper_bound)

1. 数据类型

   输入参数 `const Tp &val` ，表示值。

   返回类型 `node*` ，表示获取的结点。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   请勿通过本方法获取结点，然后对结点的属性进行修改。这样的修改不能一层一层自动 `pushup` 。

   返回的结点 `x` 可以通过 `x->is_null()` 判断是否为空结点；若无相应结点，返回空结点。

   **注意：**返回的空结点并不是 `nullptr` 。

### 三、模板示例

```c++
#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
 * 普通名次树用法
 */
void test() {
    cout << "test of normal treap:\n";
    OY::FHQTreap::Multiset<int> S;
    S.insert_by_val(400);
    S.insert_by_val(300);
    S.insert_by_val(200);
    S.insert_by_val(100);
    S.insert_by_val(200);
    S.insert_by_val(200);
    S.insert_by_val(500);
    S.insert_by_val(400);
    cout << S << '\n';

    // 如果使用 insert_by_rank，可以在指定位置插入元素。但是可能会破坏树的有序性质
    S.insert_by_rank(999, 2);
    cout << S << '\n';

    // 删除元素也有按照值和按照位置两种方式
    S.erase_by_rank(2);
    cout << S << '\n';

    S.erase_by_val(200);
    cout << S << '\n';

    // 树的分裂也有按照值和按照位置两种方式
    auto S2 = S.split_by_rank(3);
    cout << S << ' ' << S2 << '\n';
    S.join(S2);
    cout << S << ' ' << S2 << '\n';

    auto S3 = S.split_by_val(400);
    cout << S << ' ' << S3 << '\n';
    S.join(S3);
    cout << S << ' ' << S3 << '\n';

    // 如果两棵树的值域有交错，可以使用 merge 进行合并
    OY::FHQTreap::Multiset<int> S4;
    S4.insert_by_val(50);
    S4.insert_by_val(250);
    S4.insert_by_val(550);
    cout << S << ' ' << S4 << '\n';
    S.merge(S4);
    cout << S << ' ' << S4 << '\n';

    // root 获取根节点
    auto root = S.root();
    if (!root->is_null()) cout << "root = " << root->m_val << '\n';

    // kth 按照位置获取结点
    auto p = S.kth(4);
    cout << "kth(4) = " << p->m_val << '\n';

    // rank 按照值获取排名
    auto k = S.rank(250);
    cout << "rank(250) = " << k << '\n';

    // smaller_bound 按照值获取前驱结点
    auto sb = S.smaller_bound(250);
    if (!sb->is_null()) cout << "smaller_bound(250) = " << sb->m_val << '\n';

    // lower_bound 按照值获取前驱结点
    auto lb = S.lower_bound(250);
    if (!lb->is_null()) cout << "lower_bound(250) = " << lb->m_val << '\n';

    // upper_bound 按照值获取前驱结点
    auto ub = S.upper_bound(250);
    if (!ub->is_null()) cout << "upper_bound(250) = " << ub->m_val << '\n';

    cout << '\n';
}

struct node_pushup {
    int m_val;
    int m_sum;
    // 这个方法名字必须叫 pushup
    template <typename Node>
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
};
/*
 * 如何利用 pushup 进行区间查询
 */
void test_pushup() {
    cout << "test of pushup treap:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::FHQTreap::Multiset<int, node_pushup> S;
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

struct node_pushdown {
    int m_val;
    int m_inc;
    // 这个方法名字必须叫 pushdown
    template <typename Node>
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->m_val += m_inc, lchild->m_inc += m_inc;
            if (!rchild->is_null()) rchild->m_val += m_inc, rchild->m_inc += m_inc;
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushdown 进行区间查询
 */
void test_pushdown() {
    cout << "test of pushdown treap:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::FHQTreap::Multiset<int, node_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->m_inc += 100, S2.root()->m_val += 100;
    // 修改完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

struct node_pushup_pushdown {
    int m_val;
    int m_inc;
    int m_sum;
    // 方便起见，多写一个 add 方法。本方法名字当然可以随便起
    template <typename Node>
    void add(Node *self, int v) {
        m_val += v, m_inc += v, m_sum += v * self->m_size;
    }
    // 这个方法名字必须叫 pushup
    template <typename Node>
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
    // 这个方法名字必须叫 pushdown
    template <typename Node>
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(lchild, m_inc);
            if (!rchild->is_null()) rchild->add(rchild, m_inc);
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushup+pushdown 进行区间修改和查询
 */
void test_pushup_pushdown() {
    cout << "test of pushup+pushdown treap:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::FHQTreap::Multiset<int, node_pushup_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->add(S2.root(), 100);
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

struct count_node {
    std::string m_val;
    int m_count = 0;
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const count_node &x) {
        return out << "(" << x.m_val << "," << x.m_count << ")";
    }
};
/*
 * 如何利用 treap 写一个字典/哈希表/counter
 */
void test_counter() {
    cout << "test of treap counter:\n";
    // 假如我们的这个字典统计水果数量
    OY::FHQTreap::Multiset<std::string, count_node> S;
    S.insert_by_val("apple");
    S.insert_by_val("orange");
    S.insert_by_val("banana");
    cout << S << '\n';
    // 默认插入时 m_count 为零，需要手动修改
    S.modify_by_val("apple", [](auto p) { p->m_count = 10; });
    S.modify_by_val("orange", [](auto p) { p->m_count = 5; });
    S.modify_by_val("banana", [](auto p) { p->m_count = 8; });
    cout << S << '\n';

    // 也可以直接插入初始化好的结点
    OY::FHQTreap::Multiset<std::string, count_node> S2;
    auto p = S2._create("peach");
    p->m_count = 20;
    S2.insert_node_by_val(p);
    p = S2._create("melon");
    p->m_count = 3;
    S2.insert_node_by_val(p);
    p = S2._create("apple");
    p->m_count = 1;
    S2.insert_node_by_val(p);
    p = S2._create("orange");
    p->m_count = 7;
    S2.insert_node_by_val(p);
    cout << S2 << '\n';

    // 我们希望两个 counter 合并的时候，同类的 m_count 相加
    cout << S << ' ' << S2 << '\n';
    S.merge(S2, [](auto p1, auto p2) { p1->m_count += p2->m_count; });
    cout << S << ' ' << S2 << '\n';
    cout << '\n';
}

int main() {
    test();
    test_pushup();
    test_pushdown();
    test_pushup_pushdown();
    test_counter();
}
```

```
#输出如下
test of normal treap:
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 999, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200} {300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500} {}
{100, 200, 200, 300} {400, 400, 500}
{100, 200, 200, 300, 400, 400, 500} {}
{100, 200, 200, 300, 400, 400, 500} {50, 250, 550}
{50, 100, 200, 200, 250, 300, 400, 400, 500, 550} {}
root = 500
kth(4) = 250
rank(250) = 4
smaller_bound(250) = 200
lower_bound(250) = 250
upper_bound(250) = 300

test of pushup treap:
{5000, 1000} {2000, 4000, 3000} {2000}
sum of {2000, 4000, 3000} = 9000
{5000, 1000, 2000, 4000, 3000, 2000}

test of pushdown treap:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}

test of pushup+pushdown treap:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}
{5000, 1100} {2100, 4100, 3000} {2000}
sum of {2100, 4100, 3000} = 9200
{5000, 1100, 2100, 4100, 3000, 2000}

test of treap counter:
{(apple,0), (banana,0), (orange,0)}
{(apple,10), (banana,8), (orange,5)}
{(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,10), (banana,8), (orange,5)} {(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,11), (banana,8), (melon,3), (orange,12), (peach,20)} {}

```

