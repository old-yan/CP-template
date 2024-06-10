### 一、模板类别

​	序列：序列哈希助手。

​	练习题目：

1. [#103. 子串查找」三个朋友](https://loj.ac/p/103)
2. [#2823. 「BalticOI 2014 Day 1」三个朋友](https://loj.ac/p/2823)
3. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
4. [P2870 [USACO07DEC] Best Cow Line G](https://www.luogu.com.cn/problem/P2870)
5. [P3370 【模板】字符串哈希](https://www.luogu.com.cn/problem/P3370)
6. [P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
7. [P3538 [POI2012] OKR-A Horrible Poem](https://www.luogu.com.cn/problem/P3538)
8. [P3763 [TJOI2017] DNA](https://www.luogu.com.cn/problem/P3763)
9. [P3805 【模板】manacher](https://www.luogu.com.cn/problem/P3805)
10. [P4036 [JSOI2008] 火星人](https://www.luogu.com.cn/problem/P4036)
11. [P4407 [JSOI2009] 电子字典](https://www.luogu.com.cn/problem/P4407)
12. [P4824 [USACO15FEB] Censoring S](https://www.luogu.com.cn/problem/P4824)
13. [P8306 【模板】字典树](https://www.luogu.com.cn/problem/P8306)


### 二、模板功能

​	本模板包含了两个模板类。第一个模板类为 `SeqHash` ，表示对序列进行哈希之后得到的结果，这个结果由序列长度和序列哈希值两部分组成；第二个模板类为 `SeqHashPresumTable` ，表示对序列进行预处理得到的前缀和表。通过前缀和表，可以查询得到任意一个子串的 `SeqHash` 。

#### 1.构造序列哈希结果/序列哈希前缀和表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示长度、下标类型。

   类型设定 `hash_type = SeqHash<Tp>` ，表示哈希结果类型。

   类型设定 `table_type = SeqHashPresumTable<Tp>` ，表示序列哈希前缀和表的类型。

   模板参数 `typename Tp` ，可以为任意一种自取模数。

   构造参数 `Iterator first` ，表示序列的起始位置。

   构造参数 `Iterator last` ，表示序列的结束位置。（开区间）

   构造参数 `Mapping &&map` ，表示对序列中元素的映射。默认映射为自身值。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指序列长度。

3. 备注

   构造参数 `map` 表示对序列中元素的映射。例如，对于一个全部字符为小写字母的字符串，可以将所有的元素映射到 `[0, 26)` 范围内。当然，一般将元素映射为自身即可。 `map` 的返回值需要在 `[0, Base)` 范围内。

   本模板要求 `Tp` 类的模数 `P` 为质数，且尽量大，以减少哈希值冲突。假设要比较的哈希值有 `n` 个，那么出现意外冲突的概率为 $1-e^{-\frac{n\cdot(n-1)}{2\cdot P}}$ ，取 `P = 1000000007`，取 `n = 100000`，则概率为 `99.3%`；取 `n = 10000`，则概率为 `4.9%` 。所以在随机数据情况下，最好让 `n` 远小于 `sqrt(P)` 。推荐使用 `StaticModInt64` 模板内的 `mint4611686018427387847` 。

#### 2.构造序列哈希前缀和表

1. 数据类型

   构造参数 `size_type length` ，表示序列的长度。

   构造参数 `InitMapping mapping` ，表示下标到元素的映射。

   其余同上。

2. 时间复杂度

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示序列的长度。

   输入参数 `InitMapping mapping` ，表示下标到元素的映射。

   其余同上。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示序列的起始位置。

   输入参数 `Iterator last` ，表示序列的结束位置。（开区间）

   输入参数 `Mapping &&map` ，表示对序列中元素的映射。默认映射为自身值。

   其余同上。

2. 时间复杂度

   同上。

#### 5.预留空间(reserve)

1. 数据类型

   输入参数 `size_type length` ，表示预备维护的区间长度。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 6.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

#### 7.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `typename Tp::mod_type elem` ，表示在末尾添加的元素值。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 8.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 9.查询区间哈希结果(query_hash)

1. 数据类型

   输入参数 `size_type left` ，表示查询的开始下标。

   输入参数 `size_type right` ，表示查询的结束下标。（闭区间）

   返回类型 `hash_type` ，表示查询到的哈希结果。

2. 时间复杂度

   $O(1)$ 。

#### 10.查询区间哈希结果(query_value)

1. 数据类型

   输入参数 `size_type left` ，表示查询的开始下标。

   输入参数 `size_type right` ，表示查询的结束下标。（闭区间）

   返回类型 `SubSeqHashValue` ，表示查询到的哈希结果的值部分。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   有时我们在查询区间哈希结果时，已经天然保证了区间长度相等；此时不需要再关注哈希结果的长度信息，而只关注值信息，可以使用本方法。
   
   本方法返回的 `SubSeqHashValue` 类型对象可以转换成 `Tp` 也就是模数类型对象。

#### 11.查询两串的子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `const table_type &t1` ，表示第一个串的表。

   输入参数 `size_type i1` ，表示第一个串的开始位置下标。

   输入参数 `const table_type &t2` ，表示第二个串的表。

   输入参数 `size_type i2` ，表示第二个串的开始位置下标。

   输入参数 `size_type limit` ，表示要求的最长公共前缀的上限。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指 `limit` 大小。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `limit` 为可能的串长。

#### 12.查询两串的子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `const table_type &t1` ，表示第一个串的表。

   输入参数 `size_type i1` ，表示第一个串的开始位置下标。

   输入参数 `const table_type &t2` ，表示第二个串的表。

   输入参数 `size_type i2` ，表示第二个串的开始位置下标。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

#### 13.比较两个串的子串的大小(compare)

1. 数据类型

   输入参数 `const table_type &t1` ，表示第一个串的表。

   输入参数 `size_type l1` ，表示第一个子串的开始位置下标。

   输入参数 `size_type r1` ，表示第一个子串的结束位置下标。

   输入参数 `const table_type &t2` ，表示第二个串的表。

   输入参数 `size_type l2` ，表示第二个子串的开始位置下标。

   输入参数 `size_type r2` ，表示第二个子串的结束位置下标。

   返回类型 `int` 。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。

   **注意：**

   由于在序列哈希前缀和表中并不存有两个串的原始文本，所以当需要比较文本元素时，文本元素的大小比较结果并不一定和哈希比较结果一致，所以本方法的比较结果不一定能反映出字典序的比较结果。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

// 此处使用模数为 998244353 的自取模数
using mint = OY::mint998244353;
using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
using hash_type = table_type::hash_type;

void test_hash_type() {
    cout << "test for hash type:\n";
    std::string s1 = "abcd";
    auto v1 = hash_type(s1);
    cout << s1 << ": len = " << v1.m_len << ", hash = " << v1.m_val << endl;

    std::string s2 = "efg";
    auto v2 = hash_type(s2);
    cout << s2 << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;

    std::string s3 = "abcdefg";
    auto v3 = hash_type(s3);
    cout << s3 << ": len = " << v3.m_len << ", hash = " << v3.m_val << endl;

    // 字符串哈希结果本身是可以进行拼接的
    auto v4 = v1.append_right(v2);
    cout << s1 + s2 << ": len = " << v4.m_len << ", hash = " << v4.m_val << endl;

    auto v5 = v2.append_left(v1);
    cout << s1 + s2 << ": len = " << v5.m_len << ", hash = " << v5.m_val << endl;

    // 字符串哈希结果也可以进行差分
    auto v6 = v3.remove_prefix(v1);
    cout << s3.substr(s1.size()) << ": len = " << v6.m_len << ", hash = " << v6.m_val << endl;

    auto v7 = v3.remove_suffix(v2);
    cout << s3.substr(0, s3.size() - s2.size()) << ": len = " << v7.m_len << ", hash = " << v7.m_val << endl;

    // 字符串哈希结果也可以进行倍增
    auto v8 = v3.repeat_for(3);
    cout << s3 + s3 + s3 << ": len = " << v8.m_len << ", hash = " << v8.m_val << endl;
    cout << endl;
}

void test_hash_table() {
    cout << "test for hash table:\n";
    std::string s = "oxabcdyyefgabcdefg";
    table_type table(s);
    // 查询区间的哈希结果
    auto v = table.query_hash(2, 5);
    cout << s.substr(2, 4) << ": len = " << v.m_len << ", hash = " << v.m_val << endl;

    auto v2 = table.query_hash(8, 10);
    cout << s.substr(8, 3) << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;
    cout << endl;
}

void test_lcp() {
    cout << "test for compare:\n";
    std::string s1 = "xabcy";
    std::string s2 = "pollxabcz";
    table_type table1(s1);
    table_type table2(s2);
    // 求 lcp
    cout << s1.substr(0) << " and " << s2.substr(4) << "'s lcp length = " << table_type::lcp(table1, 0, table2, 4) << endl;
    // 求子串比较结果
    int res = table_type::compare(table1, 1, 4, table2, 5, 8);
    if (res < 0)
        cout << s1.substr(1, 4) << " is smaller than " << s2.substr(5, 4) << endl;
    else if (res > 0)
        cout << s1.substr(1, 4) << " is bigger than " << s2.substr(5, 4) << endl;
    else
        cout << s1.substr(1, 4) << " is equal to " << s2.substr(5, 4) << endl;

    // 但是这个函数不一定能比出字典序大小
    // 假设元素会做对 128 取模的映射
    std::vector<int> arr1{30, -10, 0, 0, 0, 0};
    std::vector<int> arr2{30, 118, 0, 0, 0, 0};
    std::vector<int> arr3{30, 3, 0, 0, 0, 0};
    auto mapping = [](int i) { return i & 127; };
    cout << "arr1:";
    for (auto a : arr1) cout << ' ' << a;
    cout << endl;
    cout << "arr2:";
    for (auto a : arr2) cout << ' ' << a;
    cout << endl;
    cout << "arr3:";
    for (auto a : arr3) cout << ' ' << a;
    cout << endl;
    int res2 = table_type::compare(table_type(arr1, mapping), 0, 5, table_type(arr2, mapping), 0, 5);
    if (res2 < 0)
        cout << "arr1 is smaller than arr2\n";
    else if (res2 > 0)
        cout << "arr1 is bigger than arr2\n";
    else
        cout << "arr1 is equal to arr2\n";
    int res3 = table_type::compare(table_type(arr1, mapping), 0, 5, table_type(arr3, mapping), 0, 5);
    if (res3 < 0)
        cout << "arr1 is smaller than arr3\n";
    else if (res3 > 0)
        cout << "arr1 is bigger than arr3\n";
    else
        cout << "arr1 is equal to arr3\n";

    cout << endl;
}

void test_ex() {
    cout << "test for advanced:\n";
    std::string s = "Hello World";
    auto v = hash_type(s);
    cout << s << ": len = " << v.m_len << ", hash = " << v.m_val << endl;

    // 怎样快速求出 Hello 首字母小写后的哈希值？
    // 移除前缀，再加个新前缀
    auto v2 = v.remove_prefix(std::string("H")).append_left(std::string("h"));
    s[0] = 'h';
    cout << s << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;

    // 怎样快速求出 World 首字母小写后的哈希值？现在可不是在最前面进行修改了
    // 首先需要知道，ASCII 表中，W = 87，w = 119，所以我们希望下标 6 的地方的字符增加 32
    // 我们可以造个下标 6 处为 32，其余地方全是 0 的哈希值
    std::vector<int> inc{0, 0, 0, 0, 0, 0, 32};
    auto v3 = v2 + hash_type(inc);
    s[6] += 32;
    cout << s << ": len = " << v3.m_len << ", hash = " << v3.m_val << endl;

    // 检验一下
    auto v4 = hash_type(s);
    cout << s << ": len = " << v4.m_len << ", hash = " << v4.m_val << endl;

    // 怎么把所有的小写转成大写？
    // 把相应的位都减去 32
    auto v32_5 = hash_type::single(32).repeat_for(5);
    auto v0 = hash_type::zero(1);
    auto v5 = v4 - v32_5.append_right(v0).append_right(v32_5);
    for (auto &c : s) c = std::toupper(c);
    cout << s << ": len = " << v5.m_len << ", hash = " << v5.m_val << endl;
}

int main() {
    // 对于一个模数、一个 base，全局只需要预处理一次
    // 对字符串进行哈希时， base 设为 128 就足够
    // 为了避免被 hack，可以使用 set_random_base 来设置 base
    hash_type::s_info.set_base(128);
    hash_type::s_info.prepare(1000);
    test_hash_type();
    test_hash_table();
    test_lcp();
    test_ex();
}
```

```
#输出如下
test for hash type:
abcd: len = 4, hash = 211349857
efg: len = 3, hash = 1700709
abcdefg: len = 7, hash = 722597612
abcdefg: len = 7, hash = 722597612
abcdefg: len = 7, hash = 722597612
efg: len = 3, hash = 1700709
abcd: len = 4, hash = 211349857
abcdefgabcdefgabcdefg: len = 21, hash = 727137999

test for hash table:
abcd: len = 4, hash = 211349857
efg: len = 3, hash = 1700709

test for compare:
xabcy and xabcz's lcp length = 4
abcy is smaller than abcz
arr1: 30 -10 0 0 0 0
arr2: 30 118 0 0 0 0
arr3: 30 3 0 0 0 0
arr1 is equal to arr2
arr1 is bigger than arr3

test for advanced:
Hello World: len = 11, hash = 468107406
hello World: len = 11, hash = 468107438
hello world: len = 11, hash = 476355061
hello world: len = 11, hash = 476355061
HELLO WORLD: len = 11, hash = 232941766

```

