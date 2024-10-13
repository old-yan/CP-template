### 一、模板类别

​	数据结构：滑动窗口。

​	练习题目：

1. [3321. 计算子数组的 x-sum II](https://leetcode.cn/problems/find-x-sum-of-all-k-long-subarrays-ii)


### 二、模板功能

​		本模板提供一个滑动窗口模板。

​		滑动窗口问题中，需要知道区间总长度、窗口长度、窗口左端收缩的回调函数、窗口右端收缩的回调函数、窗口就绪的回调函数。只要传递这些参数，本模板即可自动运行。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

#include <map>
void test() {
    // 窗口数颜色
    cout << "count color count:\n";
    int a[] = {4, 7, 5, 1, 8, 1, 4, 4, 1};

    int n = 9;
    int window_len = 4;
    std::map<int, int> mp;
    auto call = [&](int l, int r) {
        cout << "color count of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << mp.size() << endl;
    };
    auto left_call = [&](int i) {
        auto &cnt = mp[a[i]];
        if (!--cnt) mp.erase(a[i]);
    };
    auto right_call = [&](int i) {
        auto &cnt = mp[a[i]];
        ++cnt;
    };
    OY::WINDOW::solve(n, window_len, call, left_call, right_call);
}

int main() {
    test();
}
```

```
#输出如下
count color count:
color count of [4 7 5 1] = 4
color count of [7 5 1 8] = 4
color count of [5 1 8 1] = 3
color count of [1 8 1 4] = 3
color count of [8 1 4 4] = 3
color count of [1 4 4 1] = 2

```

