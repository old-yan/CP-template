### 一、模板类别

​	数据结构：区间 `mex` 离线查询表

​	练习题目：

1. [2003. 每棵子树内缺失的最小基因值](https://leetcode.cn/problems/smallest-missing-genetic-value-in-each-subtree/)
2. [P4137 Rmq Problem / mex](https://www.luogu.com.cn/problem/P4137)


### 二、模板功能

​		本模板为 `RangeMex` 的离线版本，将所有查询加入到模板内后，统一计算，计算结束后返回所有查询的结果。

#### 三、模板示例

```c++
#include "DS/OfflineRangeMex.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[7] = {1, 3, 0, 2, 0, 1, 5};
    OY::OfflineRangeMexSolver rm(7, 4);
    rm.add_query(0, 1);
    rm.add_query(1, 3);
    rm.add_query(4, 6);
    rm.add_query(1, 6);
    auto res = rm.solve(arr, arr + 7);

    cout << "mex of arr[0~1] = " << res[0] << endl;
    cout << "mex of arr[1~3] = " << res[1] << endl;
    cout << "mex of arr[4~6] = " << res[2] << endl;
    cout << "mex of arr[1~6] = " << res[3] << endl;
}

int main() {
    test();
}
```

```
#输出如下
mex of arr[0~1] = 0
mex of arr[1~3] = 1
mex of arr[4~6] = 2
mex of arr[1~6] = 4

```

