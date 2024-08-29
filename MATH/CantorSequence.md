### 一、模板类别

​	数学：逆康托展开。

​	练习题目：

1. [60. 排列序列](https://leetcode.cn/problems/permutation-sequence/)
2. [P3014 [USACO11FEB] Cow Line S](https://www.luogu.com.cn/problem/P3014)



### 二、模板功能

​	逆康托展开是指，求一堆元素的排列方式，使得其在所有可能的排列方式中的排名等于给定的排名。

​	本模板处理的是无重集合的逆康托展开，且对元素的类型、范围没有要求，只需要元素支持大小比较即可。

​	本方法中，`k` 为基于 `0` 的排名。即，`k = 0` 表示求最小排列，`k = 1` 表示求次小排列。

​	本方法返回的是给定区间内元素的排列。由于元素数量是由给定区间决定的，所以允许元素重复。

​	如果 `k` 大于等于可能的排列种类数，返回空数组。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CantorSequence.h"

int main() {
    for (int k = 0; k < 6; k++) {
        auto v = OY::get_Cantor_sequence(3, k);
        cout << k << "-th permutation of [0,1,2]: ";
        for (auto a : v) cout << a << ' ';
        cout << endl;
    }
}
```

```
#输出如下
0-th permutation of [0,1,2]: 0 1 2 
1-th permutation of [0,1,2]: 0 2 1 
2-th permutation of [0,1,2]: 1 0 2 
3-th permutation of [0,1,2]: 1 2 0 
4-th permutation of [0,1,2]: 2 0 1 
5-th permutation of [0,1,2]: 2 1 0 

```

