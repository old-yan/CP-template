### 一、模板类别

​	数据结构：配对堆

​	练习题目：

1. [P3261 [JLOI2015] 城池攻占](https://www.luogu.com.cn/problem/P3261)
2. [P3377 【模板】左偏树（可并堆）](https://www.luogu.com.cn/problem/P3377)
3. [P3378 【模板】堆](https://www.luogu.com.cn/problem/P3378)
4. [U361730 【模板】完全体·堆](https://www.luogu.com.cn/problem/U361730)

### 二、模板功能

#### 1.建堆

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号、子树大小的变量类型。

   模板参数 `template <typename> typename NodeWrapper` ，表示树中的结点结构体模板类，需传递一个 `CRTP` 基类。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   堆处理的问题为：如何在变化的数据流里维护最值元素。配对堆的效率较高。

   对于算法比赛来说，堆可以有多种备选类型。接下来我们从多个方面进行分析：

   1. 论速度，应以二叉堆和配对堆的速度最快，同为第一梯队。而左偏树相比之下稍慢，单独排在第二梯队。接下来，二项堆、冗余二项堆、 `thin` 堆等依次排在之后。这其中，第一梯队与第二梯队差距很小，第二梯队与之后的差距较大。
   2. 论动态扩容，除二叉堆外，其他堆本来都是以结点的形式组织起来的，只有二叉堆需要一块连续内存。也就是说，在面对未知的堆大小、可能同时存在多个堆的情况下，二叉堆必须使用 `vector` 作为内存，而其余的堆都可以如常使用结点内存池。所以二叉堆难以可持久化。
   3. 论合并效率，二叉堆只能启发性合并，但是其余的堆可以以对数时间复杂度进行合并。然而，这并不意味着二叉堆的实际速度慢，往往在涉及堆的合并的题目里，二叉堆的速度反而更快。
   4. 论稳定性，配对堆的时间复杂度为均摊复杂度，所以配对堆难以可持久化。

   综上所述，在不涉及可持久化时，配对树是最适合算法比赛的堆的模板；在涉及可持久化时，左偏树是最适合的堆的模板。由于在算法比赛中，可持久化可并堆为极小众需求，仅在 `K` 短路问题中涉及；所以常态下，以配对堆为最佳。

   模板参数 `NodeWrapper` 类型须包含一个用于传递子类类型的模板参数。对于基础的堆来说，结点须满足以下要求：

   1. 声明 `value_type` 为键值类型；
   2. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   3. 实现成员函数 `get` ，返回本结点的键值。
   
   除此之外可以自定义别的变量。当需要提供区间修改功能时，须添加 `pushdown` 方法；当需要提供区间查询功能时， `NodeWrapper` 类型须添加 `pushup` 方法；如果只想使用基础堆，使用默认的 `OY::PHeap::BaseNodeWrapper<Tp>::type` 即可。如果想自定义排序方式， `NodeWrapper` 类型需添加 `comp` 静态方法。

#### 2.根结点(root)

1. 数据类型

   返回类型 `node*` ，表示左偏树的根结点。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   对于基础堆来说，没有必要使用本方法。

   当需要使用懒惰修改及查询堆的聚合值时，才需要本方法获取根结点，然后在根节点中进行读写。

#### 3.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 4.判断是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示堆是否为空。

2. 时间复杂度

   $O(1)$ 。


#### 5.插入元素(push)

1. 数据类型：

   参数 `const value_type &key`​ ，表示要插入的键值。

   参数 `Modify &&modify` ，表示要对结点进行的修改。建议传递匿名函数。默认为无任何操作。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法按照堆的规则，在树中插入一个新元素。
   
   参数 `modify` 是为了对新插入的结点进行一些自由的初始化。

#### 6.弹出最值(pop)

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 7.查询最值(top)

1. 数据类型

   返回类型 `value_type` ，表示堆顶的值。

2. 时间复杂度

   $O(1)$ 。

#### 8.合并堆(join)

1. 数据类型

   参数 `Heap<NodeWrapper, BufferType> rhs` ，表示要合并的堆。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   连接后， `rhs` 树失效。


### 三、模板示例

```c++
#include "DS/PairHeap.h"
#include "IO/FastIO.h"

// 基础堆测试
void test_normal() {
    // 默认均为大根堆
    OY::VectorPairHeap<int, std::less<int>> big_root;
    for (int a : {1, 9, 3, 7, 5}) {
        big_root.push(a);
    }
    while (!big_root.empty()) {
        cout << big_root.top() << endl;
        big_root.pop();
    }

    // 传递比较参数，修改为字符串的小根堆
    OY::VectorPairHeap<std::string, std::greater<std::string>> small_root;
    for (std::string s : {"apple", "erase", "cat", "dog", "banana"}) {
        small_root.push(s);
    }
    while (!small_root.empty()) {
        cout << small_root.top() << endl;
        small_root.pop();
    }

    // 相比普通的堆，新增了 join 功能，用于将两个堆合并到一个堆
    OY::VectorPairHeap<int, std::less<int>> heap1;
    OY::VectorPairHeap<int, std::less<int>> heap2;
    for (int a : {1, 3, 5, 7, 9}) heap1.push(a);
    for (int a : {2, 4, 6, 8, 10}) heap2.push(a);
    heap1.join(heap2);
    while (!heap1.empty()) {
        cout << heap1.top() << ' ';
        heap1.pop();
        if (heap1.empty()) cout << endl;
    }
}

// 如何写出一个求全体和的堆
template <typename Node>
struct Node_pushup {
    using value_type = int;
    int m_val, m_sum;
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
};
void test_pushup() {
    OY::PHeap::Heap<Node_pushup> S;
    S.push(1);
    S.push(2);
    S.push(3);
    S.push(4);
    cout << S.root()->m_sum << endl;
}

// 如何写出一个支持全体增值的堆
template <typename Node>
struct Node_pushdown {
    using value_type = int;
    int m_val, m_inc;
    // 为方便起见，定义一个函数，同时让 m_val 和 m_inc 增值。本函数名随意
    void add(int inc) { m_val += inc, m_inc += inc; }
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
};
void test_pushdown() {
    OY::PHeap::Heap<Node_pushdown> S;
    S.push(1);
    S.push(2);
    S.push(3);
    S.push(4);
    S.root()->add(100);
    while (!S.empty()) cout << S.top() << ' ', S.pop();
    cout << endl;
}

// 自然而然的，我们可以写出既支持全体修改，也支持查询全体聚合值的堆
template <typename Node>
struct Node_pushup_pushdown {
    using value_type = int;
    int m_val, m_inc, m_size, m_sum;
    // 为方便起见，定义一个函数，同时让 m_val, m_inc, m_sum 增值。本函数名随意
    void add(int inc) { m_val += inc, m_inc += inc, m_sum += inc * m_size; }
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
    // 本方法名必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val, m_size = 1;
        if (!lchild->is_null()) m_sum += lchild->m_sum, m_size += lchild->m_size;
        if (!rchild->is_null()) m_sum += rchild->m_sum, m_size += rchild->m_size;
    }
};
void test_pushup_pushdown() {
    OY::PHeap::Heap<Node_pushup_pushdown> S1;
    for (int i = 0; i < 10; i++) S1.push(i * 200);
    OY::PHeap::Heap<Node_pushup_pushdown> S2;
    for (int i = 0; i < 10; i++) S2.push(i * 200 + 100);

    S1.root()->add(11);
    S2.root()->add(22);
    S1.join(S2);
    S1.root()->add(44);
    cout << S1.root()->m_sum << endl;
    while (!S1.empty()) cout << S1.top() << ' ', S1.pop();
    cout << endl;
}

int main() {
    test_normal();
    test_pushup();
    test_pushdown();
    test_pushup_pushdown();
}
```

```
#输出如下
9
7
5
3
1
apple
banana
cat
dog
erase
10 9 8 7 6 5 4 3 2 1 
10
104 103 102 101 
20210
1966 1855 1766 1655 1566 1455 1366 1255 1166 1055 966 855 766 655 566 455 366 255 166 55 

```

