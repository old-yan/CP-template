### 一、模板类别

​	数学：拉格朗日插值。

### 二、模板功能

#### 1.构造

1. 数据类型

   构造参数 `uint32_t __pointNum` ，表示点数。默认为 `0` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本模板根据给出的 `n` 个点，拟合一个不超过 `n-1` 次的多项式，当输入新的自变量时，输出相应的应变量。但是，本模板并不能给出多项式每一项的系数值。

   注意： `__pointNum` 只是为了预留合适的点空间，所以即使不精确也不会影响模板使用。

#### 2.添加已知点

1. 数据类型

   输入参数 `_Tp __x` ，表示已知点的横坐标。

   输入参数 `_Tp __y` ，表示已知点的纵坐标。

2. 时间复杂度

   $O(1)$ 。

#### 3.预备

1. 数据类型

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   当输入所有已知点后，调用此方法，进行预处理。

#### 4.在特殊自变量下的预处理

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   如果输入的已知点的横坐标间隔为 `1` ，且按递增顺序输入，那么可以采用特殊方式进行预处理。

#### 5.根据自变量计算应变量

1. 数据类型

   输入参数 `_Tp __x` ，表示要计算的点的横坐标。

2. 时间复杂度

   $O(n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LagrangeInterpolation.h"
#include "MATH/StaticModInt32.h"

int main() {
    //拉格朗日插值模板的变量类型必须为某种自取模数。
    using mint = OY::StaticModInt32<1000000007, true>;
    //不妨输入一个多项式：y = 2 x^4 + 3 x^3 - 10 x^2 +100
    OY::LagrangeInterpolation<mint> li(5);
    li.addPoint(10, 22100);
    li.addPoint(3, 253);
    li.addPoint(9, 14599);
    li.addPoint(0, 100);
    li.addPoint(22, 495716);
    //预处理
    li.prepare();
    //计算点位置
    cout << "when x = 5, y = " << li.calc(5) << '\n';

    //*********************************************
    //再来试一个抛物线
    //构造参数不填是没关系的
    OY::LagrangeInterpolation<mint> li2;
    li2.addPoint(10, 0);
    li2.addPoint(11, 1);
    li2.addPoint(12, 4);
    li2.addPoint(13, 9);
    //预处理
    li2.prepareIfSpecialX();
    //计算点位置
    cout << "when x = 5, y = " << li2.calc(14) << '\n';
}
```

```
#输出如下
when x = 5, y = 1475
when x = 5, y = 16

```

