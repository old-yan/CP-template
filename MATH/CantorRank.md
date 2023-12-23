### 一、模板类别

​	数学：康托展开。

​	练习题目：

1. [P2524 Uim的情人节礼物·其之弐](https://www.luogu.com.cn/problem/P2524)
2. [P3014 [USACO11FEB] Cow Line S](https://www.luogu.com.cn/problem/P3014)
3. [P5367 【模板】康托展开](https://www.luogu.com.cn/problem/P5367)

### 二、模板功能

   本模板解决的是一堆元素当前排列方式在其所有可能的排列方式里的排名问题。

​	本模板处理的是无重集合的康托展开，对元素的类型、范围没有要求，只需要元素支持大小比较即可。

​	本模板中，排名基于 `0` 。即，若输入的序列为升序排列，则查询得的排名为 `0` 。

​	本模板返回的排名为自取模数类型。

​	本模板的 `raw_query` 方法不会进行离散化，适合区间内的元素为数字类型，且数字范围较小，无需离散化的情况。

​	本模板的 `query` 方法会进行离散化，适合区间内的元素不为数字类型，或者虽然是数字类型，但是数字范围较大的情况。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CantorRank.h"
#include "MATH/StaticModInt32.h"

using mint = OY::mint1000000007;
int main() {
    int A[] = {4, 20, 9, 1, 6, 11, 2, 8, 0, 5, 7, 15, 19};
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;
    std::swap(A[0], A[1]);
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;
    std::sort(A, A + 13);
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;

    // 非数字类型需要离散化
    std::string s = "acb";
    cout << s << "'s rank is " << OY::CantorRank<mint>::query(s.begin(), s.end()) << endl;
}
```

```
#输出如下
A's rank mod 1000000007 is 902001313
A's rank mod 1000000007 is 893681285
A's rank mod 1000000007 is 0
acb's rank is 1

```

