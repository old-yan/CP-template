### 一、模板类别

​	序列：`AC` 自动机。

### 二、模板功能

#### 1.初始化

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `SAMDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `1000000` 。 

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   `_PoolSize` 须至少为字符数。

#### 2.插入模式序列

1. 数据类型

   输入类型 `_Iterator __first` ，表示模式序列的区间开始位置。

   输入类型 `_Iterator __last` ，表示模式序列的区间结尾位置。（开区间）

   返回类型 `TrieNode` ，表示插入元素的结点位置。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指模式串长度。

#### 3.构造自动机

1. 数据类型

2. 时间复杂度

   $O(n)$ ，此处`n` 表示所有模式串长度之和。

#### 4.匹配文本序列

1. 数据类型

   输入类型 `_Iterator __first` ，表示文本序列的区间开始位置。

   输入类型 `_Iterator __last` ，表示文本序列的区间结尾位置。（开区间）

   返回类型 `_Iterable` ，表示一个可迭代对象，按照文本串搜索长度递增的顺序，逐个返回最长匹配结点。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示文本串长度。
   
3. 备注

   匹配过程中，逐步将文本串的每个元素考虑进去，进而搜索出所有匹配的模式串。

   每次返回的结点所代表的模式串，一定是文本串当前搜索部分的最长后缀子串。

   通过 `fail` 指针，可以将文本串当前搜索部分的所有匹配后缀都搜出来。

   具体可以通过代码示例第一部分来理解。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

int main() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d"};
    std::string s = "abcaabcaab";

    //比如我们想知道匹配到的模式串的下标，那么我们可以写一个维护下标的结构体
    struct IndexInfo {
        uint32_t m_index = -1;
    };
    using AC = OY::ACAutomaton<OY::TrieLowerMapping, IndexInfo>;
    AC::Init();
    for (uint32_t i = 0; i < 7; i++) {
        auto it = AC::Insert(p[i].begin(), p[i].end());
        it->m_index = i;
    }
    AC::Build();

    uint32_t searched = 0;
    //观察，发现每次迭代搜到的模式串，都是文本串的已搜部分的一个后缀
    for (auto node : AC::Iteration(s.begin(), s.end())) {
        searched++;
        cout << "now we searched " << s.substr(0, searched) << endl;
        for (auto q = node; q; q = q->m_fail)
            if (q->m_index != -1) cout << s << " contains " << p[q->m_index] << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }


    //****************************************************************
    //如果想知道每个模式串的出现次数，就没有必要维护下标信息，而要维护大小信息
    struct SizeInfo {
        uint32_t m_size = 0;
    };
    using AC2 = OY::ACAutomaton<OY::TrieLowerMapping, SizeInfo>;
    AC2::Init();
    AC2::TrieNode node[7];
    //插入时不动 size 信息，仅仅把每个字符串要找的结点记录下来
    for (uint32_t i = 0; i < 7; i++) {
        node[i] = AC2::Insert(p[i].begin(), p[i].end());
    }
    AC2::Build();
    //在匹配过程中，将所有匹配的串的大小加一
    for (auto node : AC2::Iteration(s.begin(), s.end()))
        for (auto q = node; q; q = q->m_fail) q->m_size++;
    //输出模式串出现次数
    for (uint32_t i = 0; i < 7; i++) {
        cout << p[i] << " appear " << node[i]->m_size << " times\n";
    }
}
```

```
#输出如下
now we searched a
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched ab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abc
abcaabcaab contains bc
abcaabcaab contains c
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abca
abcaabcaab contains abca
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaa
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabc
abcaabcaab contains bc
abcaabcaab contains c
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabca
abcaabcaab contains abca
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabcaa
abcaabcaab contains a
~~~~~~~~~~~~~~~~~~~~~~~~~
now we searched abcaabcaab
abcaabcaab contains ab
abcaabcaab contains b
~~~~~~~~~~~~~~~~~~~~~~~~~
ab appear 3 times
abca appear 2 times
bc appear 2 times
a appear 5 times
b appear 3 times
c appear 2 times
d appear 0 times

```

