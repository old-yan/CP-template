### 一、模板类别

​	数据结构：原地升降二叉堆。

### 二、模板功能

​	本堆核心为二叉堆，在此基础上做了一些优化。

#### 1.建堆

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示点的编号、位置编号的类型。

   模板参数 `typename Mapping` ，表示堆中的不同编号进行比较时，赖以体现自己价值大小的映射函数。理解时可以参考 `python` 排序时的 `key` 参数。

   模板参数 `typename Compare` ，表示比较函数的类，默认为小于号函数。

   构造参数 `size_type length` ，表示堆中的元素编号范围。

   构造参数 `Mapping map` ，表示具体的映射函数，默认为`Mapping` 类的默认实例。

   构造参数 `Compare comp` ，表示具体的排序函数，默认为`Compare` 类的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   常规的二叉堆可以动态维护若干数据里的最小值。然而，二叉堆有个缺点：当你想维护固定若干个数据的最小值时，你可能需要修改某个数据的值。然而，你并不能精准地找到这个值在堆中的位置，然后根据它的值的增大或者减小，将它在堆中动态“上浮”或者“下沉”。
   
   经典案例就是在 `Dijkstra` 最短路算法借助 `std::priority_queue` 的常规实现中，当你将到某个点的距离缩短时，你并未将该点在堆中的旧版本“上浮”，而是在堆中加入一个该点的新版本表示。所以在同一时刻，堆中可能有同一元素的不同版本，除了最新版本外，其它旧版本都是没有作用的。当有元素出栈时，还要判断这个元素是旧版本还是新版本，如果是旧版本就置之不理。这里实际上相当于“惰性删除”。
   
   本数据结构解决了这一问题：在对某一点的值进行修改时，会在堆中该元素的旧版本基础上进行修改。（对于默认的大顶堆来说），如果将该点的值增大，可以调用“上浮”方法，在原来的位置基础上上浮；如果将该点的值减小，可以调用“下沉”方法，在原来的位置基础上下沉。堆中，任何时刻，对于同一个点只存在一个版本的表示。
   
   本数据结构可以将 `Dijkstra` 算法的时间复杂度从 $O(m\cdot \log m)$ 优化到 $O(m\log n)$ ，经实测，可以提速 `20%` 左右。其在`Johnson` 全源最短路算法、`Prim_heap` 最小生成树算法、`Steiner` 算法、`MPM` 最大流算法、 `PP_heap`最大流算法中也有用武之地。

#### 2.元素上浮(sift_up)

1. 数据类型

   输入参数 `size_type i` ，表示要上浮的元素的编号。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   当某编号的元素值增大（以大根堆为例）时，调用此函数以更新其位置。

   当某编号的元素刚刚进入堆时，会自动调用此函数以更新其位置。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 3.元素下沉(sift_down)

1. 数据类型

   输入参数 `size_type i` ，表示要下沉的元素的编号。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   当某编号的元素的值减小（以大根堆为例）时，调用此函数以更新其位置。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 4.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(n)$ 。

#### 5.插入元素(push)

1. 数据类型

   输入参数 `size_type __i` ，表示要插入的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果该元素已在堆中，则尝试将其上浮；如果不在堆中，则先插入堆中，再上浮。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.查询最值(top)

1. 数据类型

   返回类型 `size_type` ，表示目前最大（以大顶堆为例）的元素的编号

2. 时间复杂度

   $O(1)$ 。

#### 7.弹出最值(pop)

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 8.查询堆大小(size)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 9.查询堆是否为空(empty)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include "DS/SiftHeap.h"
#include "IO/FastIO.h"

int main() {
    // 如果编号对应的价值存放在 vector 里
    std::vector<long long> values{10, 20, 30, 50, 40};
    // 这是一个 [0, 4] 编号范围内、以 values 为价值判断依据的大根堆
    auto value_heap = OY::make_SiftHeap(5, values, std::less<long long>{});
    for (int i = 0; i < 5; i++) value_heap.push(i);
    // 现在价值最大的是 3 号
    cout << "top is: " << value_heap.top() << endl;
    // 我们减小一下 3 号的价值，然后令其下沉
    values[3] = 5;
    value_heap.sift_down(3);
    // 现在价值最大的是 4 号
    cout << "top is: " << value_heap.top() << endl;
    // 我们增大一下 2 号的价值，然后令其上浮
    values[2] = 60;
    value_heap.sift_up(2);
    // 现在价值最大的是 2 号
    cout << "top is: " << value_heap.top() << endl;

    // 经过了这么多操作，堆的大小还是 5
    cout << "size = " << value_heap.size() << endl;

    // 如果编号对应的价值存放在定长数组里，仍然可以以类似方式构造
    std::string values2[5] = {"banana", "apple", "cat", "egg", "dog"};
    // 这是一个 [0, 4] 编号范围内、以 values2 为价值判断依据的小根堆
    auto value_heap2 = OY::make_SiftHeap(5, values2, std::greater<std::string>{});
    for (int i = 0; i < 5; i++) value_heap2.push(i);
    // 现在字典序最小的是 1 号
    cout << "top is: " << value_heap2.top() << endl;

    // 当然，如果某个编号的价值很难用一个值来衡量，你可以用匿名函数来自定义它的价值
    // 比如，某个编号的价值取决于数学分数和语文分数之和，那么可以这么写
    long long maths[] = {95, 62, 38, 71, 98};
    long long chinese[] = {30, 56, 80, 66, 75};
    // 这表示，编号的价值就在于自己编号本身；而编号与编号比较大小时，才去查看两个数组的值。
    // 这种写法的比较任务，实际落在了 comp 身上
    auto self = [](int i) { return i; };
    auto comp = [&](int x, int y) { return maths[x] + chinese[x] < maths[y] + chinese[y]; };
    OY::SiftHeap<decltype(self), decltype(comp)> students(5, self, comp);
    // 也可以这么写：
    // 这表示，编号的价值就在于自己编号本身；而编号与编号比较大小时，才去查看两个数组的值。
    // 这种写法的比较任务，实际落在了 getsum 身上
    auto getsum = [&](int i) { return maths[i] + chinese[i]; };
    OY::SiftHeap<decltype(getsum), std::less<long long>> students2(5, getsum);
    // 不管那种写法，本质都一样，最后都能得出 4 号学生总分最高
    for (int i = 0; i < 5; i++) {
        students.push(i);
        students2.push(i);
    }
    cout << students.top() << " == " << students2.top() << endl;
}
```

```
#输出如下
top is: 3
top is: 4
top is: 2
size = 5
top is: 1
4 == 4

```

