### 一、模板类别

​	数据结构：二维【矩形区域加，单点查询】表

​	练习题目：

1. [#134. 二维树状数组 2：区间修改，单点查询](https://loj.ac/p/134)


### 二、模板功能

#### 1.建立查询表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename SizeType` ，表示坐标的类型。
   
   模板参数 `typename WeightType` ，表示权值的类型，默认为 `bool` ，表示每个点权值均为 `1` 。
   
   模板参数 `typename SumType` ，表示权值和的类型。
   
   模板参数 `bool HasModify` ，表示是否支持对已经存在的矩形进行单点修改。

   构造参数 `size_type rect_cnt` ，表示矩形数。默认为 `0` 。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板用于一次性加入很多矩形，然后再进行在线点权查询。
   
   当 `WeightType` 为 `bool` 时，加入的权固定为 `1` ， `weight_type` 为 `uint32_t` ；否则 `weight_type` 等于 `WeightType` 。
   
   当 `HasModify` 为 `true` 时可以进行单个矩形修改，但是仅限于对已加入的矩形进行单点修改。
   
   构造参数 `rect_cnt` 仅用来为后续加入的矩形预留空间；即使 `rect_cnt` 不传或者传错，也不会影响使用。

#### 2.加入矩形(add_rect)

1. 数据类型

   输入参数 `SizeType x_min` ，表示矩形的横坐标最小值。
   
   输入参数 `SizeType x_max` ，表示矩形的横坐标最大值。
   
   输入参数 `SizeType y_min` ，表示矩形的纵坐标最小值。
   
   输入参数 `SizeType y_max` ，表示矩形的纵坐标最大值。
   
   输入参数 `weight_type w` ，表示权。

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

   输入参数 `SizeType x` ，表示点的横坐标。
   
   输入参数 `SizeType y` ，表示点的纵坐标。

2. 时间复杂度

   若 `HasModify` 为 `true` ，为 $O(\log^2 n)$ ；否则为 $O(\log n)$ 。

3. 备注

   必须先调用 `prepare` ，再进行查询。

#### 5.增加矩形的权值(add_rect_value)

1. 数据类型

   输入参数 `size_type rect_id` ，表示要修改的矩形的编号。
   
   输入参数 `SumType w` ，表示要增加的权值。

2. 时间复杂度

    $O(\log^2 n)$ 。
    
3. 备注

   本方法仅当 `HasModify` 为 `true` 时可以使用。

#### 三、模板示例

```c++
#include "DS/RectAddPointSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RAPSC2D::Table<int, bool> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

void test_add() {
    OY::RAPSC2D::Table<int, uint32_t, uint32_t, true> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;

    // 编号为 1 的矩形的权值增加一百
    S.add_rect_value(1, 100);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

int main() {
    test();
    test_add();
}
```

```
#输出如下
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 101
val of S{0, 0}: 102

```

