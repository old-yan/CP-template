### 一、模板类别

​	数据结构：带懒惰标记的线段树

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   模板参数 $class\space T$ ，表示原有元素的类型。

   模板参数 $class\space F$ ，表示增量元素的类型。

   模板参数 $uint8\_t\space bsize$ ，表示增量对结点的影响是否与结点大小有关，缺省值为 $1$ 。

   模板参数 $class\space Accumulate$ ，表示累积函数的类。

   模板参数 $class\space Mapping$ ，表示增值函数的类。

   模板参数 $class\space Composition$ ，表示囤积函数的类。

   构造参数 $int\space n$ ，表示线段树的覆盖范围为 $[0,n)$。

   构造参数 $Accumulate\space op$ ，表示具体的累积函数。

   构造参数 $Mapping\space map$ ，表示具体的增值函数。

   构造参数 $Composition\space com$ ，表示具体的囤积函数。

   构造参数 $T\space default\_val$ ，表示原有元素的默认值，缺省值为 $T$ 的默认实例。

   构造参数 $F\space default\_inc$ ，表示增值元素的默认值，缺省值为 $F$ 的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   带懒惰标记的线段树处理的问题为单点修改、区间修改和区间查询。要求和线段树一样，要求累计函数的运算符满足**结合律**，同时增值函数和屯积函数也要满足**结合律**。
   
   **结合律**：$t\oplus f1\oplus f2=t\oplus (f1\oplus f2) $ 。
   
   在理解带懒惰标记的线段树的各种参数之前，请先理解普通的线段树参数。
   
   $F$ 不一定是和 $T$ 不一样的类，完全可以是相同的类.
   
   $bsize$ 影响的是一个结点的元素值，在接受某个增量影响时，需不需要考虑该结点大小影响。如果某个结点表示长度为 $10$ 的区间，某增量使区间元素全部增大 $2$ ，而累计函数为求和函数，那么这个结点的元素值应当增长 $20$ ；而如果累计函数为取最大函数，那么这个结点的元素值只应当增长 $2$ 。
   
   $op$ 函数与线段树的累积函数作用相同。
   
   $map$ 函数默认入参 $(T,F)$ ，返回类型 $T$ ，表示原有值 $T$ 在被增量 $F$ 影响之后改成多少。
   
   $com$ 函数默认入参 $(F,F)$ ，返回类型 $F$ ，表示两个增量 $F1$ 和 $F2$ 回合之后变成多大的增量。

#### 2.建立线段树

1. 数据类型

   参数作用同上，只需要填写 $int\space n$ 和 $Accumulate\space op$ 。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用本简化版构造函数的前提是，$T$ 和 $F$ 是同一个类，而且增值函数和屯积函数和累积函数是一样的，且默认元素和默认增量可以使用 $T$ 和 $F$ 的默认值。

#### 3.建立线段树

1. 数据类型

   构造参数 $iterator\space begin$ ，表示区间维护的区间头。

   构造参数 $iterator\space end$ ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   使用迭代器进行初始化，可以将区间初状态直接赋到线段树里。

#### 4.建立线段树

1. 数据类型

   参数作用同上，只需要填写 $iterator\space begin$ 和 $iterator\space end$ 。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用本简化版构造函数的前提是，$T$ 和 $F$ 是同一个类，而且增值函数和屯积函数和累积函数是一样的，且默认元素和默认增量可以使用 $T$ 和 $F$ 的默认值。


#### 5.重置

1. 数据类型

   输入参数 $int\space n$ ，表示将线段树的覆盖范围重置为 $[0,n)$。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   重置可以改变线段树的覆盖范围，并重置这个范围的值。但是不会改变各种函数参数，也不会改变 $default\_val$ ，如有需求可以建另外一棵树。

#### 6.重置

1. 数据类型

   输入参数 $iterator\space begin$ ，表示区间维护的区间头。

   输入参数 $iterator\space end$ ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到线段树里。

#### 7.单点赋值

1. 数据类型

   输入参数 $int\space i$ ，表示单点修改的下标。

   输入参数 $T\space \_val$ ，表示修改后的值。

2. 时间复杂度

   $O(\log n)$ 。

#### 8.单点增值

