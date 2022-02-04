### 一、模板类别

​	数据结构： `pb_ds` 库中的几种可并堆。

### 二、模板功能

#### 1.建堆

1. 数据类型

   模板参数 `typename _Tp` ，表示堆中的元素类型。

   模板参数 `typename _Compare` ，表示比较函数的类，默认为 `_Tp` 类的小于号函数。

   构造参数 `_Compare __comp` ，表示具体的排序函数，默认为`_Compare` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   堆处理的问题为：如何在变化的数据流里维护最值元素。
   
   `pb_ds` 库支持二叉堆、配对堆、二项堆、冗余计数二项堆、`thin`堆等。其中，二叉堆和 `thin` 堆的合并操作非常慢；二项堆、冗余计数二项堆、`thin` 堆的常数很大，一般不推荐使用。

#### 2.清空

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

#### 3.插入元素

1. 数据类型

   输入参数 `_Tp __val` ，表示插入的元素值。

2. 时间复杂度

   $O(1)\sim O(\log n)$ 。
   
3. 备注

   不同的堆有着不同的时间复杂度。

#### 6.弹出最小值

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 7.查询最小值

1. 数据类型

2. 时间复杂度

   $O(1\sim\log n)$ 。

#### 8.合并堆

1. 数据类型

2. 时间复杂度

   $O(1\sim n)$ 。
   
3. 备注

   不同的堆有着不同的时间复杂度；尽管有些堆（比如二叉堆）的合并方法非常非常慢，但是并非不支持合并。


#### 9.查询堆大小

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.查询堆是否为空

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include "DS/PB_DS_Heap.h"
#include "IO/FastIO.h"

int main() {
    //默认为大根堆
    OY::PB_DS_BinaryHeap<int> big_root;
    for (int a : {1, 9, 3, 7, 5}) {
        big_root.push(a);
    }
    while (big_root.size()) {
        cout << big_root.top() << endl;
        big_root.pop();
    }

    //传递比较参数，修改为小根堆
    OY::PB_DS_PairingHeap<std::string, std::greater<std::string>> small_root;
    for (std::string s : {"apple", "erase", "cat", "dog", "banana"}) {
        small_root.push(s);
    }
    while (small_root.size()) {
        cout << small_root.top() << endl;
        small_root.pop();
    }

    //相比普通的堆，新增了 join 功能，用于将两个堆合并到一个堆
    OY::PB_DS_BinomialHeap<int> heap1;
    OY::PB_DS_BinomialHeap<int> heap2;
    for (int a : {1, 3, 5, 7, 9}) heap1.push(a);
    for (int a : {2, 4, 6, 8, 10}) heap2.push(a);
    heap1.join(heap2);
    while (heap1.size()) {
        cout << heap1.top() << ' ';
        heap1.pop();
    }
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

```

