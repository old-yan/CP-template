### 一、模板类别

​	数据结构：离线二维【单点加，矩形区域和查询】表

​	练习题目：

1. [Static Rectangle Add Rectangle Sum](https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/771)

### 二、模板功能

​		本模板为 `RectAddRectSumCounter2D` 的离线版本，适用于先进行加矩形，再进行查询的问题。

​		本模板提供了一个 `Solver` ，将区间信息和查询全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示矩形的坐标类型； `typename WeightType` 表示矩形的权类型； `typename SumType` 表示矩形的权值和类型。


### 三、模板示例

```c++
#include "DS/OfflineRectAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERARSC2D::Solver<int, bool, int> S;
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);

    auto res = S.solve();
    cout << "val of S{-1~1, -1~1}: " << res[0] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[1] << endl;
    cout << "val of S{0~1, -1~0}: " << res[2] << endl;
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

