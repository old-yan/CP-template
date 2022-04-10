### 一、模板类别

​	数学：`Manacher` 算法，谐音“马拉车”算法。

### 二、模板功能

#### 1.构造马拉车

1. 数据类型

   构造参数 `_Iterator __first` ，表示序列开始位置。

   构造参数 `_Iterator __last` ，表示序列结束位置。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   本模板生成两个辅助数组，一个是 `m_oddArm` 数组，用来判断奇数长度子串的对称性；一个是 `m_evenArm` 数组，用来判断偶数长度子串的对称性。`m_oddArm[i]` 表示以下标 `i` 向左右同步扩展，可以扩展多长的回文串，显然这样得到的子串必然长度为奇数；`m_evenArm[i]` 表示以下标 `i-1` 和 `i` 的中点向左右同步扩展，可以扩展多长的回文串，显然这样得到的字串必然长度为偶数。

   例如：`string s="abcbadeed"`， 对这个字符串建立马拉车模板，则 `m_oddArm[2]==2` ， `m_evenArm[7]==2` 。

#### 2.查询对称性

1. 数据类型

   输入参数 `int __left` ，表示查询的子串的起始下标。

   输入参数 `int __right` ，表示查询的子串的结束下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Manacher.h"

int main() {
    std::string s = "abcdaeeabc";
    OY::Manacher mc(s.begin(), s.end());
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

