### 一、模板类别

​	数据结构：二维【单点加，矩形区域和查询】表

练习题目：

1. [P10814 【模板】离线二维数点](https://www.luogu.com.cn/problem/P10814)

### 二、模板功能

#### 1.建立查询表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename SizeType` ，表示点坐标的类型。
   
   模板参数 `typename WeightType` ，表示点的权值的类型，默认为 `bool` ，表示每个点权值均为 `1` 。
   
   模板参数 `typename SumType` ，表示点权和的类型。
   
   模板参数 `bool HasModify` ，表示是否支持对已经存在的点进行单点修改。

   构造参数 `size_type point_cnt` ，表示点数。默认为 `0` 。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板用于一次性加入很多点，然后再进行在线子矩形内点权和查询。
   
   当 `WeightType` 为 `bool` 时，加入的点的点权固定为 `1` ， `weight_type` 为 `uint32_t` ；否则 `weight_type` 等于 `WeightType` 。
   
   当 `HasModify` 为 `true` 时可以进行单点修改，但是仅限于对已加入的点进行单点修改。
   
   构造参数 `point_cnt` 仅用来为后续加入的点预留空间；即使 `point_cnt` 不传或者传错，也不会影响使用。

#### 2.加入点(add_point)

1. 数据类型

   输入参数 `SizeType x` ，表示点的横坐标。
   
   输入参数 `SizeType y` ，表示点的纵坐标。
   
   输入参数 `weight_type w` ，表示点权。

2. 时间复杂度

    $O(1)$ 。
    
3. 备注

   当 `WeightType` 为 `bool` 时无需传递点权。

#### 3.预备(prepare)

1. 数据类型

2. 时间复杂度

    $O(n\log n)$ 。

#### 4.查询(query)

1. 数据类型

   输入参数 `SizeType x_min` ，表示子矩形的最小横坐标。
   
   输入参数 `SizeType x_max` ，表示子矩形的最大横坐标。
   
   输入参数 `SizeType y_min` ，表示子矩形的最小纵坐标。
   
   输入参数 `SizeType y_max` ，表示子矩形的最大纵坐标。

2. 时间复杂度

   若 `HasModify` 为 `true` ，为 $O(\log^2 n)$ ；否则为 $O(\log n)$ 。

3. 备注

   必须先调用 `prepare` ，再进行查询。

#### 5.增加点的权值(add_point_value)

1. 数据类型

   输入参数 `size_type point_id` ，表示要修改的点的编号。
   
   输入参数 `SumType w` ，表示要增加的点权。

2. 时间复杂度

    $O(\log^2 n)$ 。
    
3. 备注

   本方法仅当 `HasModify` 为 `true` 时可以使用。

#### 三、模板示例

```c++
#include "DS/PointAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::PARSC2D::Table<int, bool> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);
    S.prepare();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

void test_add() {
    OY::PARSC2D::Table<int, uint32_t, uint32_t, true> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);
    S.prepare();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;

    // 编号为 3 的点的权值增加一百
    S.add_point_value(3, 100);
    
    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << S.query(0, 1, -1, 1) << endl;
}

int main() {
    test();
    test_add();
}
```

```
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2
sum of S{-1<=x<=1, -1<=y<=1}: 104
sum of S{0<=x<=1, -1<=y<=1}: 102

```

