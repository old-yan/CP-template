### 一、模板类别

​	数据结构：二维【矩形区域加，单点维护】表

​	练习题目：

1. [#134. 二维树状数组 2：区间修改，单点查询](https://loj.ac/p/134)


### 二、模板功能

#### 1.建立查询表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename SizeType` ，表示坐标的类型。
   
   模板参数 `typename WeightType` ，表示权值的类型，默认为 `bool` ，表示每个权值均为 `1` 。
   
   模板参数 `typename SumType` ，表示权值和的类型。
   
   模板参数 `size_type BATCH` ，表示一个阈值，低于此阈值的零散矩形会进行暴力处理；高于此阈值的零散矩形会加入表中。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本模板用于在线任务，可以边加入矩形，边进行在线点权查询。
   
   当 `WeightType` 为 `bool` 时，加入的点的点权固定为 `1` ， `weight_type` 为 `uint32_t` ；否则 `weight_type` 等于 `WeightType` 。


#### 2.加入矩形(add_rect)

1. 数据类型

   输入参数 `SizeType x_min` ，表示矩形的横坐标最小值。
   
   输入参数 `SizeType x_max` ，表示矩形的横坐标最大值。
   
   输入参数 `SizeType y_min` ，表示矩形的纵坐标最小值。
   
   输入参数 `SizeType y_max` ，表示矩形的纵坐标最大值。
   
   输入参数 `weight_type w` ，表示权。

2. 时间复杂度

   均摊 $O(\log^2 n)$ 。
    
3. 备注

   当 `WeightType` 为 `bool` 时无需传递点权。

#### 3.查询(query)

1. 数据类型

   输入参数 `SizeType x` ，表示点的横坐标。

   输入参数 `SizeType y` ，表示点的纵坐标。

2. 时间复杂度

    $O(\log^2 n)$ 。

#### 三、模板示例

```c++
#include "DS/RectAddPointSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::RAPSM2D::Table<int, int> S;

    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;

    S.add_rect(-1, 0, 0, 1, 100);
    S.add_rect(0, 1, -1, 0, 20);

    cout << "val of S{1, -1}: " << S.query(1, -1) << endl;
    cout << "val of S{0, 0}: " << S.query(0, 0) << endl;
}

int main() {
    test();
}
```

```
#输出如下
val of S{1, -1}: 1
val of S{0, 0}: 2
val of S{1, -1}: 21
val of S{0, 0}: 122

```

