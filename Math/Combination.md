### 一、模板类别

​	数学：快速幂，阶乘，逆元，组合数，多重组合

### 二、模板功能

<<<<<<< HEAD
#### 1.构造时打表

1. 数据类型

   模板参数 $int\space range$ ，表示打表的范围大小为 $[0,range]$。 默认 $1000000$ 。
=======
#### 1.打表

1. 数据类型

   模板参数 $int\space range$ ，表示打表的范围大小。默认 $1000000$ 。
>>>>>>> remotes/origin/main

   模板参数 $int\space P$ ，表示模几取余。默认 $1000000007$ 。

   构造无参数。

2. 时间复杂度

   阶乘打表 $O(range)$ 。

   逆元打表 $O(range)$ 。

   阶乘逆元打表 $O(range)$ 。

#### 2.计算快速幂

1. 数据类型：

   参数 $int\space a$ ，表示幂的底数。

   参数 $int\space n$ ，表示幂的指数。

2. 时间复杂度

   $O(\log n)$ 。

#### 3.计算阶乘

1. 数据类型

   参数  $int\space a$ ，表示是 $a$ 的阶乘。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   一般不考虑太大范围的阶乘，所以只要 $range$ 开得足够大，就可以直接查表。

#### 4.计算逆元

 1. 数据类型

    参数  $int\space a$ ，表示要求 $a$ 的逆元。

 2. 时间复杂度

    $O(1)$ ，如果 $a\le range$ 。

    $O(\log P)$ ，如果 $a\gt range$ 。

#### 5.计算阶乘逆元

1. 数据类型

   参数  $int\space a$ ，表示是 $a$ 的阶乘的逆元。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   一般不考虑太大范围的阶乘，所以只要 $range$ 开得足够大，就可以直接查表。

#### 6.计算组合数

1. 数据类型

   参数 $int\space m$ ，表示从 $m$ 个元素里进行选择。

   参数 $int\space n$ ，表示要选择 $n$ 个元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   当 $n\lt 0$ 或者 $n\gt m$ 时，返回 $0$ 。

#### 7.计算多重组合数

1. 数据类型

   输入参数 $int\space a$ ，表示加入某种重复元素 $a$ 个。

   输出参数 $T\&a$ ，根据之前加入的各种重复元素，计算得到多重组合数，将结果乘到 $a$ 上。

2. 时间复杂度

   输入时间复杂度 $O(1)$ 。

   输出时间复杂度 $O(1)$ 。

3. 备注

   本函数的使用方法为，$<<$ 运算符将输入数据送进去， $>>$ 运算符将输出数据送出去。

   注意，输出时，并非覆盖原数，而是乘到原数上。
   
   特别注意，本函数默认使用者有较好的代码习惯，若干组输入对应一组输出，在输出时，将会清空本轮所有的输入。如果在没有输出的情况下进行下一轮计算，则上一轮计算的数据会对下一轮造成影响。

### 三、模板示例

```c++
#include<bits/stdc++.h>
using namespace std;

template<int range=1000000,int P=1000000007>class CombinationHelper;

int main(){
    // range 和 P 都可以使用默认值或者自定义
    CombinationHelper<100000,1000000007>ch;

    // 3 的 10000 次方，对 1000000007 取余结果为 895629451
    long long pow=ch.pow(3,10000);
    cout<<pow<<endl;

    // 20 的阶乘，对 1000000007 取余结果为 146326063
    long long factorial=ch.f(20);
    cout<<factorial<<endl;

    // 10000 的逆元为 285700002 。可以验算，二者乘积模P余1
    long long inverse=ch.inv(10000);
    cout<<inverse<<endl;

    // 20 的阶乘逆元为 120104394 。可以验算，和 20 的阶乘的乘积模P余1
    long long factorial_inverse=ch.finv(20);
    cout<<factorial_inverse<<endl;

    // 从 20 个座位里挑选 5 个座位的方案数为 15504
    long long combination=ch.comb(20,5);
    cout<<combination<<endl;

    // 从 20 个座位里，挑选 5 个座位给大一，6 个座位给大二，7 个座位给大三，2 个座位给大四的方案数对 1000000007 取余结果为 793510706
    long long multicombination=1;
    ch<<5;
    ch<<6;
    ch<<7;
    ch<<2;
    ch>>multicombination;
    cout<<multicombination<<endl;

    system("pause");
}
```

