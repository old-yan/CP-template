### 一、模板类别

​	序列：后缀数组。

​	练习题目：

1. [P1368 【模板】最小表示法](https://www.luogu.com.cn/problem/P1368)
2. [P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
3. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
4. [P2870 [USACO07DEC] Best Cow Line G](https://www.luogu.com.cn/problem/P2870)
5. [P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
6. [P3804 【模板】后缀自动机（SAM）](https://www.luogu.com.cn/problem/P3804)
7. [P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
8. [P4051 [JSOI2007] 字符加密](https://www.luogu.com.cn/problem/P4051)
9. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
10. [Suffix Array](https://judge.yosupo.jp/problem/suffixarray)(https://github.com/yosupo06/library-checker-problems/issues/50)
11. [Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings)(https://github.com/yosupo06/library-checker-problems/issues/123)
12. [Longest Common Substring](https://judge.yosupo.jp/problem/longest_common_substring)(https://github.com/yosupo06/library-checker-problems/issues/889)

### 二、模板功能

   本模板最终目的为求出序列的后缀数组，以及从后缀数组衍生的名次数组，高度数组。
   后缀数组有多种求法，其中 `SA-IS` 可以在线性时间内求解，且常数较小。故采用 `SA-IS` 算法。

#### 1.构造后缀数组

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板中表示下标、大小的类型。
   
   模板参数 `bool Rank` ，表示是否求出每个后缀子串的排名。
   
   模板参数 `bool Height` ，表示是否求出相邻排名后缀子串的 `LCP` 。
   
   模板参数 `size_type MAX_LEN` ，表示求解的最大字符串长度。

   构造参数 `size_type length` ，表示文本序列长度。

   构造参数 `InitMapping mapping` ，表示文本序列下标对元素的映射。

2. 时间复杂度

    $O(n+\Sigma)$ ，此处 `n` 指字符串长度， `Σ` 指元素值域。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   一个长度为 `n` 的字符串有 `n` 个后缀子串。如果将这些字串按照字典序排序，则它们的起始下标按照顺序形成一个 `[0, n)` 的序列，既为 `m_sa` 数组（后缀数组）。`s.substr(m_sa[i])` 表示排名 `i` 的后缀子串。

   后缀数组以子串名次为下标，以子串在原串的起始下标为值。反过来，如果以子串在原串的起始下标为下标，以该子串的名次为值，则形成了 `m_rank` 数组。`m_rank[i]` 表示 `s.substr(i)` 的排名。

   尽管我们找出了所有后缀子串的大小关系，但我们有时候还需要知道子串之间的相似程度———或者说，公共前缀。显然，字典序相邻的子串之间的公共前缀更有价值，所以我们求出所有字典序相邻的子串之间的公共前缀长度，形成 `m_height` 数组。`m_height[i]` 表示 `s.substr(m_sa[i-1])` 和 `s.substr(m_sa[i])` 的最长公共前缀长度。特别的，`m_height[0]` 为 `0` 。
   
   **注意：** 
   
   本模板要求序列中所有元素可以类型转换为数字，且元素的值域非负。若值域过大，请对元素值进行离散化处理；若值域有负值元素，请对元素值进行偏移处理。
   
   如果传入的元素的值域范围超出 `MAX_LEN` 大小，或者最小元素小于 `0` ，则在模板中会对元素进行离散化处理；此时模板的时间复杂度会增加一个 `log` 。

#### 2.构造后缀数组

1. 数据类型

   构造参数 `Iterator first` ，表示文本序列区间开始位置。

   构造参数 `Iterator last` ，表示文本序列区间结束位置。（开区间）

   构造参数 `size_type alpha` ，表示元素值域大小。默认为 `0` ，表示需要现算。

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `uint32_t length` ，表示文本序列长度。

   输入参数 `InitMapping mapping` ，表示文本序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示文本序列区间开始位置。

   输入参数 `Iterator last` ，表示文本序列区间结束位置。（开区间）

   构造参数 `size_type alpha` ，表示元素值域大小。默认为 `-1` ，表示需要现算。

2. 时间复杂度

   同上。

#### 5.查询第 k 大后缀子串(query_sa)

1. 数据类型

   输入参数 `size_type rank` ，表示要查询的排名。

   返回类型 `size_type` ，表示该排名的后缀子串的起始下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板的下标从 `0` 开始，所以排名为 `0` 的是字典序最小的后缀子串；返回的下标范围在 `[0, n)` 范围内。

#### 6.查询后缀子串字典序排名(query_rank)

1. 数据类型

   输入参数 `size_type pos` ，表示要查询的子串起始下标。

   返回类型 `size_type` ，表示该后缀子串的字典序排名。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板的下标从 `0` 开始，所以排名为 `0` 的是字典序最小的后缀子串；下标范围在 `[0, n)` 范围内。

#### 7.查询高度(query_height)

1. 数据类型

   输入参数 `size_type rank` ，表示要查询的排名。

   返回类型 `size_type` ，表示该排名的后缀子串与比其略小的后缀子串的公共前缀长度。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   特别的，当输入参数为 `0` 时，该子串没有”比其略小的后缀子串“。所以返回零。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

int main() {
    std::string s = "abcabaaabca";
    OY::SA::SuffixArray<true, true, 1000> SA(s);

    // 按照字典序枚举后缀子串，且查询其与前一个串的最长公共前缀长度
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(SA.query_sa(i)) << ' ' << SA.query_height(i) << endl;
    // 查询每个子串的名次
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(i) << " 's rank = " << SA.query_rank(i) << endl;

    // 查询两个后缀子串的大小关系
    std::string s1 = s.substr(3);
    std::string s2 = s.substr(7);
    if (SA.m_rank[3] < SA.m_rank[7])
        cout << s1 << " is smaller than " << s2 << endl;
    else
        cout << s1 << " is bigger than " << s2 << endl;
}
```

```
#输出如下
a 0
aaabca 1
aabca 2
abaaabca 1
abca 2
abcabaaabca 4
baaabca 0
bca 1
bcabaaabca 3
ca 0
cabaaabca 2
abcabaaabca 's rank = 5
bcabaaabca 's rank = 8
cabaaabca 's rank = 10
abaaabca 's rank = 3
baaabca 's rank = 6
aaabca 's rank = 1
aabca 's rank = 2
abca 's rank = 4
bca 's rank = 7
ca 's rank = 9
a 's rank = 0
abaaabca is smaller than abca

```

