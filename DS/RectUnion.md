### 一、模板类别

​	数据结构：矩形面积并。

​	练习题目：

1. [P5490 【模板】扫描线](https://www.luogu.com.cn/problem/P5490)
2. [Area of Union of Rectangles](https://judge.yosupo.jp/problem/area_of_union_of_rectangles)(https://github.com/yosupo06/library-checker-problems/issues/787)


### 二、模板功能

​		本模板适用于离线计算矩形面积并的问题。

​		本模板提供了一个 `Solver` ，将矩形信息全部塞进去，即可获得查询结果。

​		类的模板参数有 `typename SizeType` 表示矩形的坐标类型。

​		 `solve` 方法需要提供矩形面积类型；注意面积类型可能和坐标类型不同。

### 三、模板示例

```c++
#include "DS/RectUnion.h"
#include "IO/FastIO.h"

int main() {
    OY::RU::Solver<int> sol;
    sol.add_rect(-2, 0, -2, 0);
    sol.add_rect(0, 2, 0, 2);
    // 两个 3*3 的矩形，重叠了一个角
    cout << sol.solve();
}
```

```
#输出如下
17

```

