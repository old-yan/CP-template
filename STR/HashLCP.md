### 一、模板类别

​	序列：最长公共前缀助手-哈希版。

​	练习题目：

1. [P1117 [NOI2016] 优秀的拆分](https://www.luogu.com.cn/problem/P1117)
2. [P1368 【模板】最小表示法](https://www.luogu.com.cn/problem/P1368)
3. [P1709 [USACO5.5] 隐藏口令 Hidden Password](https://www.luogu.com.cn/problem/P1709)
4. [P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
5. [P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)


### 二、模板功能

#### 1.构造助手

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示长度、下标类型。

   模板参数 `typename Sequence` ，表示存储原序列的序列类型。

   模板参数 `typename Tp` ，可以为任意一种自取模数。

   构造参数 `Iterator first` ，表示序列的起始位置。

   构造参数 `Iterator last` ，表示序列的结束位置。（开区间）

   构造参数 `Mapping &&map` ，表示对序列中元素的映射。默认映射为自身值。

2. 时间复杂度

   $O(n)$ 。

3. 备注
   
   构造参数 `map` 表示对序列中元素的映射。例如，对于一个全部字符为小写字母的字符串，可以将所有的元素映射到 `[0, 26)` 范围内。当然，一般将元素映射为自身即可。 `map` 的返回值需要在 `[0, Base)` 范围内。
   
   本模板要求 `Tp` 类的模数 `P` 为质数，且尽量大，以减少哈希值冲突。假设要比较的哈希值有 `n` 个，那么出现意外冲突的概率为 $1-e^{-\frac{n\cdot(n-1)}{2\cdot P}}$ ，取 `P = 1000000007`，取 `n = 100000`，则概率为 `99.3%`；取 `n = 10000`，则概率为 `4.9%` 。所以在随机数据情况下，最好让 `n` 远小于 `sqrt(P)` 。推荐使用 `StaticModInt64` 模板内的 `mint4611686018427387847` 。
   
   通过序列哈希类，可以在 $O(1)$ 时间将一个子字符串映射到整数值。进而我们可以在 $O(1)$ 时间判断两个子串是否相等。再加之以二分，就可以求解从两个下标处开始的子串的最长公共前缀长度。

#### 2.查询子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的开始位置下标。

   输入参数 `size_type b` ，表示第二个子串的开始位置下标。

   输入参数 `size_type limit` ，表示要求的最长公共前缀的上限。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指 `limit` 大小。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `limit` 为可能的子串长度。

#### 3.查询子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的开始位置下标。

   输入参数 `size_type b` ，表示第二个子串的开始位置下标。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

#### 4.查询子串最长公共后缀(lcs)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的结尾位置下标。

   输入参数 `size_type b` ，表示第二个子串的结尾位置下标。

   输入参数 `size_type limit` ，表示要求的最长公共后缀的上限。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指 `limit` 大小。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `limit` 为可能的子串长度。

#### 5.查询子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的结尾位置下标。

   输入参数 `size_type b` ，表示第二个子串的结尾位置下标。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

#### 6.比较子串大小(compare)

1. 数据类型

   输入参数 `size_type l1` ，表示第一个子串的开始位置下标。

   输入参数 `size_type r1` ，表示第一个子串的结束位置下标。

   输入参数 `size_type l2` ，表示第二个子串的开始位置下标。

   输入参数 `size_type r2` ，表示第二个子串的结束位置下标。

   返回类型 `int` 。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指序列长度。

3. 备注

   两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/HashLCP.h"
#include "STR/SequenceMultiHash.h"

using mint0 = OY::mint998244353;
using mint1 = OY::mint1000000007;
int main() {
    // 可以只有一组模数，也可以多模数
    // 对字符串进行哈希时， base 设为 128 就足够
    // 为了避免被 hack，可以使用 set_random_base 来设置 base
    // 在使用前要先做预备
    using hash_type = OY::SEQHASH::SeqHash<mint0, mint1>;
    hash_type::s_info.set_base(128, 131);
    hash_type::s_info.prepare(1000);

    // 给出主串
    std::string s = "abcabbbabcb";
    // 构造助手
    auto S = OY::make_hash_LCP<mint0, mint1>(s);
    // 查询 lcp
    cout << s.substr(0) << " and " << s.substr(7) << " 's lcp = " << S.lcp(0, 7) << endl;
    // 查询带上限的 lcp
    cout << s.substr(0, 2) << " and " << s.substr(7, 2) << " 's lcp = " << S.lcp(0, 7, 2) << endl;
    // 比较子串
    int res = S.compare(0, 3, 7, 10);
    if (res < 0)
        cout << s.substr(0, 4) << " is smaller than " << s.substr(7, 4) << endl;
    else if (res > 0)
        cout << s.substr(0, 4) << " is bigger than " << s.substr(7, 4) << endl;
    else
        cout << s.substr(0, 4) << " is equal to " << s.substr(7, 4) << endl;
}
```

```
#输出如下
abcabbbabcb and abcb 's lcp = 3
ab and ab 's lcp = 2
abca is smaller than abcb

```

