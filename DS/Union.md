### 一、模板类别

​	数据结构：并查集

### 二、模板功能

#### 1.建立并查集

1. 数据类型

   构造参数 $int\space n$ ，表示并查集要处理的元素个数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本数据结构只接收离散化之后的数字，如果想对字符串、二维向量进行并查集操作，需要手动将其离散化，再使用本数据结构。


#### 2.reset

1. 数据类型

   输入参数 $int\space n$ ，表示并查集要处理的元素个数。

2. 时间复杂度

   $O(n)$ 。

#### 3.Find

1. 数据类型：

   参数 $int\space i$ ，表示要寻找元素 $i$ 所在组分的首领。

2. 时间复杂度

   $O(\alpha (n))$ 。


#### 4.unite

1. 数据类型：

   输入参数 $int\space i$  和 $int\space j$ ，表示元素 $i$ 和元素 $j$ 所在的组分进行合并。

   返回类型 $bool$ ，如果成功将两个组分合并返回 $true$ ，如果 $i$ 和 $j$ 来自同一组分，返回 $false$ 。

2. 时间复杂度

   $O(\alpha(n))$ 。

#### 5.same

1. 数据类型

   输入参数 $int\space i$  和 $int\space j$ ，表示查询元素 $i$ 和元素 $j$ 是否位于同一个组分。

   返回类型 $bool$ 。

2. 时间复杂度

   $O(\alpha (n))$ 。

### 三、模板示例

```c++
#include<bits/stdc++.h>
using namespace std;

#define UNIONSIZE 100000
struct Union;

int main(){
    // 建立大小为 10 的并查集
    Union u(10);

    //查询 5 和 6 的关系，为 不在一起
    cout<<boolalpha<<u.same(5,6)<<endl;
    //将 2 和 5 进行合并
    u.unite(2,5);
    //将 3 和 6 进行合并
    u.unite(3,6);
    //将 5 和 6 进行合并
    u.unite(5,6);
    //查询 6 所在的组分, 为 2
    cout<<u.Find(6)<<endl;
    //查询 5 和 6 的关系，为 在一起
    cout<<boolalpha<<u.same(5,6)<<endl;
}
```

