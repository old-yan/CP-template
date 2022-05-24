### 一、模板类别

​	序列：序列自动机快速版。

### 二、模板功能

   俗称的序列自动机，​进行 $O(n\cdot \Sigma)$  的预处理，然后进行 $O(m)$ 的搜索；可持久化线段树做法，是进行 $O(n\cdot \log\Sigma)$  的预处理，然后进行 $O(m\cdot\log\Sigma)$ 的搜索。
   我之前实现的序列自动机（见另一份模板），进行 $O(n)$ 的预处理，$O(m\log n)$ 的搜索。如果 `n` 和 `m` 的数量级大致相当，那么哪种做法都可以。然而，如果 `m` 远大于 `n` ，那么最好还是用 $O(n\cdot \Sigma)$  的预处理加 $O(m)$ 的搜索的写法，就是本文件的写法。

#### 1.插入主序列

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `uint32_t _PoolSize` ，表示序列最长长度。默认为 `1000000` 。 

   构造参数 `_Iterator __first` ，表示主序列的区间开始位置。

   构造参数 `_Iterator __last` ，表示主序列的区间结尾位置。（开区间）

2. 时间复杂度

   $O(n\cdot \Sigma)$ 。

3. 备注

   和其他几个自动机一样，本自动机为静态方法，无需实例即可调用。

#### 2.查询当前位置的下一个特定字符的位置

1. 数据类型

   输入参数 `uint32_t __current`，表示当前所处的位置。

   输入参数 `_Tp __c` ，表示现在要找的序列元素。

2. 时间复杂度

   $O(1)$ 。

#### 3.查询是否含有某序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要查找的序列的区间开始位置。

   输入参数 `_Iterator __last` ，表示要查找的序列的区间结尾位置。（开区间）

2. 时间复杂度

   $O(m)$ ，此处 `m` 指查找序列的长度。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/FastSequenceAutomaton.h"

int main() {
    std::string S = "abcfe";
    using FAM = OY::FastSequenceAutomaton<>;
    FAM::Insert(S.begin(), S.end());

    std::string p = "abcfe";
    if (FAM::Has(p.begin(), p.end())) {
        cout << S << " has a subsequence :" << p << endl;
    }
    p = "bfe";
    if (FAM::Has(p.begin(), p.end())) {
        cout << S << " has a subsequence :" << p << endl;
    }
    p = "bcd";
    if (!FAM::Has(p.begin(), p.end())) {
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

