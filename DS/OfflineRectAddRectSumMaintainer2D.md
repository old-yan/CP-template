### 一、模板类别

​	数据结构：离线二维【矩形区域加，矩形和维护】表

​	练习题目：

1. [#135. 二维树状数组 3：区间修改，区间查询](https://loj.ac/p/135)


### 二、模板功能

​		本模板适用于加矩形和查询混杂的问题。

​		本模板提供了一个 `Solver` ，将区间信息和查询全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示矩形的坐标类型； `typename WeightType` 表示矩形权类型； `typename SumType` 表示矩形权值和的类型。

​		 `solve` 方法需要提供一个可以单点加、举行查询前缀和的辅助对象；建议提供一个 `GlobalHashBIT2D` 对象。

### 三、模板示例

```c++
#include "DS/GlobalHashBIT2D.h"
#include "DS/OfflineRectAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

void test() {
    OY::OFFLINERARSM2D::Solver<int, bool, int> S;
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);
    S.add_rect(-1, 0, 0, 1);
    S.add_rect(0, 1, -1, 0);
    S.add_query(-1, 1, -1, 1);
    S.add_query(-1, 0, 0, 1);
    S.add_query(0, 1, -1, 0);

    OY::GBIT2D::Tree<uint32_t, decltype(S)::node, false, false, 1 << 10> bit{};
    // 给 solve 传递一个辅助用的 bit
    auto res = S.solve(bit);
    cout << "before change:\n";
    cout << "val of S{-1~1, -1~1}: " << res[0] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[1] << endl;
    cout << "val of S{0~1, -1~0}: " << res[2] << endl;

    cout << "after change:\n";
    cout << "val of S{-1~1, -1~1}: " << res[3] << endl;
    cout << "val of S{-1~0, 0~1}: " << res[4] << endl;
    cout << "val of S{0~1, -1~0}: " << res[5] << endl;
}

int main() {
    test();
}
```

```
#输出如下
before change:
val of S{-1~1, -1~1}: 0
val of S{-1~0, 0~1}: 0
val of S{0~1, -1~0}: 0
after change:
val of S{-1~1, -1~1}: 8
val of S{-1~0, 0~1}: 5
val of S{0~1, -1~0}: 5

```

