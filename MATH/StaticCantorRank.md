### 一、模板类别

​	数学：康托展开（静态模数）。

### 二、模板功能

   本模板分为普通版和扩展版，普通版用于解决序列内无重复元素的情况，而扩展版可以进一步解决序列内有重复元素的情况。
   在无重序列种，首先考虑使用普通版，因为其速度最快。

#### 1.构造

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   模板参数 `_ModType _P` ，表示模数。

   构造参数 `uint32_t __maxNumber` ，表示序列中同一元素最多出现多少次。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指 `__maxNumber`。

3. 备注

   由于普通版的计算过程不需要乘法逆元，所以普通版无需构造函数，可以直接以静态方法的方式调用函数。只有扩展版需要构造函数。

   因为在计算中大量涉及到乘法逆元，所以在构造函数中对相应范围的乘法逆元进行打表。

#### 2.查询排名（无需离散化）

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间起点。

   输入参数 `_Iterator __last` ，表示区间终点。（开区间）

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本方法适用于区间内的元素为数字类型，且数字范围较小，无需离散化的情况。

   本方法返回的排名以 `0` 为基准。即，若输入的序列为升序排列，则查询得的排名为 `0` 。

#### 3.查询排名（需要离散化）

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间起点。

   输入参数 `_Iterator __last` ，表示区间终点。（开区间）

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本方法适用于区间内的元素不为数字类型，或者虽然为数字类型，但是数字范围较大，需要离散化的情况。

   本方法返回的排名以 `0` 为基准。即，若输入的序列为升序排列，则查询得的排名为 `0` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticCantorRank.h"

int main() {
    int A[] = {4, 20, 9, 1, 6, 11, 2, 8, 0, 5, 7, 15, 19};
    cout << "A's rank mod 1000000007 is " << OY::StaticCantorRank32<1000000007>::rawQuery(A, A + 13) << endl;
    std::swap(A[0], A[1]);
    cout << "A's rank mod 1000000007 is " << OY::StaticCantorRank32<1000000007>::rawQuery(A, A + 13) << endl;
    std::sort(A, A + 13);
    cout << "A's rank mod 1000000007 is " << OY::StaticCantorRank32<1000000007>::rawQuery(A, A + 13) << endl;

    //非数字类型需要离散化
    std::string s = "acb";
    cout << "\"acb\"s rank is " << OY::StaticCantorRank32<1000000007>::query(s.begin(), s.end()) << endl;

    //较大范围的数字类型需要离散化
    long long B[] = {33333333333333, 33333333333333, 22222222222222, 33333333333333, 11111111111111};
    cout << "B's rank is " << OY::StaticCantorRank_ex32<1000000007>(3).query(B, B + 5) << endl;
}
```

```
#输出如下
A's rank mod 1000000007 is 902001313
A's rank mod 1000000007 is 893681285
A's rank mod 1000000007 is 0
"acb"s rank is 1
B's rank is 17

```

