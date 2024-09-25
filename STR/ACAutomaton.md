### 一、模板类别

​	序列：`AC` 自动机。

​	练习题目：

1. [Keywords Search](https://acm.hdu.edu.cn/showproblem.php?pid=2222)
2. [病毒侵袭](https://acm.hdu.edu.cn/showproblem.php?pid=2896)
3. [病毒侵袭持续中](https://acm.hdu.edu.cn/showproblem.php?pid=3065)
4. [2707. 字符串中的额外字符](https://leetcode.cn/problems/extra-characters-in-a-string/)
5. [3213. 最小代价构造字符串](https://leetcode.cn/problems/construct-string-with-minimum-cost/)
6. [P2292 [HNOI2004] L 语言](https://www.luogu.com.cn/problem/P2292)
7. [P3311 [SDOI2014] 数数](https://www.luogu.com.cn/problem/P3311)
8. [P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
9. [P3808 【模板】AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
10. [P5357 【模板】AC 自动机（二次加强版）](https://www.luogu.com.cn/problem/P5357)
11. [parent 树上启发式合并](https://ac.nowcoder.com/acm/problem/274852)




### 二、模板功能

​	 `AC` 自动机的结点，只能使用数组来维护所有的孩子，而不适合用哈希表或者平衡树。

#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   模板参数 `typename Node` ，表示 `AC` 自动机的结点基类。默认为 `AC::BaseNode` ，为空类。
   
   模板参数 `size_type ChildCount` ，表示 `AC` 自动机的每个结点里有多少个孩子结点。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   如果从每个结点向其失配结点连接无向边，则自动机里的结点会呈现为一棵树，总的根结点为结点 `0` 。 

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

#### 5.清空(clear)

1. 输入类型

2. 时间复杂度

   $O(1)$ 。

#### 6.查询是否为空(empty)

1. 输入类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

#### 7.预备(prepare)

1. 输入类型

2. 时间复杂度

    $O(n)$ 。

#### 8.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本 `AC` 自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 9.获取失配后的索引(query_fail)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `size_type` ，表示发生失配后的索引。

2. 时间复杂度

    $O(1)$ 。

#### 10.获取失配后的结点指针(get_fail_node)

1. 输入类型

   输入参数 `size_type index` ，表示结点的索引。

   返回类型 `node *` ，表示失配后的的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本 `AC` 自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生

#### 11.按照扩展顺序对各个结点调用回调函数(do_for_extending_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

    $O(n)$ ，此处 `n` 表示 `AC` 自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是逆着失配指针的方向对各个结点调用回调函数。
   
   针对每个结点和其失配结点，本方法保证先对失配结点调用回调函数，后对当前结点调用回调函数。

#### 12.按照失配顺序对各个结点调用回调函数(do_for_failing_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

    $O(n)$ ，此处 `n` 表示 `AC` 自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是顺着失配指针的方向对各个结点调用回调函数。
   
   针对每个结点和其失配结点，本方法保证先对当前结点调用回调函数，后对失配结点调用回调函数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

#include <map>

void test_find_index() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";

    // 比如我们想知道匹配到的模式串的下标，那么我们需要额外维护每个结点对应的串的下标
    using AC = OY::ACAM<26>;
    AC ac;
    std::map<int, int> mp;
    for (uint32_t i = 0; i < 8; i++) {
        auto node_index = ac.insert_lower(p[i]);
        mp[node_index] = i;
    }
    ac.prepare();

    uint32_t searched = 0;
    // 观察，发现每次迭代搜到的模式串，都是文本串的已搜部分的一个后缀
    uint32_t pos = 0;
    for (char c : s) {
        searched++;
        pos = ac.next(pos, c - 'a');
        cout << "now we searched " << s.substr(0, searched) << endl;
        for (auto x = pos; x; x = ac.query_fail(x)) {
            if (mp.count(x)) {
                cout << s << " contains " << p[mp[x]] << endl;
            }
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
}

void test_find_count() {
    std::string p[] = {"ab", "abca", "bc", "a", "b", "c", "d", "ca"};
    std::string s = "abcaabcaab";
    using AC = OY::ACAM<26>;
    AC ac;
    // 插入时不动 size 信息，仅仅把每个字符串要找的结点记录下来
    int ins_pos[8];
    for (uint32_t i = 0; i < 8; i++) {
        ins_pos[i] = ac.insert_lower(p[i]);
    }
    ac.prepare();
    // 在匹配过程中，将所有匹配的串的大小加一
    std::vector<int> sz(ac.m_data.size());
    uint32_t pos = 0;
    for (char c : s) {
        pos = ac.next(pos, c - 'a');
        for (auto x = pos; x; x = ac.query_fail(x)) sz[x]++;
    }
    // 输出模式串出现次数
    for (uint32_t i = 0; i < 8; i++) {
        cout << p[i] << " appear " << sz[ins_pos[i]] << " times\n";
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

