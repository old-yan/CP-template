### 一、模板类别

​	数据结构：二维【单点加，矩形区域和维护】表

​	练习题目：

1. [#133. 二维树状数组 1：单点修改，区间查询](https://loj.ac/p/133)


### 二、模板功能

#### 1.建立查询表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename SizeType` ，表示点坐标的类型。
   
   模板参数 `typename WeightType` ，表示点的权值的类型，默认为 `bool` ，表示每个点权值均为 `1` 。
   
   模板参数 `typename SumType` ，表示点权和的类型。
   
   模板参数 `size_type BATCH` ，表示一个阈值，低于此阈值的零散点会进行暴力处理；高于此阈值的零散点会加入表中。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本模板用于在线任务，可以边加入点，边进行在线子矩形内点权和查询。
   
   当 `WeightType` 为 `bool` 时，加入的点的点权固定为 `1` ， `weight_type` 为 `uint32_t` ；否则 `weight_type` 等于 `WeightType` 。


#### 2.加入点(add_point)

1. 数据类型

   输入参数 `SizeType x` ，表示点的横坐标。
   
   输入参数 `SizeType y` ，表示点的纵坐标。
   
   输入参数 `WeightType w` ，表示点权。

2. 时间复杂度

   均摊 $O(\log^2 n)$ 。
    
3. 备注

   当 `WeightType` 为 `bool` 时无需传递点权。

#### 3.查询(query)

1. 数据类型

   输入参数 `SizeType x_min` ，表示子矩形的最小横坐标。
   
   输入参数 `SizeType x_max` ，表示子矩形的最大横坐标。
   
   输入参数 `SizeType y_min` ，表示子矩形的最小纵坐标。
   
   输入参数 `SizeType y_max` ，表示子矩形的最大纵坐标。

2. 时间复杂度

    $O(\log^2 n)$ 。

#### 三、模板示例

```c++
#include "DS/PointAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

void test_add() {
    OY::PARSM2D::Table<int, int, int> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;

    S.add_point(1, 1, 100);

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

int main() {
    test_add();
}
```

```
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 104
sum of S{0<=x<=1, -1<=y<=1}: 102

```

