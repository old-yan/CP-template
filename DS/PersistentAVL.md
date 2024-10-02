### 一、模板类别

​	数据结构：可持久化 `AVL` 树。

​	练习题目：

1. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
2. [P3919 【模板】可持久化线段树 1（可持久化数组）](https://www.luogu.com.cn/problem/P3919)
3. [P5055 【模板】可持久化文艺平衡树](https://www.luogu.com.cn/problem/P5055)
4. [P5350 序列](https://www.luogu.com.cn/problem/P5350)
5. [P5586 [P5350] 序列 (加强版)](https://www.luogu.com.cn/problem/P5586)
6. [P8263 [Ynoi Easy Round 2020] TEST_8](https://www.luogu.com.cn/problem/P8263)


### 二、模板功能

​		本数据结构的功能与实现与 `AVL` 几乎一致，在此之外仅仅添加了可持久化的功能。所以与 `AVL` 相同部分的内容不再介绍。

​		在可持久化数据结构中，不同版本的数据结构可能会共享很多数据，因此，当某一个版本进行写操作时，必须将涉及到的数据拷贝出来一份再进行修改，以避免对其他版本造成影响。然而，在进行单纯的读操作时，并不需要将涉及到的数据拷贝出来再进行读。所以，本数据结构新增了模板参数 `bool Lock` ，表示是否含有锁，如果没有锁，那么无论读还是写都会引发拷贝操作；如果有锁，那么可以通过静态函数 `lock` 和 `unlock` 进行状态切换，当 `lock` 之后，树上操作不会进行数据拷贝； `unlock` 之后，树上操作会引起数据拷贝。默认状态为已上锁，请在写操作前解锁，并在写操作完成后上锁。

​		在支持区间修改的树里，即使是写操作也可能引起懒惰标记下传，所以一般模板参数 `Lock` 设为 `false` 。

#### 1.拷贝(copy)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。
   
   **注意：**本函数名“拷贝”并不等价于上文中的数据拷贝。本函数只涉及根结点的拷贝；数据拷贝指的是从根到叶结点的链的拷贝。

### 三、模板示例

```c++
#include "DS/PersistentAVL.h"
#include "IO/FastIO.h"

void test_no_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 用最简单的方法制造一颗可持久化可区间加的求和树
    // op,map,com 中只有 map 必须要手写
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto map = [](int x, int y, int size) { return y + x * size; };
    auto T = OY::make_lazy_PerAVL<int, int, false, false>(std::plus<int>(), map, std::plus<int>());
#else
    struct {
        int operator()(int x, int y, int size) const { return y + x * size; };
    } map;
    auto T = OY::make_lazy_PerAVL<int, int, false, false>(std::plus<int>(), map, std::plus<int>());
#endif
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

void test_with_lock() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    // 用最简单的方法制造一颗可持久化求和树
    auto T = OY::make_PerAVL<int, std::less<int>, true>(std::plus<int>());
    // 初始状态下只有一个版本，就算是写操作也没必要解锁
    for (int a : A) T.insert_by_key(a);
    cout << T << endl
         << endl;

    auto T2 = T.copy();
    // 静态函数解锁
    decltype(T2)::unlock();
    T2.insert_by_key(250);
    decltype(T2)::lock();

    auto T3 = T2.copy();
    // 可以通过普通函数方式调用解锁
    T3.unlock();
    T3.insert_by_key(150);
    T3.unlock();

    auto T4 = T.copy();
    // 同类型共享锁
    T2.unlock();
    T4.erase_by_rank(0);
    T3.lock();

    cout << "T :" << T << endl;
    cout << "T2:" << T2 << endl;
    cout << "T3:" << T3 << endl;
    cout << "T4:" << T4 << endl;
}

int main() {
    test_no_lock();
    test_with_lock();
}
```

```
#输出如下
{100, 200, 300, 400, 500}

T :{100100, 100200, 100300, 100400, 100500}
T2:{100, 200, 250, 300, 400, 500}
T3:{100, 150, 200, 250, 300, 400, 500}
T4:{1100, 1200, 1300, 1400, 1500}
{100, 200, 300, 400, 500}

T :{100, 200, 300, 400, 500}
T2:{100, 200, 250, 300, 400, 500}
T3:{100, 150, 200, 250, 300, 400, 500}
T4:{200, 300, 400, 500}

```

