### 一、模板类别

​	序列：`SAM` （后缀自动机）。

### 二、模板功能

​	`SAM` 实现原理较为繁琐。在实现之后，整个结构呈现为一个有向无环图的状态，从根节点到尾结点有 `n` 条（可能相交的）路径，分别代表了 `n` 条后缀子串。

​	`SAM` 建立在 `Trie` 的基础上。 `SAM` 使用类内静态函数进行初始化、插入字符、构建、查找字符串。

​	`SAM` 可以转化为后缀数组。

#### 1.初始化

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `SAMDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `2000000` 。 

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   `_PoolSize` 须为字符数的两倍。

#### 2.插入序列元素

1. 数据类型

   输入参数 `_Tp __c` ，表示序列中的元素。对于字符串问题，既为字符。

   返回类型 `TrieNode` ，表示插入元素后 `SAM` 的尾结点。

2. 时间复杂度

   $O(\Sigma)$ ，即字典范围。

3. 备注

   插入顺序为，从前往后逐个插入字符。

#### 3.构造自动机

1. 数据类型

2. 时间复杂度

   $O(n)$ ，此处`n` 表示文本串长度。

#### 4.查找模式序列

1. 数据类型

   输入类型 `_Iterator __first` ，表示模式序列的区间开始位置。

   输入类型 `_Iterator __last` ，表示模式序列的区间结尾位置。（开区间）

   返回类型 `TrieNode` ，表示查找到的结点位置。

2. 时间复杂度

   $O(m)$ ，此处 `m` 表示模式串长度。

#### 5.求后缀数组

1. 数据类型

2. 时间复杂度

   $O(\Sigma \cdot n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SuffixAutomaton.h"

int main() {
    std::string s = "abcaabcaab";
    using SAM = OY::SuffixAutomaton<>;
    SAM::Init();
    for (char c : s) SAM::Insert(c);
    SAM::Build();
    std::string p = "ab";
    cout << p << " appear " << SAM::Find(p.begin(), p.end())->m_size << " times\n";
    p = "abc";
    cout << p << " appear " << SAM::Find(p.begin(), p.end())->m_size << " times\n";
    p = "a";
    cout << p << " appear " << SAM::Find(p.begin(), p.end())->m_size << " times\n";
    p = "d";
    cout << p << " appear " << SAM::Find(p.begin(), p.end())->m_size << " times\n";
}
```

```
#输出如下
ab appear 3 times
abc appear 2 times
a appear 5 times
d appear 0 times

```

