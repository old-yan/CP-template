### 一、模板类别

​	序列：`最长公共前缀助手-后缀数组版` 。

### 二、模板功能

#### 1.构造助手

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中元素的类型。默认为 `char` 。

   模板参数 `typename _SASolver` ，表示计算后缀数组的类。默认为 `GetSuffixArray` 。

   模板参数 `typename _STSolver` ，表示进行区间查询的类。默认为 `STTable` 。

   构造参数 `_Iterator __first` ，表示序列开始位置。

   构造参数 `_Iterator __last` ，表示序列结束位置。（开区间）

   构造参数 `uint32_t __alpha` ，表示序列中的元素值域范围。默认为 `128` 。

2. 时间复杂度

   计算后缀数组部分，为 $O(n)\sim O(n^2\cdot\log n)$ ，取决于后缀数组算法。

   维护区间最值部分，为 $O(n)\sim O(n\cdot\log n)$ ，取决于区间查询的类。一般情况下选择 `ST` 表即可。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   后缀数组可以将字符串的所有后缀子串按照字典序排序，并计算出排序后的后缀子串的邻串之间的最长公共前缀。假如我们已知某个串按字典序排名第 `4` ，另一个串按字典序排名第 `7` ，那么我们只需要在排名第 `4` 和排名第 `5` 的 `lcp` 、排名第 `5` 和排名第 `6` 的 `lcp` 、排名第 `6` 和排名第 `7` 的 `lcp` 之间取最小，就可以得知排名第 `4` 和排名第 `7` 的串的 `lcp` 。

#### 2.查询子串最长公共前缀

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __b` ，表示第二个子串的开始位置下标。

   输入参数 `uint32_t __limit` ，表示要求的最长公共前缀的上限。。

2. 时间复杂度

   $O(1)\sim O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `__limit` 为可能的子串长度。

#### 3.查询子串最长公共前缀

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __b` ，表示第二个子串的开始位置下标。

2. 时间复杂度

   $O(1)\sim O(\log n)$ ，此处 `n` 指序列长度。

#### 4.比较子串大小

1. 数据类型

   输入参数 `uint32_t __l1` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __r1` ，表示第一个子串的结束位置下标。

   输入参数 `uint32_t __l2` ，表示第二个子串的开始位置下标。

   输入参数 `uint32_t __r2` ，表示第二个子串的结束位置下标。

   返回类型 `int` 。

2. 时间复杂度

   $O(1)\sim O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/LongestCommonPrefix_sa.h"

int main() {
    //给出主串
    std::string s = "abcabbbabcb";
    //构造助手
    OY::LongestCommonPrefix_sa<char> lcp(s.begin(), s.end());
    //查询 lcp
    cout << s.substr(0) << " and " << s.substr(7) << " 's lcp = " << lcp.lcp(0, 7) << endl;
    //查询带上限的 lcp
    cout << s.substr(0, 2) << " and " << s.substr(7, 2) << " 's lcp = " << lcp.lcp(0, 7, 2) << endl;
    //比较子串
    int res = lcp.compare(0, 3, 7, 10);
    if (res < 0)
        cout << s.substr(0, 4) << " is smaller than " << s.substr(7, 4) << endl;
    else if (res > 0)
        cout << s.substr(0, 4) << " is bigger than " << s.substr(7, 4) << endl;
    else
        cout << s.substr(0, 4) << " is equal to " << s.substr(7, 4) << endl;
}
```

```
#输出如下
abcabbbabcb and abcb 's lcp = 3
ab and ab 's lcp = 2
abca is smaller than abcb

```

