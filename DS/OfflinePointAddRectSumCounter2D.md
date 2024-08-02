### 一、模板类别

​	数据结构：离线二维【单点加，矩形区域和查询】表

​	练习题目：

1. [P10814 【模板】离线二维数点](https://www.luogu.com.cn/problem/P10814)
2. [Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/118)


### 二、模板功能

​		本模板为 `PointAddRectSumCounter2D` 的离线版本，适用于先进行加点，再进行查询的问题。

​		本模板提供了一个 `Solver` ，将区间信息和查询全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示点的坐标类型； `typename WeightType` 表示点权类型。

​		 `solve` 方法的模板参数有 `typename SumType` 表示点权和的类型； `typename CountTree` 表示一个可以用来单点加、查询前缀和的数据结构。显然，树状数组满足这一条件；也可以使用 `WTree` 。默认参数为一颗简单的树状数组。


### 三、模板示例

```c++
#include "DS/OfflinePointAddRectSumCounter2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINEPARSC2D::Solver<int, bool> S;
    S.add_point(-1, -1);
    S.add_point(-1, 1);
    S.add_point(1, -1);
    S.add_point(1, 1);

    S.add_query(-1, 1, -1, 1);
    S.add_query(0, 1, -1, 1);

    auto res = S.solve();

    cout << "sum of S{-1<=x<=1, -1<=y<=1}: " << res[0] << endl;
    cout << "sum of S{0<=x<=1, -1<=y<=1}: " << res[1] << endl;
}

int main() {
    test();
}
```

```
#输出如下
sum of S{-1<=x<=1, -1<=y<=1}: 4
sum of S{0<=x<=1, -1<=y<=1}: 2

```

