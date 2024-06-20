### 一、模板类别

​	序列：`Z` 算法。

​	练习题目：

1. [P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
2. [P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
3. [P5410 【模板】扩展 KMP/exKMP（Z 函数）](https://www.luogu.com.cn/problem/P5410)
4. [Z Algorithm](https://judge.yosupo.jp/problem/zalgorithm)(https://github.com/yosupo06/library-checker-problems/issues/49)

### 二、模板功能

#### 1.构造 Z 函数

1. 数据类型

   类型设定 `value_type = typename Sequence::value_type` ，表示序列里的元素类型。

   模板参数 `typename Sequence` ，表示序列类型。

   构造参数 `uint32_t length` ，表示模式序列长度。

   构造参数 `InitMapping mapping` ，表示模式序列下标到元素的映射。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   要使用 `Z` 算法模板，必须了解的一个概念就是 `Z` 函数。不妨设 `f(i)` 表示下标 `i` 处的前缀函数值，则 `f(i)` 为满足以下条件的最大值： ` s[0 ~ f(i) - 1] == s[i ~ i + f(i) - 1] ` ，也就是以下标 `0` 为起点和以下标 `i` 为起点的最长相同子串的长度。

   特别的，下标 `0` 处的 `Z` 函数值为 `0`。

   怎样利用前缀函数来进行字符串查找呢？不妨设模式串为 `p` ，文本串为 `s` ，我们可以另起一个新字符串 `q = p + '#' + s` 。当然中间的字符可以换成任何一个不在 `p` 和 `s` 中出现的字符。

   对新串 `q` 求 `Z` 函数，那么前半部分的 `Z` 函数和 `p` 求自己的 `Z` 函数的过程是一样的。

   而后半部分，求出的 `Z` 函数值肯定不会超过 `len(p)` ，因为中间有个 `#` 字符。而每次 `Z` 函数值等于 `len(p)` 就等价于在 `s` 里找出了一个子串等于 `p` 。

   可以看出，模式串求 `Z` 函数，和在文本串里找模式串，其实是同样的过程。唯一的区别在于，在模式串里求 `Z` 函数，需要保存结果到数组里；在文本串里求 `Z` 函数，不需要保存结果，只需要随时关注此时函数值是否等于 `len(p)` 。

#### 2.构造 Z 函数

1. 数据类型

   构造参数 `Iterator first` ，表示模式序列区间开始位置。

   构造参数 `Iterator last` ，表示模式序列区间结束位置。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `uint32_t length` ，表示模式序列长度。

   输入参数 `InitMapping mapping` ，表示模式序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示模式序列区间开始位置。

   输入参数 `Iterator last` ，表示模式序列区间结束位置。（开区间）

2. 时间复杂度

   同上。

#### 5.查询当前位置的下一个匹配的位置(adjust)

1. 数据类型

   输入参数 `Ptr &l` ，表示在文本串上一个字符时的 `l` 的值。

   输入参数 `Ptr &r` ，表示在文本串上一个字符时的 `r` 的值。

   输入参数 `Ptr i` ，表示文本串当前字符的下标。

   输入参数 `Ptr length` ，表示文本串长度。

   输入参数 `const Iter &iter` ，表示文本串。

   返回类型 `Ptr` ，表示经过计算后，文本串当前位置的 `Z` 函数值。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   时间复杂度为均摊时间复杂度。

   本方法中的 `l` 和 `r` 为辅助推导 `Z` 函数的变量，初始时均为 `-1`  。

   本方法的用法较难以解释，可以配合示例代码理解。


#### 6.查询是否被某序列包含(contained_by)

1. 数据类型

   输入参数 `Iterator first` ，表示要查找的文本序列的区间开始位置。

   输入参数 `Iterator last` ，表示要查找的文本序列的区间结尾位置。（开区间）

   返回类型 `uint32_t` ，表示查询结果。

2. 时间复杂度

   $O(m)$ ，此处 `m` 指模式序列的长度。

3. 备注

   本方法只返回文本串中第一次出现模式串的下标。

   当找不到时，返回 `-1` 。

#### 7.查询 Z 函数值(query_Z)

1. 数据类型

   输入参数 `uint32_t i` ，表示模式串的下标。

   返回类型 `uint32_t` ，表示模式串相应位置的 `Z` 函数值。

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/ZAlgorithm.h"

int main() {
    //给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::ZAlgorithm_string Z(p);
    for (int i = 0; i < p.size(); i++) {
        int j = Z.query_Z(i);
        if (j > 0) {
            cout << "index " << i << ": " << p << " startwith " << p.substr(i, j) << endl;
        }
    }

    //给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = Z.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl;

    //如果要查找所有出现位置
    for (int l = -1, r = -1, i = 0; i < s.size(); i++) {
        int z = Z.adjust(l, r, i, s.size(), s);
        if (z == p.size()) {
            int index = i;
            cout << index << " " << s.substr(index, p.size()) << endl;
        }
    }

    //借助 Z 函数可以快速求出字符串周期
    std::string q = "abcabcabcabcabcabcabc";
    // q 长度为 21
    int n = q.size();
    OY::ZAlgorithm_string Z2(q);
    for (int i = 1; i < n; i++) {
        if (i + Z2.query_Z(i) == n && n % i == 0) {
            cout << i << " is minimum cycle of string " << q << endl;
            break;
        }
    }
}
```

```
#输出如下
index 4: abcdabceabcd startwith abc
index 8: abcdabceabcd startwith abcd
11 abcdabceabcd
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd
3 is minimum cycle of string abcabcabcabcabcabcabc

```

