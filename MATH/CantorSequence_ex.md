### 一、模板类别

​	数学：逆康托展开。

​	练习题目：

1. [1643. 第 K 条最小指令](https://leetcode.cn/problems/kth-smallest-instructions/)

### 二、模板功能

​	逆康托展开是指，求一堆元素的排列方式，使得其在所有可能的排列方式中的排名等于给定的排名。

​	本模板处理的是可重集合的逆康托展开，且对元素的类型、范围没有要求，只需要元素支持大小比较即可。

​	本方法中，`k` 为基于 `0` 的排名。即，`k = 0` 表示求最小排列，`k = 1` 表示求次小排列。

​	本方法返回的是给定区间内元素的排列。由于元素数量是由给定区间决定的，所以允许元素重复。

​	如果 `k` 大于等于可能的排列种类数，返回空数组。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CantorSequence_ex.h"

int main() {
    std::string s = "acbc";
    for (int k = 0; k < 12; k++) {
        auto v = OY::get_Cantor_sequence_ex(s.begin(), s.end(), k);
        cout << k << "-th permutation of \"acbc\": ";
        for (auto a : v) cout << a;
        cout << endl;
    }
}
```

```
#输出如下
0-th permutation of "acbc": abcc
1-th permutation of "acbc": acbc
2-th permutation of "acbc": accb
3-th permutation of "acbc": bacc
4-th permutation of "acbc": bcac
5-th permutation of "acbc": bcca
6-th permutation of "acbc": cabc
7-th permutation of "acbc": cacb
8-th permutation of "acbc": cbac
9-th permutation of "acbc": cbca
10-th permutation of "acbc": ccab
11-th permutation of "acbc": ccba

```

