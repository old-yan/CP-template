### 一、模板类别

​	序列：可回滚的回文自动机。

​	练习题目：

1. [132. 分割回文串 II](https://leetcode.cn/problems/palindrome-partitioning-ii/)
2. [P3649 [APIO2014] 回文串](https://www.luogu.com.cn/problem/P3649)
3. [P4287 [SHOI2011] 双倍回文](https://www.luogu.com.cn/problem/P4287)
4. [P4555 [国家集训队] 最长双回文串](https://www.luogu.com.cn/problem/P4555)
5. [P4762 [CERC2014] Virus synthesis](https://www.luogu.com.cn/problem/P4762)
6. [P5496 【模板】回文自动机（PAM）](https://www.luogu.com.cn/problem/P5496)
7. [P5555 秩序魔咒](https://www.luogu.com.cn/problem/P5555)
8. [P5685 [JSOI2013] 快乐的 JYY](https://www.luogu.com.cn/problem/P5685)


### 二、模板功能

​	本模板与普通 `PAM` 模板功能一致，仅仅是保证了 `push_back` 和 `pop_back` 不会因最坏情况而超时。

#### 1.构造回文自动机

1. 数据类型

   模板参数 `size_type DepthBitCount` ，表示回文树的最大高度的二进制位长。

   其余同普通 `PAM` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   同普通 `PAM` 。

#### 2.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `const value_type &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(1)$ ，最坏 $O(\log n)$ 。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 3.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ ，最坏 $O(\log n)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 4.查询结点的长度一半以下的失配结点(query_fail_to_half)

1. 数据类型

   输入参数 `size_type node_index` ，表示回文树中结点的索引。

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。
   
#### 5.查询对称性(query)

1. 数据类型

   输入参数 `uint32_t left` ，表示查询的子串的起始下标。

   输入参数 `uint32_t right` ，表示查询的子串的结束下标。（闭区间）

2. 时间复杂度

   $O(\log n)$ 。

#### 6.按照失配顺序对某回文串的每个本质不同的失配串结点调用回调函数(do_for_each_distinct_node)

1. 输入类型

   输入参数 `size_type init_node_index` ，表示初始时的回文树结点。

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   一个回文串的失配串的数量，最多可以达到和原串长度同数量级。但是，其本质不同的失配串最多只有 `log n` 种。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/RollbackPAM.h"

#include <map>

void test_find_longest() {
    cout << "test find longest:\n";

    std::string s = "abacabadabadabacabad";
    cout << "original string:\n ["
         << s << "] \n";
    using PAM = OY::StaticRollbackPAM_string<>;
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
    using PAM = OY::StaticRollbackPAM_string<>;
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
    using PAM = OY::StaticRollbackPAM_string<>;
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
    using PAM = OY::RollbackPAM::Automaton<Node, std::string, 10>;
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

