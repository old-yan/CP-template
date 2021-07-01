### 一、模板类别

​	数据结构：猫树

### 二、模板功能

#### 1.建立猫树

1. 数据类型

   模板参数 $class\space T$ ，表示要进行区间最值查询的元素类型。

   模板参数 $class\space Max$ ，表示最值函数的类。

   构造参数 $iterator\space begin$ ，表示区间最值查询的区间头。

   构造参数 $iterator\space end$ ，表示区间最值查询的区间尾。（开区间）

   构造参数 $Max\space max$ ，表示具体的最值函数，缺省值为 $Max$ 的默认实例。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   猫树处理的问题为区间查询，要求查询函数的运算符满足**交换律**和**结合律**，就可以应用猫树。

   **交换律**：$a\oplus b=b\oplus a $ 。

   **结合律**：$a\oplus b\oplus c=a\oplus (b\oplus c) $ 。


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

#define CATTREEDEPTH 17
template<class T,class Max=T(*)(T,T)>struct CatTree;

int main(){
    //猫树可以实现 ST 表的任何功能，而且效率相当
    //此外，猫树可以实现 ST 表不能做到的功能，比如求和查询

    //先给出一个长度为 10 的数组
    int A[10]={11,5,9,12,8,4,6,15,7,7};

    //建立一个区间求和猫树
    auto add=[](int x,int y){
        return x+y;
    };
    //一般可以忽略第二个模板参数
    CatTree<int>ct_sum(A,A+10,add);
    //区间 [3,6] 的和为 30 
    cout<<ct_sum(3,6)<<endl;
}
```

