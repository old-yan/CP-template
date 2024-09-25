### 一、模板类别

​	序列：`KMP` 。

​	练习题目：

1. [1397. 找到所有好字符串](https://leetcode.cn/problems/find-all-good-strings/)
2. [P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
3. [P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
4. [P5410 【模板】扩展 KMP/exKMP（Z 函数）](https://www.luogu.com.cn/problem/P5410)

### 二、模板功能

#### 1.构造 KMP 数组

1. 数据类型

   类型设定 `value_type = typename Sequence::value_type` ，表示序列里的元素类型。

   类型设定 `size_type = uint32_t` ，表示索引、大小类型。

   模板参数 `typename Sequence` ，表示序列类型。

   构造参数 `size_type length` ，表示模式序列长度。

   构造参数 `InitMapping mapping` ，表示模式序列下标到元素的映射。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   要使用 `kmp` 模板，必须了解的一个概念就是前缀函数。不妨设 `f(i)` 表示下标 `i` 处的前缀函数值，则 `f(i)` 为满足以下条件的最大值： ` s[0 ~ f(i) - 1] == s[i - f(i) + 1 ~ i] ` ，也就是 `s[0 ~ i]` 的长度为 `f(i)` 的前缀子串和长度为 `f(i)` 的后缀字串相同。

   显然，如果 `f(i) == i + 1`，那么等式恒成立。所以规定 `f(i) <= i` ，也就是这个前缀子串和后缀字串必须为真子串。下标 `0` 处的前缀函数值恒为 `0`。

   怎样利用前缀函数来进行字符串查找呢？不妨设模式串为 `p` ，文本串为 `s` ，我们可以另起一个新字符串 `q = p + '#' + s` 。当然中间的字符可以换成任何一个不在 `p` 和 `s` 中出现的字符。

   对新串 `q` 求前缀函数，那么前半部分的前缀函数和 `p` 求自己的前缀函数的过程是一样的。

   而后半部分，求出的前缀函数值肯定不会超过 `len(p)` ，因为中间有个 `#` 字符。而每次前缀函数值等于 `len(p)` 就等价于在 `s` 里找出了一个子串等于 `p` 。

   可以看出，模式串求前缀函数，和在文本串里找模式串，其实是同样的过程。唯一的区别在于，在模式串里求前缀函数，需要保存结果到数组里；在文本串里求前缀函数，不需要保存结果，只需要随时关注此时函数值是否等于 `len(p)` 。

#### 2.构造 KMP 数组

1. 数据类型

   构造参数 `Iterator first` ，表示模式序列区间开始位置。

   构造参数 `Iterator last` ，表示模式序列区间结束位置。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示模式序列长度。

   输入参数 `InitMapping mapping` ，表示模式序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示模式序列区间开始位置。

   输入参数 `Iterator last` ，表示模式序列区间结束位置。（开区间）

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

   输入参数 `value_type elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 8.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 9.查询模板内元素数量(size)

1. 数据类型

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

#### 10.查询前缀函数值的失配转移值(jump)

1. 数据类型
   
   输入参数 `size_type last_pi`，表示上一个元素匹配到的前缀函数值。

   输入参数 `const value_type &elem` ，表示现在要找的序列元素。

   返回类型 `size_type` ，表示前缀函数失配后的转移值。

2. 时间复杂度

   均摊 $O(1)$ 。
   
3. 备注

    `jump` 方法本质上就是对 `query_Pi` 方法的反复调用，直到找到可以接 `elem` 元素的 `Pi` 值。
    
    如果跳到 `0` ，则无论是否可以接 `elem` 元素，均会返回。

#### 11.查询是否被某序列包含(contained_by)

1. 数据类型

   输入参数 `Iterator first` ，表示要查找的文本序列的区间开始位置。

   输入参数 `Iterator last` ，表示要查找的文本序列的区间结尾位置。（开区间）

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(m)$ ，此处 `m` 指模式序列的长度。
   
3. 备注

   本方法只返回文本串中第一次出现模式串的下标。

   当找不到时，返回 `-1` 。

#### 12.对某个位置的所有 border 调用回调(do_for_each_border)

1. 数据类型

   输入参数 `size_type init_border` ，表示初始 `border` 值。

   输入参数 `Callback &&call` ，表示对 `border` 调用的回调。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   一个位置的 `border` 的数量，最多可以达到和这个位置的下标同数量级。

#### 13.查询前缀函数值(query_Pi)

1. 数据类型

   输入参数 `size_type i` ，表示模式串的下标。

   返回类型 `size_type` ，表示模式串相应位置的前缀函数值。

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/KMP.h"

void test_find_longest_border() {
    cout << "test find border:\n";
    // 给出模式串并进行预处理
    std::string p = "aabaabaabaa";
    OY::KMP_string kmp(p);
    // 找出每个前缀子串的最长 border，或者说每个位置的前缀函数
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith " << p.substr(0, j) << endl;
        }
    }
    cout << endl;
}

void test_find_all_borders() {
    cout << "test find all borders:\n";
    // 给出模式串并进行预处理
    std::string p = "aabaabaabaa";
    OY::KMP_string kmp(p);
    // 找出每个前缀子串的所有 border
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith ";
            //  << p.substr(0, j) << endl;
            kmp.do_for_each_border(j, [&](int pi) {
                if (pi != j) cout << "/";
                cout << p.substr(0, pi);
            });
            cout << endl;
        }
    }
    cout << endl;
}

void test_find_occurrence() {
    cout << "test find occurrence:\n";
    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::KMP_string kmp(p);
    // 给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = kmp.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl
         << endl;
}

void test_find_all_occurrences() {
    cout << "test find all occurrences:\n";
    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::KMP_string kmp(p);

    // 给出文本串并进行查找所有出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";

    // 如果要查找所有出现位置
    int cur_pi = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        cur_pi = kmp.jump(cur_pi, c);
        if (p[cur_pi] == c) cur_pi++;
        if (cur_pi == kmp.size()) {
            int index = i - cur_pi + 1;
            cout << index << " " << s.substr(index, p.size()) << endl;
        }
    }
    cout << endl;
}

void test_find_cycle() {
    cout << "test find cycle of string:\n";
    // 借助前缀函数可以快速求出字符串周期
    std::string p = "abcabcabcabcabcabcabc";
    // p 长度为 21
    int n = p.size();
    OY::KMP_string kmp(p);
    auto val = kmp.query_Pi(p.size() - 1);
    if (n % (n - val) == 0) {
        cout << n - val << " is minimum cycle of string " << p << endl;
    }
    cout << endl;
}

int main() {
    test_find_longest_border();
    test_find_all_borders();
    test_find_occurrence();
    test_find_all_occurrences();
    test_find_cycle();
}
```

```
#输出如下
test find border:
index 1: aa startwith and endwith a
index 3: aaba startwith and endwith a
index 4: aabaa startwith and endwith aa
index 5: aabaab startwith and endwith aab
index 6: aabaaba startwith and endwith aaba
index 7: aabaabaa startwith and endwith aabaa
index 8: aabaabaab startwith and endwith aabaab
index 9: aabaabaaba startwith and endwith aabaaba
index 10: aabaabaabaa startwith and endwith aabaabaa

test find all borders:
index 1: aa startwith and endwith a
index 3: aaba startwith and endwith a
index 4: aabaa startwith and endwith aa/a
index 5: aabaab startwith and endwith aab
index 6: aabaaba startwith and endwith aaba/a
index 7: aabaabaa startwith and endwith aabaa/aa/a
index 8: aabaabaab startwith and endwith aabaab/aab
index 9: aabaabaaba startwith and endwith aabaaba/aaba/a
index 10: aabaabaabaa startwith and endwith aabaabaa/aabaa/aa/a

test find occurrence:
11 abcdabceabcd

test find all occurrences:
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd

test find cycle of string:
3 is minimum cycle of string abcabcabcabcabcabcabc

```

