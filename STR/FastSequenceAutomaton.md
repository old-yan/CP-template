### 一、模板类别

​	序列：序列自动机快速版。

​	练习题目：

1. [P5826 【模板】子序列自动机](https://www.luogu.com.cn/problem/P5826)

### 二、模板功能

   俗称的序列自动机，​进行 $O(n\cdot \Sigma)$  的预处理，然后进行 $O(m)$ 的搜索；可持久化线段树做法，是进行 $O(n\cdot \log\Sigma)$  的预处理，然后进行 $O(m\cdot\log\Sigma)$ 的搜索。
   另一版序列自动机（见 `SequenceAutomaton` ），进行 $O(n)$ 的预处理， $O(m\log n)$ 的搜索。如果 `n` 和 `m` 的数量级大致相当，那么哪种做法都可以。然而，如果 `m` 远大于 `n` ，那么最好还是用 $O(n\cdot \Sigma)$  的预处理加 $O(m)$ 的搜索的写法，就是本文件的写法。

#### 1.构造序列自动机

1. 数据类型

   模板参数 `uint32_t MAX_VALUE` ，表示区间中出现的最大值。

   构造参数 `uint32_t length` ，表示区间长度。

   构造参数 `InitMapping mapping` ，表示区间下标到元素值的映射。

2. 时间复杂度

   $O(n\cdot \Sigma)$ ，此处 `n` 指区间长度； `Σ` 指字符集大小，

3. 备注

   显然，本模板适用于 `Σ` 较小的情况。

#### 2.构造序列自动机

1. 数据类型

   构造参数 `Iterator first` ，表示序列的起始位置。

   构造参数 `Iterator last` ，表示序列的结束位置。（开区间）

   其余同上。

2. 时间复杂度

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示序列的长度。

   输入参数 `InitMapping mapping` ，表示下标到元素的映射。

   其余同上。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示序列的起始位置。

   输入参数 `Iterator last` ，表示序列的结束位置。（开区间）

   输入参数 `Mapping &&map` ，表示对序列中元素的映射。默认映射为自身值。

   其余同上。

2. 时间复杂度

   同上。

#### 5.查询当前位置的下一个特定字符的位置(next)

1. 数据类型

   输入参数 `uint32_t last_pos`，表示之前搜到的位置。

   输入参数 `uint32_t elem` ，表示现在要找的序列元素。

   返回类型 `uint32_t` ，表示当前要找的序列元素所在的位置。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   返回的结果表示在原序列中，下标大于 `last_pos` 的范围内出现的第一个 `elem` 的下标。

   如果找不到，返回 `length` 。

#### 6.查询是否包含某序列(contains)

1. 数据类型

   输入参数 `Iterator first` ，表示要查找的序列的区间开始位置。

   输入参数 `Iterator last` ，表示要查找的序列的区间结尾位置。（开区间）

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(m)$ ，此处 `m` 指查找序列的长度。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/FastSequenceAutomaton.h"

int main() {
    std::string s = "abcfe";
    OY::FastSequenceAutomaton<127> sa(s.begin(), s.end());

    std::string p = "abcfe";
    if (sa.contains(p.begin(), p.end())) {
        cout << s << " has a subsequence :" << p << endl;
    }
    p = "bfe";
    if (sa.contains(p.begin(), p.end())) {
        cout << s << " has a subsequence :" << p << endl;
    }
    p = "bcd";
    if (!sa.contains(p.begin(), p.end())) {
        cout << s << " doesn't have a subsequence :" << p << endl;
    }
}
```

```
#输出如下
abcfe has a subsequence :abcfe
abcfe has a subsequence :bfe
abcfe doesn't have a subsequence :bcd

```

