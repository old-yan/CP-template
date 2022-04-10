### 一、模板类别

​	数学：KMP。

### 二、模板功能

#### 1.构造 KMP 数组

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中的元素类型，对于字符串问题，即为 `char`。

   构造参数 `_Iterator __first` ，表示模式序列开始位置。

   构造参数 `_Iterator __last` ，表示模式序列结束位置。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   要使用 `kmp` 模板，必须了解的一个概念就是前缀函数。不妨设 `f(i)` 表示下标 `i` 处的前缀函数值，则 `f(i)` 为满足以下条件的最大值：

   ​	` s[0 ~ f(i)-1] == s[i-f(i)+1 ~ i] ` ，也就是 `s[0 ~ i]` 的长度为 `f(i)` 的前缀子串和长度为 `f(i)` 的后缀字串相同。

   显然，如果 `f(i) == i+1`，那么等式恒成立。所以规定 `f(i) <= i` ，也就是这个前缀子串和后缀字串必须为真子串。下标 `0` 处的前缀函数值恒为 `0`。

   怎样利用前缀函数来进行字符串查找呢？不妨设模式串为 `p` ，文本串为 `s` ，我们可以另起一个新字符串 `q = p + '#' + s` 。当然中间的字符可以换成任何一个不在 `p` 和 `s` 中出现的字符。

   对新串 `q` 求前缀函数，那么前半部分的前缀函数和 `p` 求自己的前缀函数的过程是一样的。

   而后半部分，求出的前缀函数值肯定不会超过 `len(p)` ，因为中间有个 `#` 字符。而每次前缀函数值等于 `len(p)` 就等价于在 `s` 里找出了一个子串等于 `p` 。

   可以看出，模式串求前缀函数，和在文本串里找模式串，其实是同样的过程。唯一的区别在于，在模式串里求前缀函数，需要保存结果到数组里；在文本串里求前缀函数，不需要保存结果，只需要随时关注此时函数值是否等于 `len(p)` 。

#### 2.在文本串中进行搜索

1. 数据类型

   输入参数 `_Iterator __first` ，表示文本序列区间开始位置。

   输入参数 `_Iterator __last` ，表示文本序列区间结束位置。（开区间）

2. 时间复杂度

   $O(n)$ ，此处 `n` 指序列长度。

3. 备注

   本方法只返回文本串中第一次出现模式串的下标。
   
   当找不到时，返回 `-1` 。

#### 3.查询前缀函数值

1. 数据类型

   输入参数 `int __i` ，表示模式串的下标。

   返回类型 `int` ，表示模式串相应位置的前缀函数值。

2. 时间复杂度

   $O(1)$ 。

#### 4.调整光标

1. 数据类型

   输入参数 `int __cursor` ，表示与文本串上一个字符对应的模式串字符下标。

   输入参数 `_Tp __c` ，表示文本串当前元素值。

   返回类型 `int` ，表示经过计算后，文本串当前元素最大能够匹配到的模式串下标。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   时间复杂度为均摊时间复杂度。

   本方法的用法较难以解释，可以配合示例代码理解。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/KMP.h"

int main() {
    //给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::KMP kmp(p.begin(), p.end());
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.queryPi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith " << p.substr(0, j) << endl;
        }
    }

    //给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = kmp.search(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl;

    //如果要查找所有出现位置
    int cursor = -1;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        cursor = kmp.adjust(cursor, c);
        if (cursor == p.size() - 1) {
            int index = i - cursor;
            cout << index << " " << s.substr(index, p.size()) << endl;
        }
    }

    //借助前缀函数可以快速求出字符串周期
    std::string q = "abcabcabcabcabcabcabc";
    // q 长度为 21
    int n = q.size();
    OY::KMP kmp2(q.begin(), q.end());
    auto val = kmp2.queryPi(q.size() - 1);
    if (n % (n - val) == 0) {
        cout << n - val << " is minimum cycle of string " << q << endl;
    }
}
```

```
#输出如下
index 4: abcda startwith and endwith a
index 5: abcdab startwith and endwith ab
index 6: abcdabc startwith and endwith abc
index 8: abcdabcea startwith and endwith a
index 9: abcdabceab startwith and endwith ab
index 10: abcdabceabc startwith and endwith abc
index 11: abcdabceabcd startwith and endwith abcd
11 abcdabceabcd
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd
3 is minimum cycle of string abcabcabcabcabcabcabc

```

