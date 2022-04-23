### 一、模板类别

​	数学：序列自动机。

### 二、模板功能

​	序列自动机的普通做法，是进行 $O(n\cdot \Sigma)$  的预处理，然后进行 $O(m)$ 的搜索；可持久化线段树做法，是进行 $O(n\cdot \log\Sigma)$  的预处理，然后进行 $O(m\cdot\log\Sigma)$ 的搜索。然而在实践中，最方便也是最快的做法还是本模板的实现方法：$O(n)$ 的预处理，$O(m\log n)$ 的搜索。姑且盗用”序列自动机“的名称。

#### 1.初始化

1. 数据类型

   构造参数 `_Iterator __first` ，表示主序列的区间开始位置。

   构造参数 `_Iterator __last` ，表示主序列的区间结尾位置。（开区间）

   构造参数 `_uint32_t __alpha` ，表示序列元素的值域为 `[0, __alpha)` 。

2. 时间复杂度

   $O(n)$ 。

#### 2.查询当前位置的下一个特定字符的位置

1. 数据类型

   输入参数 `uint32_t __current`，表示当前所处的位置。

   输入参数 `uint32_t __c` ，表示现在要找的序列元素。

2. 时间复杂度

   $O(\log n)$ ，取决于要找的元素的数量多少。

#### 3.查询是否含有某序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要查找的序列的区间开始位置。

   输入参数 `_Iterator __last` ，表示要查找的序列的区间结尾位置。（开区间）

2. 时间复杂度

   $O(m\cdot\log n)$ ，此处 `m` 指查找序列的长度，`n` 指主序列的长度。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SequenceAutomaton.h"

int main() {
    std::string S = "abcfe";
    OY::SequenceAutomaton SQAM(S.begin(), S.end(), 128);

    std::string p = "abcfe";
    if (SQAM.has(p.begin(), p.end())) {
        cout << S << " has a subsequence :" << p << endl;
    }
    p = "bfe";
    if (SQAM.has(p.begin(), p.end())) {
        cout << S << " has a subsequence :" << p << endl;
    }
    p = "bcd";
    if (!SQAM.has(p.begin(), p.end())) {
        cout << S << " doesn't have a subsequence :" << p << endl;
    }
}
```

```
#输出如下
abcfe has a subsequence :abcfe
abcfe has a subsequence :bfe
abcfe doesn't have a subsequence :bcd

```

