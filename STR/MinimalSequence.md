### 一、模板类别

​	序列：最小表示法。

​	练习题目：

1. [How many](https://acm.hdu.edu.cn/showproblem.php?pid=2609)
2. [P1368 【模板】最小表示法](https://www.luogu.com.cn/problem/P1368)
3. [P1709 [USACO5.5] 隐藏口令 Hidden Password](https://www.luogu.com.cn/problem/P1709)

### 二、模板功能

#### 1.计算最小表示的起始下标(get_minimal_rotation)

1. 数据类型

   输入参数 `Iterator first` ，表示序列开始位置。

   输入参数 `Iterator last` ，表示序列结束位置。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   一个字符串，以某一个下标作为起点，走到字符串结尾；再返回字符串开头，走到起点位置。这样的起点选取方式有 `n` 种，而每个起点对应了原字符串的一种循环重构。在这 `n` 种循环重构里，字典序最小的就是它的最小表示。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/MinimalSequence.h"

int main() {
    std::string s = "59813621";
    for (int i = 0; i < s.size(); i++) {
        cout << "starts with index " << i << " : " << s.substr(i) + s.substr(0, i) << endl;
    }

    auto start = OY::get_minimal_rotation(s.begin(), s.end());
    cout << s.substr(start) + s.substr(0, start) << " is the smallest\n";
}
```

```
#输出如下
starts with index 0 : 59813621
starts with index 1 : 98136215
starts with index 2 : 81362159
starts with index 3 : 13621598
starts with index 4 : 36215981
starts with index 5 : 62159813
starts with index 6 : 21598136
starts with index 7 : 15981362
13621598 is the smallest

```

