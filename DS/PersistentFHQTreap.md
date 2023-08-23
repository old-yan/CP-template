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

int main() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 用最简单的方法制造一颗可持久化可区间加的求和树
    // op,map,com 中只有 map 必须要手写
    auto map = [](int x, int y, int size) { return y + x * size; };
    auto T = OY::make_lazy_PerFHQTreap<int, int, false>(std::plus<int>(), map, std::plus<int>());
    for (int a : A) T.insert_by_key(a);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    T2.insert_by_key(250);

    auto T3 = T2.copy();
    T3.insert_by_key(150);

    auto T4 = T.copy();
    T4.root()->modify(1000);

    T.root()->modify(100000);
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

