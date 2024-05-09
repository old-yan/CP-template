### 一、模板类别

​	数据结构：离线二维【单点加，矩形区域和维护】表

​	练习题目：

1. [#133. 二维树状数组 1：单点修改，区间查询](https://loj.ac/p/133)
2. [Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/519)


### 二、模板功能

​		本模板为 `PointAddRectSumMaintainer2D` 的离线版本，适用于加点和查询混杂的问题。

​		本模板提供了一个 `Solver` ，将区间信息和查询全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示点的坐标类型； `typename WeightType` 表示点权类型； `typename SumType` 表示点权和的类型。

### 三、模板示例

```c++
#include "DS/OfflinePointAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINEPARSM2D::Solver<int, bool> S;
    S.add_query(-1, 1, -1, 1);
    S.add_query(0, 1, -1, 1);

    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);

    S.add_query(-1, 1, -1, 1);
    S.add_query(0, 1, -1, 1);

    auto res = S.solve();

    cout << "before add points:\n";
    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << res[0] << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << res[1] << endl;

    cout << "after add points:\n";
    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << res[2] << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << res[3] << endl;
}

int main() {
    test();
}
```

```
#输出如下
before add points:
sum of S{-1<=x<=1, -1<=y<=1}: 0
sum of S{0<=x<=1, -1<=y<=1}: 0
after add points:
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2

```

