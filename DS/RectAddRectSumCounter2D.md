### 一、模板类别

​	数据结构：二维【矩形区域加，矩形区域查询】表


### 二、模板功能

#### 1.建立查询表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename SizeType` ，表示坐标的类型。
   
   模板参数 `typename WeightType` ，表示权值的类型 。
   
   模板参数 `typename SumType` ，表示权值和的类型。
   
   构造参数 `size_type rect_cnt` ，表示加入的矩形数。默认为 `0` 。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板用于一次性加入很多矩形，然后再进行在线矩形和查询。
   
   当 `WeightType` 为 `bool` 时，加入的权固定为 `1` ， `weight_type` 为 `uint32_t` ；否则 `weight_type` 等于 `WeightType` 。
   
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

   输入参数 `SizeType x_min` ，表示矩形的横坐标最小值。

   输入参数 `SizeType x_max` ，表示矩形的横坐标最大值。

   输入参数 `SizeType y_min` ，表示矩形的纵坐标最小值。

   输入参数 `SizeType y_max` ，表示矩形的纵坐标最大值。

2. 时间复杂度

    $O(\log^2 n)$ 。

3. 备注

   必须先调用 `prepare` ，再进行查询。

#### 三、模板示例

```c++
#include "DS/RectAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RARSC2D::Table<int, bool, int> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.prepare();

    cout << "val of S{-1~1, -1~1}: " << S.query(-1, 1, -1, 1) << endl;
    cout << "val of S{-1~0, 0~1}: " << S.query(-1, 0, 0, 1) << endl;
    cout << "val of S{0~1, -1~0}: " << S.query(0, 1, -1, 0) << endl;
}

int main() {
    test();
}
```

```
#输出如下
val of S{-1~1, -1~1}: 8
val of S{-1~0, 0~1}: 5
val of S{0~1, -1~0}: 5

```

