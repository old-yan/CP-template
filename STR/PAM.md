### 一、模板类别

​	序列：回文自动机。

​	练习题目：

1. [Best Reward](https://acm.hdu.edu.cn/showproblem.php?pid=3613)
2. [132. 分割回文串 II](https://leetcode.cn/problems/palindrome-partitioning-ii/)
3. [P3649 [APIO2014] 回文串](https://www.luogu.com.cn/problem/P3649)
4. [P4287 [SHOI2011] 双倍回文](https://www.luogu.com.cn/problem/P4287)
5. [P4555 [国家集训队] 最长双回文串](https://www.luogu.com.cn/problem/P4555)
6. [P4762 [CERC2014] Virus synthesis](https://www.luogu.com.cn/problem/P4762)
7. [P5496 【模板】回文自动机（PAM）](https://www.luogu.com.cn/problem/P5496)
8. [P5555 秩序魔咒](https://www.luogu.com.cn/problem/P5555)
9. [P5685 [JSOI2013] 快乐的 JYY](https://www.luogu.com.cn/problem/P5685)

### 二、模板功能

​	回文自动机的结点，可以使用数组来维护所有的孩子，也可以使用哈希表、平衡树等数据结构来维护所有的孩子。下文中，时间复杂度均以前者为准；当使用者使用后者情况的结点时，时间复杂度会有相应的变化。

#### 1.构造回文自动机

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   类型设定 `value_type = typename Sequence::value_type` ，表示序列里的元素类型。

   类型设定 `typename node` ，表示自动机的结点类型。

   类型设定 `typename series` ，表示一个等差数列。含有最长长度、最短长度、公差三个成员变量。

   模板参数 `typename ChildGetter` ，表示自动机里的结点寻找孩子结点时的方法类。

   模板参数 `typename Sequence` ，表示序列类型。

   构造参数 `size_type length` ，表示序列长度。

   构造参数 `InitMapping mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   $O(n\cdot\Sigma)$ ，此处 `Σ` 表示字符集大小。
   
3. 备注

   对于字符集有限的情况，可以使用 `StaticChildGetter` 封装的结点，通过数组存储孩子结点；对于字符集较大的情况，可以自己手写使用哈希表或者平衡树存储孩子的结点。

   本模板的所有奇数长度的串对应的结点的根结点的下标为 `0` ，其串长设为 `-1` ；所有偶数长度的串对应的结点的根结点的下标为 `1` ，其串长设为 `0` 。非空回文子串对应的结点的下标，从 `2` 开始有意义。

   如果从每个结点向其孩子结点连接无向边，则自动机里的结点会呈现为两个树，第一个树里的结点代表着一个奇数长度的串，根结点为结点 `0` ；第二个树里的起点代表着一个偶数长度的串，根结点为结点 `1` 。

   如果从每个结点向其失配结点连接无向边，则自动机里的结点会呈现为一棵树，总的根结点为结点 `0` 。 
   
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

   输入参数 `InitMapping mapping` ，表示序列下标到元素的映射。

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

   $O(n\cdot\Sigma)$ ，此处 `n` 指序列长度， `Σ` 表示字符集大小。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 6.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 7.查询模板内元素数量(size)

1. 数据类型

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(1)$ 。

#### 8.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `const value_type &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(\Sigma)$ ，此处 `Σ` 表示字符集大小。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 9.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 10.查询序列元素对应的回文自动机结点(query_node_index)

1. 数据类型

   输入参数 `size_type i` ，表示序列的下标。

   返回类型 `size_type` ，表示序列相应元素对应的回文自动机结点索引。

2. 时间复杂度

   $O(1)$ 。

#### 11.获取失配后的索引(query_fail)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `size_type` ，表示发生失配后的索引。

2. 时间复杂度

   $O(1)$ 。
   
#### 12.获取下一个等差数列的索引(query_ancestor)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `size_type` ，表示跳 `fail` 到达的下一个等差数列的索引。

2. 时间复杂度

   $O(1)$ 。

#### 13.查询对称性(query)

1. 数据类型

   输入参数 `uint32_t left` ，表示查询的子串的起始下标。

   输入参数 `uint32_t right` ，表示查询的子串的结束下标。（闭区间）

2. 时间复杂度

   $O(\log n)$ 。

#### 14.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本回文自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 15.获取失配后的结点指针(get_fail_node)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `node *` ，表示失配后的的结点指针。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：**

   因为本回文自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生

#### 16.按照扩展顺序对各个结点调用回调函数(do_for_extending_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示回文自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是逆着失配指针的方向对各个结点调用回调函数。
   
   针对每个结点和其孩子结点，本方法保证先对父结点调用回调函数，后对孩子结点调用回调函数。
   
   针对每个结点和其失配结点，本方法保证先对失配结点调用回调函数，后对当前结点调用回调函数。

#### 17.按照失配顺序对各个结点调用回调函数(do_for_failing_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示回文自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是顺着失配指针的方向对各个结点调用回调函数。
   
   针对每个结点和其孩子结点，本方法保证先对孩子结点调用回调函数，后对父结点调用回调函数。
   
   针对每个结点和其失配结点，本方法保证先对当前结点调用回调函数，后对失配结点调用回调函数。

#### 18.对某下标处的所有等差数列调用回调函数(do_for_each_series)

1. 输入类型

   输入参数 `size_type i` ，表示要找的回文子串的结尾下标。

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   在一个序列内，以某个下标为结尾的所有回文串的长度，最多可以有 $O(n)$ 种；将相邻的公差相等的子串们合并，以一个等差数列表示出来，最多可以有 $O(n\cdot\log n)$ 个等差数列。

   显然，在两个等差数列交界处的长度，同时存在于两个等差数列内。

   尤其需要注意的是，如果以下标 `i` 为结尾的回文串只有一个，即长度为 `1` 的子串，则本函数无法找到任何的等差数列。

#### 19.按照失配顺序对某回文串的每个失配串结点调用回调函数(do_for_each_node)

1. 输入类型

   输入参数 `size_type init_node_index` ，表示初始时的回文树结点。

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   一个回文串的失配串的数量，最多可以达到和原串长度同数量级。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/PAM.h"

#include <map>

void test_find_longest() {
    cout << "test find longest:\n";

    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        int len = pam.get_node(pam.query_node_index(pos))->m_length;
        cout << "longest palindromic string endsWith index " << pos << " is: \n"
             << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
    };

    show_endwith(19);
    show_endwith(18);
    show_endwith(14);
    show_endwith(8);

    cout << endl;
}

void test_find_all() {
    cout << "test find all:\n";
    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        cout << "palindromic substr endsWidth index " << pos << ":\n";
        auto cur = pam.query_node_index(pos);
        do {
            auto len = pam.get_node(cur)->m_length;
            cout << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
            cur = pam.query_fail(cur);
        } while (pam.is_node(cur));
    };
    show_endwith(18);
    show_endwith(14);
    cout << endl;
}

void test_find_serieses() {
    cout << "test find serieses:\n";
    std::string u = "abcba";
    std::string u2 = "bab" + u + u + u;
    std::string s = u2 + u2 + u2 + u2 + "zya";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticPAM_string<>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_series = [&](int pos) {
        cout << "palindromic substr endsWidth index " << pos << ":\n";
        auto print = [&](PAM::series x) {
            int longest = x.m_longest;
            int shortest = x.m_shortest;
            int delta = x.m_delta;
            for (int len = shortest; len <= longest; len += delta) {
                cout << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
            }
        };
        pam.do_for_each_series(pos, print);
    };
    // 可以看到，以下标 71 位置为结尾的对称子串长度，形成了三个 series(等差数列)
    show_series(71);
    cout << endl;
}

struct Node {
    std::map<uint32_t, uint32_t> m_child;
    void add_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    void remove_child(uint32_t index) { m_child.erase(index); }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.find(index);
        return it == m_child.end() ? 0 : it->second;
    }
};
void test_map_node() {
    cout << "test map node:\n";

    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::PAM::Automaton<Node, std::string>;
    PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });

    auto show_endwith = [&](int pos) {
        int len = pam.get_node(pam.query_node_index(pos))->m_length;
        cout << "longest palindromic string endsWith index " << pos << " is: \n"
             << s.substr(0, pos + 1 - len) << " [" << s.substr(pos + 1 - len, len) << "] " << s.substr(pos + 1) << endl;
    };

    show_endwith(19);
    show_endwith(18);
    show_endwith(14);
    show_endwith(8);

    cout << endl;
}

int main() {
    test_find_longest();
    test_find_all();
    test_find_serieses();
    test_map_node();
}
```

```
#输出如下
test find longest:
original string:
 [abacabadabadabacabad] 
longest palindromic string endsWith index 19 is: 
abacabadaba [dabacabad] 
longest palindromic string endsWith index 18 is: 
 [abacabadabadabacaba] d
longest palindromic string endsWith index 14 is: 
abac [abadabadaba] cabad
longest palindromic string endsWith index 8 is: 
abacab [ada] badabacabad

test find all:
original string:
 [abacabadabadabacabad] 
palindromic substr endsWidth index 18:
 [abacabadabadabacaba] d
abacabadabad [abacaba] d
abacabadabadabac [aba] d
abacabadabadabacab [a] d
palindromic substr endsWidth index 14:
abac [abadabadaba] cabad
abacabad [abadaba] cabad
abacabadabad [aba] cabad
abacabadabadab [a] cabad

test find serieses:
original string:
 [bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbazya] 
palindromic substr endsWidth index 71:
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcbabababcbaabcbaabcba] zya
bababcbaabcbaabcbabab [abcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcba] zya
bab [abcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcba [abcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcba [abcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabab [abcbaabcbaabcba] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcb [a] zya
bababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcbaabcbabababcbaabcba [abcba] zya

test map node:
original string:
 [abacabadabadabacabad] 
longest palindromic string endsWith index 19 is: 
abacabadaba [dabacabad] 
longest palindromic string endsWith index 18 is: 
 [abacabadabadabacaba] d
longest palindromic string endsWith index 14 is: 
abac [abadabadaba] cabad
longest palindromic string endsWith index 8 is: 
abacab [ada] badabacabad

```

