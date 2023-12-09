### 一、模板类别

​	序列：回文自动机。

​	练习题目：

1. [P3649 [APIO2014] 回文串](https://www.luogu.com.cn/problem/P3649)
2. [P4287 [SHOI2011] 双倍回文](https://www.luogu.com.cn/problem/P4287)
3. [P4762 [CERC2014] Virus synthesis](https://www.luogu.com.cn/problem/P4762)
4. [P5496 【模板】回文自动机（PAM）](https://www.luogu.com.cn/problem/P5496)
5. [P5555 秩序魔咒](https://www.luogu.com.cn/problem/P5555)
6. [P5685 [JSOI2013] 快乐的 JYY](https://www.luogu.com.cn/problem/P5685)

### 二、模板功能

#### 1.构造回文自动机

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   类型设定 `value_type = typename Sequence::value_type` ，表示序列里的元素类型。

   模板参数 `typename Node` ，表示自动机里的结点类型。

   模板参数 `typename Sequence` ，表示序列类型。

   构造参数 `size_type length` ，表示序列长度。

   构造参数 `InitMapping &&mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本模板的所有奇数长度的串对应的结点的根结点的下标为 `1` ，其串长设为 `-1` ；所有偶数长度的串对应的结点的根结点的下标为 `2` ，其串长设为 `0` 。非空回文子串对应的结点的下标，从 `3` 开始有意义。

   **注意：**

   因为本回文自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 2.构造回文自动机

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

   输入参数 `size_type length` ，表示序列长度。

   输入参数 `InitMapping &&mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示序列区间开始位置。

   输入参数 `Iterator last` ，表示序列区间结束位置。（开区间）

2. 时间复杂度

   同上。


#### 5.预留空间(reserve)

1. 数据类型

   输入参数 `size_type length` ，表示预备维护的区间长度。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指序列长度。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 6.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 7.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `const value_type &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 8.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 9.查询序列元素对应的回文自动机结点(query_node_index)

1. 数据类型

   输入参数 `size_type i` ，表示序列的下标。

   返回类型 `uint32_t` ，表示序列相应元素对应的回文自动机结点索引。

2. 时间复杂度

   $O(1)$ 。

#### 10.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本回文自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 11.获取失配后的索引(get_fail)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `size_type` ，表示发生失配后的索引。

2. 时间复杂度

    $O(1)$ 。

#### 12.获取失配后的结点指针(get_fail_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示失配后的的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本回文自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生

#### 13.按照失配顺序对各个结点调用回调函数(do_for_failing_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示回文自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是顺着失配指针的方向对各个结点调用回调函数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/PalindromicAutomaton.h"

void test_find_longest() {
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticAutomaton_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });
    cout << "longest palindromic string endsWith index 19 is: \n"
         << s.substr(19 - pam.get_node(pam.query_node_index(19))->m_length + 1, pam.get_node(pam.query_node_index(19))->m_length) << endl;

    cout << "longest palindromic string endsWith index 18 is: \n"
         << s.substr(18 - pam.get_node(pam.query_node_index(18))->m_length + 1, pam.get_node(pam.query_node_index(18))->m_length) << endl;

    cout << "longest palindromic string endsWith index 14 is: \n"
         << s.substr(14 - pam.get_node(pam.query_node_index(14))->m_length + 1, pam.get_node(pam.query_node_index(14))->m_length) << endl;
}

void test_find_all() {
    std::string s = "abacabadabadabacabad";

    using PAM = OY::StaticAutomaton_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto cur = pam.query_node_index(18);
    do {
        cout << s.substr(18 - pam.get_node(cur)->m_length + 1, pam.get_node(cur)->m_length) << endl;
        cur = pam.get_fail(cur);
    } while (cur > 2);
}

int main() {
    test_find_longest();
    test_find_all();
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

