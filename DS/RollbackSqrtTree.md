### 一、模板类别

​	数据结构：回滚 `SqrtTree` 。

​	练习题目：

1. [3117. 划分数组得到最小的值之和](https://leetcode.cn/problems/minimum-sum-of-values-by-dividing-array/)
2. [P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
3. [P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)



### 二、模板功能

​		本模板与普通 `SqrtTree` 的功能基本一致，但是额外添加了动态扩容的功能，可以在尾部进行高效的 `push_back` 和 `pop_back` 操作。以下仅介绍与普通版不同的操作。

#### 1.预留空间(reserve)

1. 数据类型

   输入参数 `size_type capacity`​ ，表示想预留的空间。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   回滚根树的底层用到 `vector` 以实现动态扩容；如果能够预先知道 `SqrtTree` 维护的区间大小，可以预留空间，减小常数。
   
   即便不调用本函数，也不会影响程序正确性。


#### 2.查询维护的区间大小(size)

1. 数据类型

   返回类型 `size_type` ，表示目前维护的区间大小。

2. 时间复杂度

   $O(1)$ 。
   

#### 3.查询是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示目前维护的区间是否为空。

2. 时间复杂度

    $O(1)$  。

#### 4.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `const value_type &val` ，表示添加的元素值。

2. 时间复杂度

    均摊 $O(1)$  。

#### 5.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

    均摊 $O(1)$ 。


### 三、模板示例

```c++
#include "DS/RollbackSqrtTree.h"
#include "IO/FastIO.h"

void test() {
    // 仅展示与普通根树不同的功能
    using Tree = OY::RollbackSqrtMaxTable<int>;
    Tree S;
    S.reserve(10);
    for (int i = 0; i < 10; i++) S.push_back(i * i);
    cout << S << endl;
    for (int i = 0; i < 5; i++) S.pop_back();
    for (int i = 5; i < 10; i++) S.push_back(i * i + 100);
    cout << S << endl;
}

int main() {
    test();
}
```

```
#输出如下
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
[0, 1, 4, 9, 16, 125, 136, 149, 164, 181]

```

