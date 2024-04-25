### 一、模板类别

​	序列：序列自动机快速版。

​	练习题目：

1. [940. 不同的子序列 II](https://leetcode.cn/problems/distinct-subsequences-ii)
2. [2350. 不可能得到的最短骰子序列](https://leetcode.cn/problems/shortest-impossible-sequence-of-rolls)
3. [2565. 最少得分子序列](https://leetcode.cn/problems/subsequence-with-the-minimum-score)
4. [P3500 [POI2010] TES-Intelligence Test](https://www.luogu.com.cn/problem/P3500)
5. [P5826 【模板】子序列自动机](https://www.luogu.com.cn/problem/P5826)

### 二、模板功能

   俗称的序列自动机，进行 $O(n\cdot \Sigma)$  的预处理，然后进行 $O(m)$ 的搜索；可持久化线段树做法，是进行 $O(n\cdot \log\Sigma)$  的预处理，然后进行 $O(m\cdot\log\Sigma)$ 的搜索。然而在实践中，最方便也是最快的做法还是本模板的实现方法： $O(n)$ 的预处理， $O(m\log n)$ 的搜索。姑且盗用”序列自动机“的名称。

#### 1.构造序列自动机

1. 数据类型

   模板参数 `typename IndexContainer` ，表示用来存放各个下标的容器。

   构造参数 `uint32_t length` ，表示区间长度。

   构造参数 `InitMapping mapping` ，表示区间下标到元素值的映射。

2. 时间复杂度

   若不离散化，为 $O(n+\Sigma)$ ，此处 `n` 指区间长度， `Σ` 为字符集大小；若离散化，为 $O(n\cdot\log n)$ ，此处 `n` 指区间长度。

3. 备注

   当字符集不太大时，不需要离散化，可以使用 `SequenceAutomaton_vector` ；当字符集较大时，需要离散化，可以使用 `SequenceAutomaton_map` 。

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

#### 5.查询当前位置的上一个特定字符的位置(prev)

1. 数据类型

   输入参数 `uint32_t last_pos`，表示之前搜到的位置。

   输入参数 `uint32_t elem` ，表示现在要找的序列元素。

   返回类型 `uint32_t` ，表示当前要找的序列元素所在的位置。

2. 时间复杂度

   若不离散化，为 $O(\log n)$ ，此处 `n` 指区间长度；若离散化，为 $O(\log \Sigma + \log n)$ ，此处 `n` 指区间长度，`Σ` 指字符集大小。
   
3. 备注

   返回的结果表示在原序列中，下标小于 `last_pos` 的范围内出现的最后一个 `elem` 的下标。

   如果找不到，返回 `-1` 。


#### 6.查询当前位置的下一个特定字符的位置(next)

1. 数据类型

   输入参数 `uint32_t last_pos`，表示之前搜到的位置。

   输入参数 `uint32_t elem` ，表示现在要找的序列元素。

   返回类型 `uint32_t` ，表示当前要找的序列元素所在的位置。

2. 时间复杂度

   若不离散化，为 $O(\log n)$ ，此处 `n` 指区间长度；若离散化，为 $O(\log \Sigma + \log n)$ ，此处 `n` 指区间长度，`Σ` 指字符集大小。
   
3. 备注

   返回的结果表示在原序列中，下标大于 `last_pos` 的范围内出现的第一个 `elem` 的下标。

   如果找不到，返回 `length` 。

#### 7.查询是否包含某序列(contains)

1. 数据类型

   输入参数 `Iterator first` ，表示要查找的序列的区间开始位置。

   输入参数 `Iterator last` ，表示要查找的序列的区间结尾位置。（开区间）

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   若不离散化，为 $O(m\cdot\log n)$ ，此处 `m` 指查找序列的长度， `n` 指区间长度；若离散化，为 $O(m\cdot(\log \Sigma + \log n))$ ，此处 `m` 指查找序列的长度 `n` 指区间长度，`Σ` 指字符集大小。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SequenceAutomaton.h"

int main() {
    std::string s = "abcfe";
    OY::SequenceAutomaton_vector sa(s.begin(), s.end());

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

