### 一、模板类别

​	数据结构：线段树

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   模板参数 $class\space T$ ，表示元素类型。

   模板参数 $class\space Accumulate$ ，表示累积函数的类。

   构造参数 $int\space n$ ，表示线段树的覆盖范围为 $[0,n)$。

   构造参数 $Accumulate\space op$ ，表示具体的累积函数，缺省值为 $Accumulate$ 的默认实例。

   构造参数 $T\space default\_val$ ，表示元素默认值，缺省值为 $T$ 的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   线段树处理的问题为单点修改和区间查询。只要累计函数的运算符满足**结合律**，就可以应用线段树。
   
   **结合律**：$a\oplus b\oplus c=a\oplus (b\oplus c) $ 。

#### 2.建立线段树

1. 数据类型

   构造参数 $iterator\space begin$ ，表示区间维护的区间头。

   构造参数 $iterator\space end$ ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到线段树里。


#### 3.重置

1. 数据类型

   输入参数 $int\space n$ ，表示将线段树的覆盖范围重置为 $[0,n)$。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   重置可以改变线段树的覆盖范围，并重置这个范围的值。但是不会改变累积函数，也不会改变 $default\_val$ ，如有需求可以建另外一棵树。

#### 4.重置

1. 数据类型

   输入参数 $iterator\space begin$ ，表示区间维护的区间头。

   输入参数 $iterator\space end$ ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到线段树里。

#### 5.单点赋值

1. 数据类型

   输入参数 $int\space i$ ，表示单点修改的下标。

   输入参数 $T\space \_val$ ，表示修改后的值。

2. 时间复杂度

   $O(\log n)$ 。

#### 6.单点增值

1. 数据类型

   输入参数 $int\space i$ ，表示单点增值的下标。

   输入参数 $T\space inc$ ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   单点增值和单点赋值的不同，可以参见如下例子：

   假定累积函数为求和函数 $[\space](auto\space x,auto\space y)\{return\space x\space+\space y;\}$ 。

   某点原值为 $10$ ，单点赋值 $3$ ，赋值后值变为 $3$ 。

   某点原值为 $10$ ，单点增值 $3$ ，增值后值变为 $13$ 。

#### 7.单点查询

1. 数据类型

   输入参数 $int\space i$ ，表示单点查询的下标。

2. 时间复杂度

   $O(\log n)$ 。

#### 8.区间查询

1. 数据类型

   输入参数 $int\space l$ ，表示区间查询的开头下标。

   输入参数 $int\space r$，表示区间查询的结尾下标。（闭区间）

2. 时间复杂度

   $O(\log n)$ 。

#### 9.查询全部

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.查询第 $n$ 个元素

1. 数据类型

   输入参数 $T\space n$ ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   顺次 $n$ 表示第 $n+1$ 小，顺次 $0$ 表示查询最小的元素。

   只有在线段树的元素类型 $T$ 为数字，且累积函数为加法的时候，本函数才有意义。

### 三、模板示例

```c++
#include <bits/stdc++.h>
using namespace std;

#define SEGTREEDEPTH 10
template<class T,class Accumulate=T(*)(T,T)>class SegTree;

int main(){
    //建立一个区间最大值线段树
    auto max=[](int x,int y){
        return x>y?x:y;
    };
    //默认值设为 -100
    SegTree<int,decltype(max)>tree_max(10,max,-100);

    //区间 [3,6] 的最大值为 -100
    cout<<tree_max(3,6)<<endl;
    //进行单点修改
    tree_max.set(3,10);
    tree_max.set(4,12);
    tree_max.set(5,8);
    tree_max.set(6,10);
    //区间 [3,6] 的最大值为 12
    cout<<tree_max(3,6)<<endl;

    //下标 [7] 的值为 -100
    cout<<tree_max[7]<<endl;
    tree_max.set(7,100);
    //下标 [7] 的值为 100
    cout<<tree_max[7]<<endl;

    //给下标 [7] 添加增量 90 ,试图修改其最大值
    tree_max.step(7,90);
    //下标 [7] 的值仍为 100
    cout<<tree_max[7]<<endl;

    //给下标 [7] 添加增量 110 ,试图修改其最大值
    tree_max.step(7,110);
    //下标 [7] 的值为 110
    cout<<tree_max[7]<<endl;

    //现在针对一个已存在的区间，建立线段树
    int A[10]={11,12,13,14,15,16,17,18,19,20};
    //建立一个区间求和线段树
    auto sum=[](int x,int y){
        return x+y;
    };
    //其实在大部分情况下，模板参数的累积函数类不需要填写
    SegTree<int>tree_sum(A,A+10,sum);

    //区间 [3,6] 的和为 62
    cout<<tree_sum(3,6)<<endl;
    //进行单点修改
    tree_sum.set(3,20);
    //区间 [3,6] 的和为 68
    cout<<tree_sum(3,6)<<endl;

    //查询全部，全部和为 161
    cout<<tree_sum.all()<<endl;

    //查询大小排名为 23 的元素，即：假定有11个0，12个1，13个2...从小到大排 23 的是谁
    // 0 占据排名 [0,10], 1 占据排名 [11,22]，所以排 23 的是元素 2
    cout<<tree_sum.find_nth(23)<<endl;
}
```

