### 一、模板类别

​	数据结构：数位 `DP` 助手。

​	练习题目：

1. [不要62](https://acm.hdu.edu.cn/showproblem.php?pid=2089)
2. [Valley Numer](https://acm.hdu.edu.cn/showproblem.php?pid=6148)
3. [233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one)
4. [357. 统计各位数字都不同的数字个数](https://leetcode.cn/problems/count-numbers-with-unique-digits/)
5. [600. 不含连续1的非负整数](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/)
6. [788. 旋转数字](https://leetcode.cn/problems/rotated-digits/)
7. [902. 最大为 N 的数字组合](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)
8. [1012. 至少有 1 位重复的数字](https://leetcode.cn/problems/numbers-with-repeated-digits/)
9. [1397. 找到所有好字符串](https://leetcode.cn/problems/find-all-good-strings/)
10. [1397. 找到所有好字符串](https://leetcode.cn/problems/find-all-good-strings/)
11. [2376. 统计特殊整数](https://leetcode.cn/problems/count-special-integers/)
12. [2719. 统计整数数目](https://leetcode.cn/problems/count-of-integers/)
13. [2801. 统计范围内的步进数字数目](https://leetcode.cn/problems/count-stepping-numbers-in-range/)
14. [2827. 范围中美丽整数的数目](https://leetcode.cn/problems/number-of-beautiful-integers-in-the-range/)
15. [2999. 统计强大整数的数目](https://leetcode.cn/problems/count-the-number-of-powerful-integers/)
16. [3007. 价值和小于等于 K 的最大数字](https://leetcode.cn/problems/maximum-number-that-sum-of-the-prices-is-less-than-or-equal-to-k/)
17. [#10165. 「一本通 5.3 例 3」Windy 数](https://loj.ac/p/10165)
18. [P2602 [ZJOI2010] 数字计数](https://www.luogu.com.cn/problem/P2602)
19. [P2657 [SCOI2009] windy 数](https://www.luogu.com.cn/problem/P2657)
20. [P3311 [SDOI2014] 数数](https://www.luogu.com.cn/problem/P3311)
21. [P3413 SAC#1 - 萌数](https://www.luogu.com.cn/problem/P3413)
22. [P4127 [AHOI2009] 同类分布](https://www.luogu.com.cn/problem/P4127)
23. [P4317 花神的数论题](https://www.luogu.com.cn/problem/P4317)
24. [P8764 [蓝桥杯 2021 国 BC] 二进制问题](https://www.luogu.com.cn/problem/P8764)


### 二、模板功能

​		本模板提供一个对数位 `dp` 的抽象模板。对于数位 `dp` 问题，使用者需要传递如下参数：

1. 表达数字数量的类型 `typename Tp` ；

2. 进制 `uint32_t Base` ；

3. 上限 `Integer<Base> number` ，表示要求的是 `[1, number]` 之间的和；

4. 状态数量 `uint32_t state_count` ，表示状态数量；

5. 函数 `Transfer` ，表示状态转移函数；

6. 函数 `ValueMapping` ，表示某个状态对应的权值是多少。

   本模板提供了二十道以上的例题，通过借鉴例题，可以掌握模板运用方法。

**注意：**


1. 状态转移函数 `transfer` 接受三个参数， `uint32_t old`  ，表示旧状态，位于 `[-1, state_count)` ； `uint32_t len` ，表示旧长度； `uint32_t c` ，表示在旧数字的基础上，添加一个数字 `c` 到最高位。需要返回一个 `uint32_t` 作为添加最高位之后的状态。

   切记，从空数字的基础上得到一个长度为 `1` 的数字时， `old` 值为 `-1` ；如果想要把某个状态舍弃掉，返回 `-1` ；如果想要保留该状态，就返回 `[0, state_count)` 。

2. 权值映射 `ValueMapping` 用于处理不同类型的问题。当问题为“有多少个满足某条件的数”时，本函数要么返回 `0` ，要么返回 `1` ；而当问题为“在 `[1,n]` 里的数里，数字 `7` 出现了多少次”时，显然本函数不能只返回 `0` 或者 `1` ，而要根据状态（数字 `7` 的数量）返回相应值。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

void test1() {
    // 统计 [1, n] 包含 9 的数的数量
    auto solve = [](uint64_t n) {
        OY::DIGITDP::Solver<uint64_t, 10> sol;
        // 状态数：2，分别表示没 9 /有 9
        auto transfer = [&](uint32_t old_state, uint32_t old_len, uint32_t c) {
            if (!~old_state) old_state = 0;
            return old_state || c == 9;
        };
        // 映射：状态为 1 时产生 1 的贡献
        auto map = [&](uint32_t state, uint32_t len) {
            return state == 1;
        };
        return sol.solve(n, 2, transfer, map);
    };

    cout << solve(100) << " numbers contain '9' in range [1,100]\n";
    cout << solve(100000000000000000) << " numbers contain '9' in range [1,100000000000000000]\n\n";
}

void test2() {
    // 统计 [1, n] 所包含的 9 字符的总数
    auto solve = [](uint64_t n) {
        OY::DIGITDP::Solver<uint64_t, 10> sol;
        // 状态数：20，表示具体包含了多少个 9
        auto transfer = [&](uint32_t old_state, uint32_t old_len, uint32_t c) {
            if (!~old_state) old_state = 0;
            return old_state + (c == 9);
        };
        // 映射：状态是多少，就产生多少贡献
        auto map = [&](uint32_t state, uint32_t len) {
            return state;
        };
        return sol.solve(n, 20, transfer, map);
    };

    cout << "there are " << solve(100) << " '9' in range [1,100]\n";
    cout << "there are " << solve(100000000000000000) << " '9' in range [1,100000000000000000]\n\n";
}

void test_debug() {
    // 统计长度为偶数的单调递增数字
    // 开启 debug
    OY::DIGITDP::Solver<uint64_t, 10, true> sol;
    // 状态数：10，表示已有部分的最高位数字
    auto transfer = [&](uint32_t old_state, uint32_t old_len, uint32_t c) -> uint32_t {
        if (!~old_state) return c;
        if (old_state <= c) return -1;
        return c;
    };
    // 映射：每个 state 都合法，只需要判断一下 len 即可
    auto map = [&](uint32_t state, uint32_t len) {
        return len % 2 == 0;
    };
    uint64_t n = 100000;
    auto res = sol.solve(n, 10, transfer, map);
    cout << "f(100000) = " << res << '\n';

    // 查看每个造成贡献的数字
    std::vector<int> ans;
    sol.enumerate(map, [&](int val) {
        ans.push_back(val);
    });
    for (auto x : ans) cout << x << ' ';
}

int main() {
    test1();
    test2();
    test_debug();
}
```

```
#输出如下
19 numbers contain '9' in range [1,100]
83322818300333431 numbers contain '9' in range [1,100000000000000000]

there are 20 '9' in range [1,100]
there are 170000000000000000 '9' in range [1,100000000000000000]

f(100000) = 162
12 13 14 15 16 17 18 19 23 24 25 26 27 28 29 34 35 36 37 38 39 45 46 47 48 49 56 57 58 59 67 68 69 78 79 89 1234 1235 1236 1237 1238 1239 1245 1246 1247 1248 1249 1256 1257 1258 1259 1267 1268 1269 1278 1279 1289 1345 1346 1347 1348 1349 1356 1357 1358 1359 1367 1368 1369 1378 1379 1389 1456 1457 1458 1459 1467 1468 1469 1478 1479 1489 1567 1568 1569 1578 1579 1589 1678 1679 1689 1789 2345 2346 2347 2348 2349 2356 2357 2358 2359 2367 2368 2369 2378 2379 2389 2456 2457 2458 2459 2467 2468 2469 2478 2479 2489 2567 2568 2569 2578 2579 2589 2678 2679 2689 2789 3456 3457 3458 3459 3467 3468 3469 3478 3479 3489 3567 3568 3569 3578 3579 3589 3678 3679 3689 3789 4567 4568 4569 4578 4579 4589 4678 4679 4689 4789 5678 5679 5689 5789 6789 

```

