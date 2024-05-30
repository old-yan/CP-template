### 一、模板类别

​	数据结构：离线区间元素种类数查询表

​	练习题目：

1. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
2. [Static Range Count Distinct](https://judge.yosupo.jp/problem/static_range_count_distinct)(https://github.com/yosupo06/library-checker-problems/issues/770)

### 二、模板功能

​		本模板为 `KindCounter` 的离线版本，将所有查询加入到模板内后，统一计算，计算结束后返回所有查询的结果。

​		各种参数的意义参考 `KindCounter` 。

### 三、模板示例

```c++
#include "DS/OfflineKindCounter.h"
#include "IO/FastIO.h"

void test() {
    std::vector<int> arr{400, 900, 200, 300, 400, 300, 300, 100, 900};
    std::vector<std::pair<int, int>> queries{{1, 3}, {4, 6}, {1, 6}};

    OY::OfflineArrayKindCounter<901> kc(arr.begin(), arr.end());
    for (int i = 0; i < 3; i++) kc.add_query(queries[i].first, queries[i].second);
    auto res = kc.solve();

    for (int i = 0; i < queries.size(); i++)
        cout << "kind count of arr[" << queries[i].first << "~" << queries[i].second << "] = " << res[i] << endl;
}

int main() {
    test();
}
```

```
#输出如下
kind count of arr[1~3] = 3
kind count of arr[4~6] = 2
kind count of arr[1~6] = 4

```

