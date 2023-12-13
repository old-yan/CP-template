### 一、模板类别

​	序列：可回滚的 `KMP` 。

​	练习题目：

1. [P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
2. [P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
3. [P5410 【模板】扩展 KMP/exKMP（Z 函数）](https://www.luogu.com.cn/problem/P5410)

### 二、模板功能

​	本模板与普通 `kmp` 模板功能一致，仅仅是保证了 `push_back` 和 `pop_back` 不会因最坏情况而超时。

#### 1.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `const value_type &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(1)$ ，最坏 $O(\log n)$ 。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 2.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。

#### 3.查询当前前缀函数值的失配转移(get_fail)

1. 数据类型

   输入参数 `uint32_t last_pi`，表示上一个元素匹配到的前缀函数值。

   输入参数 `const value_type &elem` ，表示现在要找的序列元素。

   返回类型 `uint32_t` ，表示前缀函数失配后的转移值。

2. 时间复杂度

   均摊 $O(1)$ ，最坏 $O(\log n)$ 。

#### 4.对某个位置的所有本质不同的 border 调用回调(do_for_each_distince_border)

1. 数据类型

   输入参数 `size_type init_border` ，表示初始 `border` 值。

   输入参数 `Callback &&call` ，表示对 `border` 调用的回调。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   一个位置的 `border` 的数量，最多可以达到和这个位置的下标同数量级。但是，一个位置的本质不同的 `border` 最多只有 `log n` 种。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/RollbackKMP.h"

int main() {
    // RollbackKMP 拥有和普通 kmp 一样的功能，仅仅是保证了 push_back 和 pop_back 不会因最坏情况而超时

    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::RollbackKMP_string kmp(p);
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith " << p.substr(0, j) << endl;
        }
    }

    // 给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = kmp.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl;

    // 如果要查找所有出现位置
    int cur_pi = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        cur_pi = kmp.next(cur_pi, c);
        if (p[cur_pi] == c) cur_pi++;
        if (cur_pi == kmp.size()) {
            int index = i - cur_pi + 1;
            cout << index << " " << s.substr(index, p.size()) << endl;
            cur_pi = kmp.query_Pi(cur_pi - 1);
        }
    }

    // 借助前缀函数可以快速求出字符串周期
    std::string q = "abcabcabcabcabcabcabc";
    // q 长度为 21
    int n = q.size();
    OY::RollbackKMP_string kmp2(q);
    auto val = kmp2.query_Pi(q.size() - 1);
    if (n % (n - val) == 0) {
        cout << n - val << " is minimum cycle of string " << q << endl;
    }
}
```

```
#输出如下
index 4: abcda startwith and endwith a
index 5: abcdab startwith and endwith ab
index 6: abcdabc startwith and endwith abc
index 8: abcdabcea startwith and endwith a
index 9: abcdabceab startwith and endwith ab
index 10: abcdabceabc startwith and endwith abc
index 11: abcdabceabcd startwith and endwith abcd
11 abcdabceabcd
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd
3 is minimum cycle of string abcabcabcabcabcabcabc

```

