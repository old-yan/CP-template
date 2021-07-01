### 一、模板类别

​	数据结构：动态开点的带懒惰标记的线段树

### 二、模板功能

​	动态开点的带懒惰标记的线段树的接口和功能，完全等同于带懒惰标记的线段树。唯一差别是，动态开点的带懒惰标记的线段树的数据范围可以高达 $10^9$ ，也不会爆空间。但是时间效率相应地会差一点点。

​	备注：

​	以 $int\space n$ 为入参的构造函数，还有以 $int\space n$ 为入参的重置函数，时间复杂度为 $O(1)$

​	目前以迭代器为入参的构造函数，还有以迭代器为入参的重置函数，时间复杂度为 $O(n\cdot \log n)$

### 三、模板示例

```c++
#include <bits/stdc++.h>
using namespace std;

//动态开点懒惰标记线段树
#define DLTREESIZE 4000000
template<class T,class F=T,uint8_t bsize=1,class Accumulate=T(*)(T,T),class Mapping=T(*)(T,F),class Composition=F(*)(F,F)>class DLTree;

int main(){
    //动态开点的带懒惰标记的线段树的大部份功能和带懒惰标记的线段树是一样的，相同部分不多做展示

    //建立一个区间求和线段树，且以求和函数来进行更新
    auto add=[](int x,int y){
        return x+y;
    };
    //可以看到，和懒惰标记线段树的初始化是一样的，但是可以管理长达十亿的区间
    DLTree<int>tree_sum(1000000000,add);

    //区间 [1，50000000] 的和为 0
    cout<<tree_sum(1,50000000)<<endl;

    //进行区间增值修改
    tree_sum.step(40000000,60000000,1);
    //区间 [1，50000000] 的和为 10000001
    cout<<tree_sum(1,50000000)<<endl;
}
```

