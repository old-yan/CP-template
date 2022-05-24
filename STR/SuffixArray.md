### 一、模板类别

​	序列：后缀数组。

### 二、模板功能

   本模板最终目的为求出序列的后缀数组，以及从后缀数组衍生的名次数组，高度数组。
   后缀数组有多种求法，在序列很短时可以采用简单暴力法 `GetSuffixArray_naive` ；在序列长度中等时可以采用倍增法 `GetSuffixArray_doubling` ；在序列长度很长时可以采用 `GetSuffixArray_sa_is` 。如果采用 `GetSuffixArray` 表示按照区间长度自动选择适合的算法。

#### 1.构造后缀数组

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中的元素类型。

   模板参数 `bool _Copy` ，表示在后缀数组结构体中是否保留序列的副本。默认为 `true` 。

   模板参数 `typename _Solver` ，表示后缀数组的解法类。默认为 `GetSuffixArray`  。

   构造参数 `_Iterator __first` ，表示文本序列开始位置。

   构造参数 `_Iterator __last` ，表示文本序列结束位置。（开区间）

   构造参数 `uint32_t __alpha` ，表示文本序列的值域区间为 `[0, __alpha)` 。对于字符串，可为 `128` .

2. 时间复杂度

   简单暴力法 `GetSuffixArray_naive` 的时间复杂度为 $O(n^2\cdot \log n)$ ；
   倍增法 `GetSuffixArray_doubling` 的时间复杂度为 $O(n\cdot \log n)$ ；
   `GetSuffixArray_sa_is` 的时间复杂度为 $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   一个长度为 `n` 的字符串有 `n` 个后缀子串。如果将这些字串按照字典序排序，则它们的起始下标按照顺序形成一个 `[0, n)` 的序列，既为 `m_sa` 数组（后缀数组）。`s.substr(m_sa[i])` 表示排名 `i` 的后缀子串。

   后缀数组以子串名次为下标，以子串在原串的起始下标为值。反过来，如果以子串在原串的起始下标为下标，以该子串的名次为值，则形成了 `m_rank` 数组。`m_rank[i]` 表示 `s.substr(i)` 的排名。

   尽管我们找出了所有后缀子串的大小关系，但我们有时候还需要知道子串之间的相似程度———或者说，公共前缀。显然，字典序相邻的子串之间的公共前缀更有价值，所以我们求出所有字典序相邻的子串之间的公共前缀长度，形成 `m_height` 数组。`m_height[i]` 表示 `s.substr(m_sa[i-1])` 和 `s.substr(m_sa[i])` 的最长公共前缀长度。特别的，`m_height[0]` 为 `0` 。
   
   **注意：** 简单暴力法虽然时间复杂度高，但是常数很小，在字符串很短的时候是实际运行速度最快的；`GetSuffixArray_sa_is` 的时间复杂度虽然低，但是常数较大，所以只适合在字符串很长的时候使用。

#### 2.查询大于等于某模式串的子串位置

1. 数据类型

   输入参数 `_Iterator __first` ，表示模式序列开始位置。

   输入参数 `_Iterator __last` ，表示模式序列结束位置。（开区间）

   返回类型 `uint32_t` ，表示大于等于模式串的最小子串的名次。

2. 时间复杂度

   $O(m\log n)$ ，此处 `m` 表示模式串长度，`n` 表示文本串长度。

3. 备注

   本方法要求模板参数 `_Copy == true` 。

#### 3.查询大于某模式串的子串位置

1. 数据类型

   输入参数 `_Iterator __first` ，表示模式序列开始位置。

   输入参数 `_Iterator __last` ，表示模式序列结束位置。（开区间）

   返回类型 `uint32_t` ，表示大于模式串的最小子串的名次。

2. 时间复杂度

   $O(m\log n)$ ，此处 `m` 表示模式串长度，`n` 表示文本串长度。

3. 备注

   本方法要求模板参数 `_Copy == true` 。

### 三、模板示例

```c++
#include "DS/STtable.h"
#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

int main() {
    std::string s = "abcabaaabca";
    OY::SuffixArray SA(s.begin(), s.end(), 128);
    //按照字典序枚举后缀子串，且查询其与前一个串的最长公共前缀长度
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(SA.m_sa[i]) << ' ' << SA.m_height[i] << endl;
    //查询每个子串的名次
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(i) << " 's rank = " << SA.m_rank[i] << endl;
    //查询模式串 ab 的所有出现位置
    std::string p = "ab";
    int lower = SA.lower_bound(p.begin(), p.end());
    int upper = SA.upper_bound(p.begin(), p.end());
    for (int i = lower; i < upper; i++)
        cout << SA.m_sa[i] << " is the index of \"ab\"\n";
    //查询两个后缀子串的大小关系
    std::string s1 = s.substr(3);
    std::string s2 = s.substr(7);
    if (SA.m_rank[3] < SA.m_rank[7])
        cout << s1 << " is smaller than " << s2 << endl;
    else
        cout << s1 << " is bigger than " << s2 << endl;

    //**********************************************************************
    //借助线段树、st 表等数据结构，还可以快速查询任意两个后缀子串的最长公共前缀
    //假如我们要处理的查询数很多，我们会选择 st 表。（初始化略慢，查询很快）
    OY::STTable st(SA.m_height.begin(), SA.m_height.end(), std::min);
    int lcp = SA.m_rank[3] < SA.m_rank[7] ? st.query(SA.m_rank[3] + 1, SA.m_rank[7]) : st.query(SA.m_rank[7] + 1, SA.m_rank[3]);
    cout << s.substr(3, lcp) << " is the longest common prefix of " << s1 << " and " << s2 << endl;
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
3 is the index of "ab"
7 is the index of "ab"
0 is the index of "ab"
abaaabca is smaller than abca
ab is the longest common prefix of abaaabca and abca


```

