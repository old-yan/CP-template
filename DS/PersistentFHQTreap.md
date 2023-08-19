### 一、模板类别

​	数据结构：可持久化 `FHQTreap` 树（可持久化无旋 `treap` ）。

### 二、模板功能

​		本数据结构的功能与实现与 `FHQTreap` 几乎一致，在此之外仅仅添加了可持久化的功能。所以与 `FHQTreap` 相同部分的内容不再介绍。

#### 1.拷贝(copy)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。

### 三、模板示例

```c++
#include "DS/PersistentFHQTreap.h"
#include "IO/FastIO.h"

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

int main() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 直接把 FHQTreap 文档里的支持区间加的求和树给抄过来
    OY::PerFHQTreap::Multiset<int, node_pushup_pushdown> T;
    for (int a : A) T.insert_by_val(a);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    T2.insert_by_val(250);

    auto T3 = T2.copy();
    T3.insert_by_val(150);

    auto T4 = T.copy();
    T4.root()->add(T4.root(), 1000);

    T.root()->add(T.root(), 100000);
    cout << "T :" << T << endl;
    cout << "T2:" << T2 << endl;
    cout << "T3:" << T3 << endl;
    cout << "T4:" << T4 << endl;
}
```

```
#输出如下
{100, 200, 300, 400, 500}

T :{100100, 100200, 100300, 100400, 100500}
T2:{100, 200, 250, 300, 400, 500}
T3:{100, 150, 200, 250, 300, 400, 500}
T4:{1100, 1200, 1300, 1400, 1500}

```

