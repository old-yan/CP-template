### 一、模板类别

​	序列：`最长公共前缀助手-哈希版` 。

### 二、模板功能

#### 1.构造助手

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中的元素类型，默认为 `char` 。

   模板参数 `typename _Hasher` ，表示借以进行序列哈希的类。可以为 `StaticSequencehasher` ，`DynamicSequenceHasher` ，`MontgomerySequenceHasher` 。默认为`MontgomerySequenceHasher` 。

   模板参数 `uint32_t _MAXN` ，表示进行序列哈希的最长长度。默认为 `500000` 。

   构造参数 `_Iterator __first` ，表示序列开始位置。

   构造参数 `_Iterator __last` ，表示序列结束位置。（开区间）

   构造参数 `_Mapping __map` ，表示序列中的元素的映射函数。默认为 `_Hasher` 类的默认映射函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   通过序列哈希类，可以在 $O(1)$ 时间将一个子字符串映射到整数值。进而我们可以在 $O(1)$ 时间判断两个子串是否相等。再加之以二分，就可以求解从两个下标处开始的子串的最长公共前缀长度。

#### 2.查询子串最长公共前缀

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __b` ，表示第二个子串的开始位置下标。

   输入参数 `uint32_t __limit` ，表示要求的最长公共前缀的上限。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `__limit` 为可能的子串长度。

#### 3.查询子串最长公共前缀

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __b` ，表示第二个子串的开始位置下标。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

#### 4.比较子串大小

1. 数据类型

   输入参数 `uint32_t __l1` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __r1` ，表示第一个子串的结束位置下标。

   输入参数 `uint32_t __l2` ，表示第二个子串的开始位置下标。

   输入参数 `uint32_t __r2` ，表示第二个子串的结束位置下标。

   返回类型 `int` 。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/LongestCommonPrefix_hash.h"

int main() {
    //给出主串
    std::string s = "abcabbbabcb";
    //构造助手
    OY::LongestCommonPrefix_hash<char, OY::MontgomerySequenceHasher> lcp(s.begin(), s.end());
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

