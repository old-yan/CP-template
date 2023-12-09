### 一、模板类别

​	序列：`AC` 自动机。

​	练习题目：

1. [P2292 [HNOI2004] L 语言](https://www.luogu.com.cn/problem/P2292)
2. [P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
3. [P3808 【模板】AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
4. [P5357 【模板】AC 自动机（二次加强版）](https://www.luogu.com.cn/problem/P5357)

### 二、模板功能

#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   类型设定 `node = NodeWrap<node>` ，表示自动机里的结点类型。

   模板参数 `template <typename> typename NodeWrap` ，表示 `AC` 自动机中的结点结构体模板类，需传递一个 `CRTP` 基类。

   模板参数 `size_type ChildCount` ，表示 `AC` 自动机的每个结点里有多少个孩子结点。

2. 时间复杂度

   $O(1)$ 。

#### 2.预留空间(reserve)

1. 数据类型

   输入参数 `size_type cap` ，表示要预留的空间大小。

2. 时间复杂度

    $O(n)$ 。

#### 3.插入模式序列(insert)

1. 数据类型

   输入参数 `Iterator first` ，表示模式序列的区间开始位置。

   输入参数 `Iterator last` ，表示模式序列的区间结尾位置。（开区间）

   输入参数 `Mapping &&map` ，表示序列中元素到孩子索引的映射。

   返回类型 `size_type` ，表示插入元素的结点位置。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指模式串长度。
   
3. 备注

    `Mapping &&map` 参数的存在，是为了可能的空间节省。例如，如果插入的字符串全部都是小写字母，就可以映射到 `[0, 26)` 区间；如果插入的字符串全部都是数字字符，就可以映射到 `[0, 10)` 区间

#### 4.插入小写字母序列(insert_lower)

1. 输入类型

   输入参数 `const std::string &s` ，表示即将被插入自动机里的字符串。

   返回类型 `size_type` ，表示插入元素的结点位置。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   显然，插入小写字母组成的字符串是最常见的问题类型。在这种情况下，可以将元素映射到 `[0, 26)` 区间。

#### 5.预备(prepare)

1. 输入类型

2. 时间复杂度

    $O(n)$ 。

#### 6.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本 `AC` 自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 7.获取失配后的索引(get_fail)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `size_type` ，表示发生失配后的索引。

2. 时间复杂度

    $O(1)$ 。

#### 8.获取失配后的结点指针(get_fail_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示失配后的的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本 `AC` 自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生

#### 9.按照扩展顺序对各个结点调用回调函数(do_for_extending_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

    $O(n)$ ，此处 `n` 表示 `AC` 自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是逆着失配指针的方向对各个结点调用回调函数。

#### 10.按照失配顺序对各个结点调用回调函数(do_for_failing_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

    $O(n)$ ，此处 `n` 表示 `AC` 自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是顺着失配指针的方向对各个结点调用回调函数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

template <typename Node>
struct NodeWrap_index {
    uint32_t m_index = -1;
};
void test_find_index() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";

    // 比如我们想知道匹配到的模式串的下标，那么我们可以写一个维护下标的结构体
    using AC = OY::AC::Automaton<NodeWrap_index, 26>;
    AC ac;
    for (uint32_t i = 0; i < 8; i++) {
        auto index = ac.insert_lower(p[i]);
        ac.get_node(index)->m_index = i;
    }
    ac.prepare();

    uint32_t searched = 0;
    // 观察，发现每次迭代搜到的模式串，都是文本串的已搜部分的一个后缀
    uint32_t pos = 0;
    for (char c : s) {
        searched++;
        pos = ac.next(pos, c - 'a');
        cout << "now we searched " << s.substr(0, searched) << endl;
        for (auto x = pos; x; x = ac.get_fail(x)) {
            if (ac.get_node(x)->m_index != -1) {
                cout << s << " contains " << p[ac.get_node(x)->m_index] << endl;
            }
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
}

template <typename Node>
struct NodeWrap_cnt {
    uint32_t m_size = 0;
};
void test_find_count() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";
    using AC = OY::AC::Automaton<NodeWrap_cnt, 26>;
    AC ac;
    // 插入时不动 size 信息，仅仅把每个字符串要找的结点记录下来
    int ins_pos[8];
    for (uint32_t i = 0; i < 8; i++) {
        ins_pos[i] = ac.insert_lower(p[i]);
    }
    ac.prepare();
    // 在匹配过程中，将所有匹配的串的大小加一
    uint32_t pos = 0;
    for (char c : s) {
        pos = ac.next(pos, c - 'a');
        for (auto x = pos; x; x = ac.get_fail(x)) ac.get_node(x)->m_size++;
    }
    // 输出模式串出现次数
    for (uint32_t i = 0; i < 8; i++) {
        cout << p[i] << " appear " << ac.get_node(ins_pos[i])->m_size << " times\n";
    }
}
int main() {
    test_find_index();
    test_find_count();
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
abcaabcaab contains ca
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
abcaabcaab contains ca
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
ca appear 2 times

```

