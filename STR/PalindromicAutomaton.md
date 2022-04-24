### 一、模板类别

​	数学：回文自动机。

### 二、模板功能

#### 1.初始化

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `PAMDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `1000000` 。 

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   `_PoolSize` 须至少为字符数。

#### 2.插入序列元素

1. 数据类型

   输入类型 `_Sequence &__sequence` ，表示要插入的序列。

   输入类型 `uint32_t __index` ，表示当前插入元素的下标。

   返回类型 `TrieNode` ，表示插入元素的结点位置。

2. 时间复杂度

   $O(1)$ 。

#### 3.查询下一个回文结点

1. 数据类型

   输入类型 `_Sequence &__sequence` ，表示要插入的序列。

   输入类型 `uint32_t __index` ，表示当前查询元素的下标。

   输入类型 `TrieNode __cur` ，表示当前回文结点。

   返回类型 `TrieNode` ，表示下一个回文结点位置。

2. 时间复杂度

   $O(1)$ ，此处`n` 表示所有模式串长度之和。

3. 备注

   一个回文结点唯一确定一个回文串。“下一个”回文结点，指的是以当前下标为结尾的、比当前回文结点对应长度略短的回文串所在的结点。

   可以结合示例代码理解。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

int main() {
    std::string s = "abacabadabadabacabad";

    using PAM = OY::PalindromicAutomaton<OY::TrieLowerMapping, OY::PAMDefaultInfo, 1000>;
    using TrieNode = PAM::TrieNode;
    // longest[i] 保存以 i 为结尾的最长回文串
    TrieNode longest[s.size()];
    PAM::Init();
    for (uint32_t i = 0; i < s.size(); i++)
        longest[i] = PAM::Insert(s, i);

    // 我们查看以 i 为结尾的最长回文串
    cout << "longest palindromic string endsWith index 19 is: \n"
         << s.substr(19 - longest[19]->m_length + 1, longest[19]->m_length) << endl;

    cout << "longest palindromic string endsWith index 18 is: \n"
         << s.substr(18 - longest[18]->m_length + 1, longest[18]->m_length) << endl;

    cout << "longest palindromic string endsWith index 14 is: \n"
         << s.substr(14 - longest[14]->m_length + 1, longest[14]->m_length) << endl;

    // 如果我们想看以 18 为结尾的所有回文串
    TrieNode cur = longest[18];
    do {
        cout << s.substr(18 - cur->m_length + 1, cur->m_length) << endl;
    } while (cur = cur->m_fail);
}
```

```
#输出如下
longest palindromic string endsWith index 19 is: 
dabacabad
longest palindromic string endsWith index 18 is: 
abacabadabadabacaba
longest palindromic string endsWith index 14 is: 
abadabadaba
abacabadabadabacaba
abacaba
aba
a

```

