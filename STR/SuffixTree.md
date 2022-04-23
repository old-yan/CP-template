### 一、模板类别

​	数学：后缀树。

### 二、模板功能

​	后缀树的构建采用 `Ukkonen` 算法，实现原理较为繁琐。在实现之后，整个结构呈现为一个多叉树的状态；与其他自动机、字典树的区别是，树上每条边不仅可以表示一个字符，也可以表示多个字符。每个结点处的 `m_begin` ，`m_end` 属性都标注了从父结点到本结点，需要历经主串的哪个区间。

​	后缀树建立在 `Trie` 的基础上。 后缀树使用类内静态函数进行初始化、插入主串、查找字符串。

​	后缀树可以转化为后缀数组。

#### 1.初始化

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `SAMDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `2000000` 。 

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   `_PoolSize` 须为字符数的两倍。

   后缀树和后缀自动机一样，是对文本串进行处理，然后利用其来查找模式串。

#### 2.插入主序列

1. 数据类型

   输入类型 `_Iterator __first` ，表示主序列的区间开始位置。

   输入类型 `_Iterator __last` ，表示主序列的区间结尾位置。（开区间）

2. 时间复杂度

   $O(n)$ ，此处 `n` 指主串长度。

#### 3.查找模式序列

1. 数据类型

   输入类型 `_Sequence __sequence` ，表示主序列。

   输入类型 `_Iterator __first` ，表示模式序列的区间开始位置。

   输入类型 `_Iterator __last` ，表示模式序列的区间结尾位置。（开区间）

   返回类型 `_Find_ans` ，包含 `node` 和 `length` 两个属性，表示查找到的结点位置和在这个结点向下走几个字符才够模式串。

2. 时间复杂度

   $O(m)$ ，此处 `m` 表示模式串长度。

#### 4.求后缀数组

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SuffixTree.h"

int main() {
    std::string s = "abcaabcaab";
    using Tree = OY::SuffixTree<>;
    Tree::Init();
    Tree::Insert(s.begin(), s.end());
    Tree::Build();
    std::string p = "ab";
    cout << p << " appear " << Tree::Find(s, p.begin(), p.end()).node->m_size << " times\n";
    p = "abc";
    cout << p << " appear " << Tree::Find(s, p.begin(), p.end()).node->m_size << " times\n";
    p = "a";
    cout << p << " appear " << Tree::Find(s, p.begin(), p.end()).node->m_size << " times\n";
    p = "d";
    cout << p << " appear " << Tree::Find(s, p.begin(), p.end()).node->m_size << " times\n";
}
```

```
#输出如下
ab appear 3 times
abc appear 2 times
a appear 5 times
d appear 0 times

```

