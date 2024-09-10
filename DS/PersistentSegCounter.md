### 一、模板类别

​	数据结构：可持久化基于线段树的 `Counter` 。

​	练习题目：

1. [P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
2. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
3. [P5283 [十二省联考 2019] 异或粽子](https://www.luogu.com.cn/problem/P5283)

### 二、模板功能

​		本数据结构的功能与实现与 `SegCounter` 几乎一致，在此之外仅仅添加了可持久化的功能。所以相同部分的内容不再介绍。

​		在可持久化数据结构中，不同版本的数据结构可能会共享很多数据，因此，当某一个版本进行写操作时，必须将涉及到的数据拷贝出来一份再进行修改，以避免对其他版本造成影响。然而，在进行单纯的读操作时，并不需要将涉及到的数据拷贝出来再进行读。所以，本数据结构新增了模板参数 `bool Lock` ，表示是否含有锁，如果没有锁，那么无论读还是写都会引发拷贝操作；如果有锁，那么可以通过静态函数 `lock` 和 `unlock` 进行状态切换，当 `lock` 之后，树上操作不会进行数据拷贝； `unlock` 之后，树上操作会引起数据拷贝。默认状态为已上锁，请在写操作前解锁，并在写操作完成后上锁。

#### 1.拷贝线段树(copy)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。

   **注意：**本函数名“拷贝”并不等价于上文中的数据拷贝。本函数只涉及根结点的拷贝；数据拷贝指的是从根到叶结点的链的拷贝。
   

### 三、模板示例

```c++
#include "DS/PersistentSegCounter.h"
#include "IO/FastIO.h"

void test() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    using PerCounter = OY::PerSEGCNT::Table<uint32_t, int, true, false, false>;

    std::vector<PerCounter> counters;
    // counters[0] 作为基础
    counters.push_back({});
    for (int i = 0; i < 5; i++) {
        counters.push_back(counters.back().copy());
        counters.back().add(A[i], 1);
    }

    // 两个 counter 可以做差，得到一个差视图
    cout << "diff of counters[1] and counters[4]" << counters[4] - counters[1] << endl;

    // 输出 [200,300,400] 里和 150 进行异或的最大异或结果
    cout << "max_bitxor with 150: " << (counters[4] - counters[1]).max_bitxor(150) << endl;

    // 输出 [200,300,400] 里和 150 进行异或的最小异或结果
    cout << "min_bitxor with 150: " << (counters[4] - counters[1]).min_bitxor(150) << endl;

    // 输出 [200,300,400] 里的最大键
    cout << "max: " << (counters[4] - counters[1]).max() << endl;

    // 输出 [200,300,400] 里的最小键
    cout << "min: " << (counters[4] - counters[1]).min() << endl;

    // 输出 [200,300,400] 里，键位于 [200,300] 的个数
    cout << "count(200~300) = " << (counters[4] - counters[1]).query(200, 300) << endl;

    // 输出 [200,300,400] 里，和 150 异或后，位于 [200,300] 的个数
    cout << "count([200<=x^150<=300]) = " << (counters[4] - counters[1]).query_bitxor(200, 300, 150) << endl;
}

int main() {
    test();
}
```

```
#输出如下
diff of counters[1] and counters[4]{200*1,300*1,400*1}
max_bitxor with 150: 442
min_bitxor with 150: 94
max: 400
min: 200
count(200~300) = 2
count([200<=x^150<=300]) = 1

```

