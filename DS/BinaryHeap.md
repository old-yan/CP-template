### 一、模板类别

​	数据结构：二叉堆

### 二、模板功能

#### 1.建堆

1. 数据类型

   模板参数 `typename _Tp` ，表示堆中的元素类型。

   模板参数 `typename _Compare` ，表示比较函数的类，默认为 `_Tp` 类的小于号函数。

   构造参数 `_Compare __comp` ，表示具体的排序函数，默认为`_Compare` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   堆处理的问题为：如何在变化的数据流里维护最值元素。广义的堆可以泛指二叉堆、配对堆、左偏树、二项堆、斐波那契堆等等；而狭义的堆特指二叉堆。二叉堆也是最常用到的堆，一般堆的问题都可以用二叉堆来解决。

#### 2.建堆

1. 数据类型

   构造参数 `_Iterator __first` ，表示区间头。

   构造参数 `_Iterator __last` ，表示区间尾。（开区间）

   其它同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间中的元素直接插入到堆里。这比一个一个地插入效率要高。

#### 3.清空

1. 数据类型

2. 时间复杂度

   $O(n)$ 。


#### 4.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间头。

   输入参数 `_Iterator __last` ，表示区间尾。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   使用迭代器进行重置，可以将区间中的元素直接插入到堆里。这比一个一个地插入效率要高。

#### 5.插入元素

1. 数据类型

   输入参数 `_Tp __val` ，表示插入的元素值。

2. 时间复杂度

   $O(\log n)$ 。

#### 6.弹出最值

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 7.查询最值

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 8.合并堆

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   二叉堆的合并采用启发式合并的方法，保证了对数量级的均摊复杂度。

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
#include "DS/BinaryHeap.h"
#include "IO/FastIO.h"

int main(){
    //默认均为元素类型为 int 的大根堆
    OY::BinaryHeap big_root;
    for(int a:{1,9,3,7,5}){
        big_root.push(a);
    }
    while(big_root.size()){
        cout<<big_root.top()<<endl;
        big_root.pop();
    }

    //传递比较参数，修改为字符串的小根堆
    OY::BinaryHeap small_root(std::greater<std::string>{});
    for(std::string s:{"apple","erase","cat","dog","banana"}){
        small_root.push(s);
    }
    while(small_root.size()){
        cout<<small_root.top()<<endl;
        small_root.pop();
    }

    //相比普通的堆，新增了 join 功能，用于将两个堆合并到一个堆
    OY::BinaryHeap heap1;
    OY::BinaryHeap heap2;
    for(int a:{1,3,5,7,9})heap1.push(a);
    for(int a:{2,4,6,8,10})heap2.push(a);
    heap1.join(heap2);
    while(heap1.size()){
        cout<<heap1.top()<<' ';
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

