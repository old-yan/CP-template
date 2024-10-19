#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

void test1() {
    // 使用从低位到高位添加数字的方式进行 dp
    // 统计 [1, n] 包含 9 的数的数量
    using OY::DIGITDP::IntStr10;
    auto solve = [](uint64_t n) {
        OY::DIGITDP::AppendHighSolver<uint64_t, IntStr10> sol;
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
    // 使用从高位到低位添加数字的方式进行 dp
    // 统计 [1, n] 包含 9 的数的数量
    using OY::DIGITDP::IntStr10;
    auto solve = [](uint64_t n) {
        OY::DIGITDP::AppendLowSolver<uint64_t, IntStr10> sol;
        // 状态数：2，分别表示没 9 /有 9
        auto transfer = [&](uint32_t old_state, uint32_t digit, uint32_t c) {
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

void test3() {
    // 使用从低位到高位添加数字的方式进行 dp
    // 统计 [1, n] 所包含的 9 字符的总数
    using OY::DIGITDP::IntStr10;
    auto solve = [](uint64_t n) {
        OY::DIGITDP::AppendHighSolver<uint64_t, IntStr10> sol;
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

void test4() {
    // 使用从高位到低位添加数字的方式进行 dp
    // 统计 [1, n] 所包含的 9 字符的总数
    using OY::DIGITDP::IntStr10;
    auto solve = [](uint64_t n) {
        OY::DIGITDP::AppendLowSolver<uint64_t, IntStr10> sol;
        // 状态数：20，表示具体包含了多少个 9
        auto transfer = [&](uint32_t old_state, uint32_t digit, uint32_t c) {
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

void test_debug1() {
    // 统计长度为偶数的单调递增数字
    // 开启 debug
    using OY::DIGITDP::IntStr10;
    OY::DIGITDP::AppendHighSolver<uint64_t, IntStr10, true> sol;
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
    cout << "f(" << n << ") = " << res << '\n';

    // 查看每个造成贡献的数字
    std::vector<int> ans;
    sol.enumerate(map, [&](int val) {
        ans.push_back(val);
    });
    for (auto x : ans) cout << x << ' ';
    cout << "\n\n";
}

void test_debug2() {
    // 统计长度为偶数的单调递增数字
    // 开启 debug
    using OY::DIGITDP::IntStr10;
    OY::DIGITDP::AppendLowSolver<uint64_t, IntStr10, true> sol;
    // 状态数：10，表示已有部分的最低位数字
    auto transfer = [&](uint32_t old_state, uint32_t digit, uint32_t c) -> uint32_t {
        if (!~old_state) return c;
        if (old_state < c) return c;
        return -1;
    };
    // 映射：[0,10) state 合法
    auto map = [&](uint32_t state, uint32_t len) {
        return len % 2 == 0;
    };
    uint64_t n = 100000;
    auto res = sol.solve(n, 10, transfer, map);
    cout << "f(" << n << ") = " << res << '\n';

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
    test3();
    test4();
    test_debug1();
    test_debug2();
}
/*
#输出如下
19 numbers contain '9' in range [1,100]
83322818300333431 numbers contain '9' in range [1,100000000000000000]

19 numbers contain '9' in range [1,100]
83322818300333431 numbers contain '9' in range [1,100000000000000000]

there are 20 '9' in range [1,100]
there are 170000000000000000 '9' in range [1,100000000000000000]

there are 20 '9' in range [1,100]
there are 170000000000000000 '9' in range [1,100000000000000000]

f(100000) = 162
12 13 14 15 16 17 18 19 23 24 25 26 27 28 29 34 35 36 37 38 39 45 46 47 48 49 56 57 58 59 67 68 69 78 79 89 1234 1235 1236 1237 1238 1239 1245 1246 1247 1248 1249 1256 1257 1258 1259 1267 1268 1269 1278 1279 1289 1345 1346 1347 1348 1349 1356 1357 1358 1359 1367 1368 1369 1378 1379 1389 1456 1457 1458 1459 1467 1468 1469 1478 1479 1489 1567 1568 1569 1578 1579 1589 1678 1679 1689 1789 2345 2346 2347 2348 2349 2356 2357 2358 2359 2367 2368 2369 2378 2379 2389 2456 2457 2458 2459 2467 2468 2469 2478 2479 2489 2567 2568 2569 2578 2579 2589 2678 2679 2689 2789 3456 3457 3458 3459 3467 3468 3469 3478 3479 3489 3567 3568 3569 3578 3579 3589 3678 3679 3689 3789 4567 4568 4569 4578 4579 4589 4678 4679 4689 4789 5678 5679 5689 5789 6789 

f(100000) = 162
12 23 13 24 34 14 25 35 45 15 26 36 46 56 16 27 37 47 57 67 17 28 38 48 58 68 78 18 29 39 49 59 69 79 89 19 1234 1235 1245 2345 1345 1236 1246 2346 1346 1256 2356 1356 2456 3456 1456 1237 1247 2347 1347 1257 2357 1357 2457 3457 1457 1267 2367 1367 2467 3467 1467 2567 3567 4567 1567 1238 1248 2348 1348 1258 2358 1358 2458 3458 1458 1268 2368 1368 2468 3468 1468 2568 3568 4568 1568 1278 2378 1378 2478 3478 1478 2578 3578 4578 1578 2678 3678 4678 5678 1678 1239 1249 2349 1349 1259 2359 1359 2459 3459 1459 1269 2369 1369 2469 3469 1469 2569 3569 4569 1569 1279 2379 1379 2479 3479 1479 2579 3579 4579 1579 2679 3679 4679 5679 1679 1289 2389 1389 2489 3489 1489 2589 3589 4589 1589 2689 3689 4689 5689 1689 2789 3789 4789 5789 6789 1789 

*/