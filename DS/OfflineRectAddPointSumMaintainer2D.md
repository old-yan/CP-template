### 一、模板类别

​	数据结构：离线二维【矩形区域加，单点维护】表

​	练习题目：

1. [#134. 二维树状数组 2：区间修改，单点查询](https://loj.ac/p/134)
2. [Rectangle Add Point Get](https://judge.yosupo.jp/problem/rectangle_add_point_get)(https://github.com/yosupo06/library-checker-problems/issues/994)


### 二、模板功能

​		本模板为 `PointAddRectSumMaintainer2D` 的离线版本，适用于加点和查询混杂的问题。

​		本模板提供了一个 `Solver` ，将区间信息和查询全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示点的坐标类型； `typename WeightType` 表示点权类型； `typename SumType` 表示点权和的类型。

### 三、模板示例

```c++
#include "DS/OfflineRectAddPointSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERAPSM2D::Solver<int, int> S;

    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);

    S.add_query(1, -1);
    S.add_query(0, 0);

    S.add_rect(-1, 0, 0, 1, 100);
    S.add_rect(0, 1, -1, 0, 20);

    S.add_query(1, -1);
    S.add_query(0, 0);

    auto res = S.solve();
    cout << "before add rects:\n";
    cout << "val of S{1, -1}: " << res[0] << endl;
    cout << "val of S{0, 0}: " << res[1] << endl;
    
    cout << "after add rects:\n";
    cout << "val of S{1, -1}: " << res[2] << endl;
    cout << "val of S{0, 0}: " << res[3] << endl;
}

int main() {
    test();
}
```

```
#输出如下
before add rects:
val of S{1, -1}: 0
val of S{0, 0}: 1
after add rects:
val of S{1, -1}: 0
val of S{0, 0}: 101

```

