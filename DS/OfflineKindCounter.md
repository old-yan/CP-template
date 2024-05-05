### 一、模板类别

​	数据结构：离线区间元素种类数查询表

​	练习题目：

1. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
2. [Static Range Count Distinct](https://judge.yosupo.jp/problem/static_range_count_distinct)(https://github.com/yosupo06/library-checker-problems/issues/770)

### 二、模板功能

​		本模板为 `KindCounter` 的离线版本，由于是离线处理，所以只提供一个函数接口，将区间信息和查询全部塞进去，即可获得查询结果。

​		模板参数 `typename CountTree` 表示一个可以用来单点加、查询前缀和的数据结构。显然，树状数组满足这一条件；也可以使用 `WTree` 。默认参数为一颗简单的树状数组。

​		模板参数 `typename Tag` 的作用参见 `KindCounter` 模板。

​		本模板提供两个版本的 `solve` 函数，第一个 `solve` 需要使用者提供查询数量、查询生产者、区间长度、区间映射函数，效率较高，但是使用复杂；第二个 `solve` 只需要提供两个 `std::vector` 分别表示区间和查询，使用较为简单。

### 三、模板示例

```c++
#include "DS/OfflineKindCounter.h"
#include "IO/FastIO.h"

void test() {
    std::vector<int> arr{400, 900, 200, 300, 400, 300, 300, 100, 900};
    std::vector<std::pair<int, int>> queries{{1, 3}, {4, 6}, {1, 6}};
    auto res = OY::OFFLINEKC::solve(arr, queries);

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

