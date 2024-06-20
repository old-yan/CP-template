### 一、模板类别

​	序列：`Manacher` 算法，谐音“马拉车”算法。

​	练习题目：

1. [Best Reward](https://acm.hdu.edu.cn/showproblem.php?pid=3613)
2. [Hotaru's problem](https://acm.hdu.edu.cn/showproblem.php?pid=5371)
3. [P3805 【模板】manacher](https://www.luogu.com.cn/problem/P3805)
4. [P4287 [SHOI2011] 双倍回文](https://www.luogu.com.cn/problem/P4287)
5. [P4555 [国家集训队] 最长双回文串](https://www.luogu.com.cn/problem/P4555)
6. [Enumerate Palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes)(https://github.com/yosupo06/library-checker-problems/issues/574)




### 二、模板功能

#### 1.构造马拉车

1. 数据类型

   模板参数 `Even` ，表示是否要求出偶数长度的回文子串。
   
   模板参数 `Odd` ，表示是否要求出奇数长度的回文子串。
   
   构造参数 `uint32_t length` ，表示序列长度。

   构造参数 `InitMapping mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   本模板生成两个辅助数组，一个是 `m_odd_arm` 数组，用来判断奇数长度子串的对称性；一个是 `m_even_arm` 数组，用来判断偶数长度子串的对称性。`m_odd_arm[i]` 表示以下标 `i` 向左右同步扩展，可以扩展多长的回文串，显然这样得到的子串必然长度为奇数；`m_even_arm[i]` 表示以下标 `i-1` 和 `i` 的中点向左右同步扩展，可以扩展多长的回文串，显然这样得到的字串必然长度为偶数。

   例如：`string s = "abcbadeed"`， 对这个字符串建立马拉车模板，则 `m_odd_arm[2] == 2` ， `m_even_arm[7] == 2` 。

#### 2.构造 Z 函数

1. 数据类型

   构造参数 `Iterator first` ，表示序列区间开始位置。

   构造参数 `Iterator last` ，表示序列区间结束位置。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `uint32_t length` ，表示序列长度。

   输入参数 `InitMapping mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示序列区间开始位置。

   输入参数 `Iterator last` ，表示序列区间结束位置。（开区间）

2. 时间复杂度

   同上。

#### 5.查询最大对称奇长度子串半径(query_max_odd_arm)

1. 数据类型

   输入参数 `uint32_t center` ，表示查询的子串的中心下标。

   返回类型 `uint32_t` ，表示从指定中心进行扩展，所能获得的最长对称子串的半径。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   若返回值为 `x` ，则从 `center` 扩展所得的最长对称子串长度为 `x * 2 + 1` 。
   
#### 6.查询最大对称偶长度子串半径(query_max_even_arm)

1. 数据类型

   输入参数 `uint32_t center_l` ，表示查询的子串的左中心下标。
   
   输入参数 `uint32_t center_r` ，表示查询的子串的右中心下标。

   返回类型 `uint32_t` ，表示从指定中心进行扩展，所能获得的最长对称子串的半径。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   显然，偶长度子串的中心恰好落在两个下标之间，所以需要传递两个中心下标。这两个中心下标恰好差为一。
   
   若返回值为 `x` ，则从 `center_l` 和 `center_r` 扩展所得的最长对称子串长度为 `x * 2` 。
   
#### 7.查询对称性(query)

1. 数据类型

   输入参数 `uint32_t left` ，表示查询的子串的起始下标。

   输入参数 `uint32_t right` ，表示查询的子串的结束下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Manacher.h"

int main() {
    std::string s = "abcdaeeabc";
    OY::Manacher<true, true> mc(s);
    for (int l = 0; l < s.size(); l++) {
        for (int r = l; r < s.size(); r++) {
            cout << s.substr(l, r - l + 1) << " is " << (mc.query(l, r) ? "palindromic\n" : "not palindromic\n");
        }
    }
}
```

```
#输出如下
a is palindromic
ab is not palindromic
abc is not palindromic
abcd is not palindromic
abcda is not palindromic
abcdae is not palindromic
abcdaee is not palindromic
abcdaeea is not palindromic
abcdaeeab is not palindromic
abcdaeeabc is not palindromic
b is palindromic
bc is not palindromic
bcd is not palindromic
bcda is not palindromic
bcdae is not palindromic
bcdaee is not palindromic
bcdaeea is not palindromic
bcdaeeab is not palindromic
bcdaeeabc is not palindromic
c is palindromic
cd is not palindromic
cda is not palindromic
cdae is not palindromic
cdaee is not palindromic
cdaeea is not palindromic
cdaeeab is not palindromic
cdaeeabc is not palindromic
d is palindromic
da is not palindromic
dae is not palindromic
daee is not palindromic
daeea is not palindromic
daeeab is not palindromic
daeeabc is not palindromic
a is palindromic
ae is not palindromic
aee is not palindromic
aeea is palindromic
aeeab is not palindromic
aeeabc is not palindromic
e is palindromic
ee is palindromic
eea is not palindromic
eeab is not palindromic
eeabc is not palindromic
e is palindromic
ea is not palindromic
eab is not palindromic
eabc is not palindromic
a is palindromic
ab is not palindromic
abc is not palindromic
b is palindromic
bc is not palindromic
c is palindromic

```