1. 数据类型

   输入参数 $int\space i$ ，表示单点增值的下标。

   输入参数 $ F\space \_inc$ ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   单点增值和单点赋值的不同，可以参见线段树中的例子。


#### 9.区间增值

1. 数据类型

   输入参数 $int\space l$ ，表示区间增值的开头下标。

   输入参数 $int\space r$ ，表示区间增值的结尾下标。（闭区间）

   输入参数 $ F\space \_inc$ ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   不存在对应版本的区间赋值函数，但是可以通过改变 $map$ 函数的实现，来使区间增值起到区间赋值的作用。

#### 10.单点查询

1. 数据类型

   输入参数 $int\space i$ ，表示单点查询的下标。

2. 时间复杂度

   $O(\log n)$ 。

#### 11.区间查询

1. 数据类型

   输入参数 $int\space l$ ，表示区间查询的开头下标。

   输入参数 $int\space r$，表示区间查询的结尾下标。（闭区间）

2. 时间复杂度

   $O(\log n)$ 。

#### 12.查询全部

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 13.查询第 $n$ 个元素

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

#define LAZYTREEDEPTH 10
template<class T,class F=T,uint8_t bsize=1,class Accumulate=T(*)(T,T),class Mapping=T(*)(T,F),class Composition=F(*)(F,F)>class LazyTree;
int main(){
    //带懒惰标记的线段树的大部份功能和线段树是一样的，相同部分不多做展示

    //建立一个区间求和线段树，且以求和函数来进行更新
    auto add=[](int x,int y){
        return x+y;
    };
    //实际使用中不需要传递那么多参数，因为模板参数一般默认就可以，累积函数、增值函数、囤积函数大部分情况下相同
    LazyTree<int>tree_sum(10,add);

    //区间 [3,6] 的和为 0
    cout<<tree_sum(3,6)<<endl;

    //进行单点赋值修改
    tree_sum.set(4,10);
    //区间 [3,6] 的和为 10
    cout<<tree_sum(3,6)<<endl;
    
    //进行单点增值修改
    tree_sum.step(4,20);
    //区间 [3,6] 的和为 30
    cout<<tree_sum(3,6)<<endl;

    //进行区间增值修改
    tree_sum.step(3,6,1);
    //区间 [3,6] 的和为 34
    cout<<tree_sum(3,6)<<endl;

    int A[10]={11,12,13,14,15,16,17,18,19,20};
    //建立一个区间最大值线段树，且以求和函数来进行更新
    auto max=[](int x,int y){
        return x>y?x:y;
    };
    //这里 bsize 设为 0 表示结点大小对结点更新没有影响
    LazyTree<int,int,0>tree_sum2(A,A+10,max,add,add);

    //区间 [3,6] 的最大值为 17
    cout<<tree_sum2(3,6)<<endl;

    //进行区间增值修改
    tree_sum2.step(3,6,10);
    //区间 [3,6] 的最大值为 27
    cout<<tree_sum2(3,6)<<endl;

    //最后再写一个 T 和 F 不是同一个类的树
    struct node{
        int mul,add;
    };
    auto op=[](int x,int y){
        return x+y;
    };
    auto map=[](int x,node y){
        return x*y.mul+y.add;
    };
    auto com=[](node x,node y){
        return node{x.mul*y.mul,x.add*y.mul+y.add};
    };
    //这棵树的功能，就是对数列里的数字进行两种操作：乘个倍率/加个偏移
    //可以把这两种操作，抽象为 node 类，那么需要手写 map 表示 int 受 node 影响后的结果，手写 com 表示两个 node 合并后的结果
    //注意一定要写好 node 的默认值，“乘1加0” 表示不变，很适合做默认值
    LazyTree<int,node,0>tree_eval(10,op,map,com,0,node{1,0});
    //区间 [3,5] 的和为 0
    cout<<tree_eval(3,5)<<endl;
    //将区间 [3,4] 统一加 10
    tree_eval.step(3,4,node{1,10});
    //将区间 [4,5] 统一乘 2
    tree_eval.step(4,5,node{2,0});
    //现在 下标3为 10 ，下标4为 20 ，下标5为 0
    cout<<tree_eval[3]<<' '<<tree_eval[4]<<' '<<tree_eval[5]<<endl;
}
```

