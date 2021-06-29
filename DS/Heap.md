### 一、模板类别

​	数据结构：二叉堆，配对堆，左偏树

### 二、模板功能

​	堆的实现方法有差别，但是在使用的时候完全不需要知道实现的细节，只需要知道对外的接口。

#### 0.和STL::priority_queue的差别

​	标准库的优先级队列，允许重复元素的存在，而我这里写的堆，允许提升或者降低堆中某个元素的优先级，从而让它在堆中上浮下沉，不需要把新的元素送到堆里。

#### 1.建立堆

1. 数据类型

   模板参数 $class\space T$ ，表示堆中的元素类型。

   模板参数 $class\space Cmp$ ，表示排序函数的类，默认为元素的小于号结构体。

   模板参数 $uint8\_t\space mask$ ，表示堆的一些参数。

   1. $mask=0$ 表示元素类型为 $int$ ，不需要离散化，这样效率最高；
   2. $mask=1$ 表示元素需要离散化，效率一般，且需要该元素类型支持哈希；
   3. $mask=2$ 表示允许重复，效率较高。

   构造参数 $Cmp\space comp$ ，表示具体的排序函数，默认为小于号，即较大的元素优先出队。

2. 时间复杂度

   $O(1)$ 。


#### 2.clear

1. 数据类型

2. 时间复杂度

   与堆的大小成正相关。

3. 备注

   $BiHeap<T,Cmp,2>$ 的 $clear$ 方法，时间复杂度恒为 $O(1)$ 。

   $PairHeap<T,Cmp,2>$ 的 $clear$ 方法，时间复杂度恒为 $O(1)$ 。

   $LeftistTree<T,Cmp,2>$ 的 $clear$ 方法，时间复杂度恒为 $O(1)$ 。

#### 3.push

1. 数据类型：

   参数 $T\space val$ ，表示要推入的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   若 $mask=0$ 或者 $mask=1$ ，则推入元素时会检查堆里有没有该元素，如果有的话不会推入重复元素。

#### 4.emplace

1. 数据类型：

   参数 $\_Args\&\&... \_\_args$ ，表示要推入的元素的构造参数。

   因为可以通过这些参数直接在目的地调用构造函数，所以效率较高。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   若 $mask=0$ 或者 $mask=1$ ，则推入元素时会检查堆里有没有该元素，如果有的话不会推入重复元素。

#### 5.sink

1. 数据类型

   参数 $T \space val$ ，表示降低元素 $val$ 的优先级。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   元素优先级下降，一般是因为其排序依赖于外部变量，外部变量发生变化，所以优先级也发生变动。

   当前只有 $BiHeap$ 提供 $sink$ 函数。

#### 6.top

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 7.pop

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 8.size

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 9.empty

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include<bits/stdc++.h>
using namespace std;

//由于三种堆的接口是一样的，所以只展示二叉堆
template<class T,class Cmp=less<T>,uint8_t mask=0>class BiHeap;
template<class T,class Cmp>class BiHeap<T,Cmp,0>;//不需要离散化
template<class T,class Cmp>class BiHeap<T,Cmp,1>;//需要离散化
template<class T,class Cmp>class BiHeap<T,Cmp,2>;//不去重

int main(){
    // mask 默认为 0，不需要离散化的堆 H0
    BiHeap<int>H0;
    H0.push(100);
    H0.push(50);
    H0.push(150);
    H0.push(50);
    //输出顺序为 [150,100,50]
    while(H0.size()){
        cout<<H0.top()<<'\n';
        H0.pop();
    }

    // mask 为 1，需要离散化的堆 H1。排序规则自定义为大于号（小顶堆）
    BiHeap<string,greater<string>,1>H1;
    H1.push("banana");
    H1.push("apple");
    H1.push("cat");
    H1.push("apple");
    //输出顺序为 ["apple", "banana", "cat"]
    while(H1.size()){
        cout<<H1.top()<<'\n';
        H1.pop();
    }

    // mask 为 2，不去重的堆 H2。完全相当于stl::priority_queue
    BiHeap<string,greater<string>,2>H2;
    H2.push("banana");
    H2.push("apple");
    H2.push("cat");
    H2.push("apple");
    //输出顺序为 ["apple", "apple", "banana", "cat"]
    while(H2.size()){
        cout<<H2.top()<<'\n';
        H2.pop();
    }
}
```

