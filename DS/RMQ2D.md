### 一、模板类别

​	数据结构：二维 `RMQ` 表。

​	练习题目：

1. [Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
2. [P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)

### 二、模板功能


​		本模板完全为 `MaskRMQ` 与 `SparseTable` 的结合，可以在二维进行子矩形最值查询。

​		本模板功能较为单一，只能进行最值查询而不能进行其他半群信息的查询，因而本模板的时间复杂度也可以优于 `SparseTable2D` 和 `CatTable2D` 。初始化时间为 $O(m\cdot n\cdot \log m)$ ，也就是说，如果行数和列数不均衡，行数越小越快；查询时间复杂度为 $O(1)$ 。

### 三、模板示例

```c++
#include "DS/RMQ2D.h"
#include "IO/FastIO.h"

int main() {
    int matrix[4][5] = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
    };
    // 除了行数、列数，还需要传递一个寻址函数
    auto T = OY::RMQMaxTable2D<int>(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T << endl;
    // 输出[0,2]行，[1,4]列的最大值
    cout << "max(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;
}
```

```
#输出如下
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
max(matrix[0~2][1~4])=6

```
