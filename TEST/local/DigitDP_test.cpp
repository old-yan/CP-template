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
/*
#输出如下
19 numbers contain '9' in range [1,100]
83322818300333431 numbers contain '9' in range [1,100000000000000000]

there are 20 '9' in range [1,100]
there are 170000000000000000 '9' in range [1,100000000000000000]

f(100000) = 162
12 13 14 15 16 17 18 19 23 24 25 26 27 28 29 34 35 36 37 38 39 45 46 47 48 49 56 57 58 59 67 68 69 78 79 89 1234 1235 1236 1237 1238 1239 1245 1246 1247 1248 1249 1256 1257 1258 1259 1267 1268 1269 1278 1279 1289 1345 1346 1347 1348 1349 1356 1357 1358 1359 1367 1368 1369 1378 1379 1389 1456 1457 1458 1459 1467 1468 1469 1478 1479 1489 1567 1568 1569 1578 1579 1589 1678 1679 1689 1789 2345 2346 2347 2348 2349 2356 2357 2358 2359 2367 2368 2369 2378 2379 2389 2456 2457 2458 2459 2467 2468 2469 2478 2479 2489 2567 2568 2569 2578 2579 2589 2678 2679 2689 2789 3456 3457 3458 3459 3467 3468 3469 3478 3479 3489 3567 3568 3569 3578 3579 3589 3678 3679 3689 3789 4567 4568 4569 4578 4579 4589 4678 4679 4689 4789 5678 5679 5689 5789 6789 

*/