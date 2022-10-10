### 一、模板类别

​	序列：`最长公共前缀助手-后缀自动机版` 。

### 二、模板功能

#### 1.构造助手

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中元素的类型。默认为 `char` 。

   模板参数 `uint32_t _MAXN` ，表示序列的最大长度，默认为 `1000000` 。

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Solver` ，表示计算最近公共祖先的类。默认为 `HeavyLightDecompositionLCA` 。

   构造参数 `_Iterator __first` ，表示序列开始位置。

   构造参数 `_Iterator __last` ，表示序列结束位置。（开区间）

2. 时间复杂度

   理论时间复杂度为 $O(\Sigma\cdot n)$ ，也就是和区间长度、字符集大小呈正比。

   实际实现的时间复杂度为 $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   根据后缀自动机的实现原理，从后往前逐字符插入字符串后，后缀链接呈现一个类似于字典树的网络状态；而两个字符对应的结点，其最近公共祖先结点在后缀链接网络里的深度，即为这两个字符所带领的子串的最长公共前缀长度。
   
   **注意：**由于后缀自动机的模板为静态大小且不复用空间，所以模板参数 `_MAXN`  必须达到程序运行期间的序列长度之和。

#### 2.查询子串最长公共前缀

1. 数据类型

   输入参数 `uint32_t __a` ，表示第一个子串的开始位置下标。

   输入参数 `uint32_t __b` ，表示第二个子串的开始位置下标。

   输入参数 `uint32_t __limit` ，表示要求的最长公共前缀的上限。。

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
#include "STR/LongestCommonPrefix_sam.h"

int main() {
    //给出主串
    std::string s = "abcabbbabcb";
    //构造助手
    OY::LongestCommonPrefix_sam<char, 1000> lcp(s.begin(), s.end());
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

