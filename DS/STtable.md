### 一、模板类别

​	数据结构：ST表

### 二、模板功能

#### 1.建立ST表

1. 数据类型

   模板参数 $class\space T$ ，表示要进行区间最值查询的元素类型。

   模板参数 $class\space Max$ ，表示最值函数的类。

   构造参数 $iterator\space begin$ ，表示区间最值查询的区间头。

   构造参数 $iterator\space end$ ，表示区间最值查询的区间尾。（开区间）

   构造参数 $Max\space max$ ，表示具体的最值函数，缺省值为 $Max$ 的默认实例。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   $ST$ 表处理的问题为区间最值查询，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数。本质上，按位或就是二进制的每位最大值，按位与就是二进制的每位最小值。只要满足**可重复贡献**的性质，就可以使用 $ST$ 表。
   
   **可重复贡献**：比如想查询区间 $[1,13]$ 的最大值，可以先查询 $[1,8]$ 的最大值，再查询 $[6,13]$ 的最大值，再将二者合并。尽管两次查询的区间有重复部分，但是不影响结果。


#### 2.查询

1. 数据类型

   输入参数 $int\space l$ ，表示要查询的区间的左边界。

   输入参数 $int\space r$ ，表示要查询的区间的右边界。（闭区间）

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include <bits/stdc++.h>
using namespace std;

#define STDEPTH 17
template<class T,class Max>struct STtable;

int main(){
    //先给出一个长度为 10 的数组
    int A[10]={11,5,9,12,8,4,6,15,7,7};

    //建立一个区间最大值 ST 表
    auto max=[](int x,int y){
        return x>y?x:y;
    };
<<<<<<< HEAD
    //一般可以忽略第二个模板参数
    STtable<int>st_max(A,A+10,max);
=======
    STtable<int,decltype(max)>st_max(A,A+10,max);
>>>>>>> remotes/origin/main
    //区间 [3,6] 的最大值为 12 
    cout<<st_max(3,6)<<endl;

    //建立一个区间最小值 ST 表
    auto min=[](int x,int y){
        return x<y?x:y;
    };
<<<<<<< HEAD
    STtable<int>st_min(A,A+10,min);
=======
    STtable<int,decltype(min)>st_min(A,A+10,min);
>>>>>>> remotes/origin/main
    //区间 [3,6] 的最小值为 4 
    cout<<st_min(3,6)<<endl;

    //建立一个区间最大公约数 ST 表
    auto gcd=[](int x,int y){
        return std::__gcd(x,y);
    };
<<<<<<< HEAD
    STtable<int>st_gcd(A,A+10,gcd);
=======
    STtable<int,decltype(gcd)>st_gcd(A,A+10,gcd);
>>>>>>> remotes/origin/main
    //区间 [3,6] 的最大公约数为 2
    cout<<st_gcd(3,6)<<endl;

    //建立一个区间按位与 ST 表
<<<<<<< HEAD
    //按位与的函数类具有默认构造，可以忽略构造参数
=======
>>>>>>> remotes/origin/main
    STtable<int,bit_and<int>>st_bit_and(A,A+10);
    //区间 [3,6] 的按位与为 0
    cout<<st_bit_and(3,6)<<endl;

    //建立一个区间按位或 ST 表
    STtable<int,bit_or<int>>st_bit_or(A,A+10);
    //区间 [3,6] 的按位或为 14
    cout<<st_bit_or(3,6)<<endl;
<<<<<<< HEAD
=======

    system("pause");
>>>>>>> remotes/origin/main
}
```

