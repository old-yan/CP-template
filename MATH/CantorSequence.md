### 一、模板类别

​	数学：逆康托展开。

### 二、模板功能

​	逆康托展开是指，求一堆元素的排列方式，使得其在所有可能的排列方式中的排名等于给定的排名。

#### 1.普通版

1. 数据类型

   输入参数 `uint32_t n` ，表示要排列的数的个数。

   输入参数 `uint32_t k`，表示要求的序列的排名。

   返回类型 `std::vector<uint32_t>` ，表示 `0 ~ n-1`  的一个排列。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本方法中，`k` 为基于 `0` 的排名。即，`k=0` 表示求最小排列，`k=1` 表示求次小排列。
   
   本方法返回的是 `0 ~ n-1` 的无重排列。
   
   如果 `k` 大于等于可能的排列种类数，返回空数组。

#### 2.扩展版

1. 数据类型

   输入参数 `_Iterator first` ，表示区间起点。

   输入参数 `_Iterator last` ，表示区间终点。（开区间）

   输入参数 `uint32_t k`，表示要求的序列的排名。

   返回类型 `std::vector<_Tp>` ，表示区间里的元素的一个排列。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本方法中，`k` 为基于 `0` 的排名。即，`k=0` 表示求最小排列，`k=1` 表示求次小排列。

   本方法返回的是给定区间内元素的排列。由于元素数量是由给定区间决定的，所以允许元素重复。

   如果 `k` 大于等于可能的排列种类数，返回空数组。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CantorSequence.h"

int main() {
    for (int k = 0; k < 6; k++) {
        auto v = OY::CantorSequence(3, k);
        cout << k << "-th permutation of [0,1,2]: ";
        for (auto a : v) cout << a << ' ';
        cout << endl;
    }
    std::string s = "acbc";
    for (int k = 0; k < 12; k++) {
        auto v = OY::CantorSequence_ex(s.begin(), s.end(), k);
        cout << k << "-th permutation of \"acbc\": ";
        for (auto a : v) cout << a;
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

