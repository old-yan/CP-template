#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/GridCachedLambda.h"

using mint = OY::mint1000000007;
void test_factorial() {
    // 第一个参数类型必须为 auto && 或者 auto &
    auto fac = [](auto &&self, int n) -> mint {
        if (!n) return 1;
        return self(self, n - 1) * n;
    };

    // 需要指定两个参数
    // MakeRecord 表示是否记录每条结果，以便清空 cache
    // BUFFER 表示哈希表池子大小
    auto cached_fac = OY::make_StaticGridCacheSolver<false, 20001>(fac);

    cout << "fac[3] = " << cached_fac.query(3) << endl;
    cout << "fac[4] = " << cached_fac.query(4) << endl;
    cout << "fac[5] = " << cached_fac.query(5) << endl;

    // 输出 fac[10000~20000] 的和
    mint sum = 0;
    for (int i = 10000; i <= 20000; i++) sum += cached_fac.query(i);
    cout << "sum of fac[10000~20000] = " << sum << endl
         << endl;
}

void test_fibonacci() {
    auto cached_fib = OY::make_StaticGridCacheSolver<false, 1001>(
        [](auto &&self, int n) -> mint {
            return self(self, n - 1) + self(self, n - 2);
        });
    // 因为没有写 n<=1 时的初始值，所以需要手动设置递归出口
    cached_fib.set_initial_value(0, 1);
    cached_fib.set_initial_value(1, 1);

    cout << "fib[10] = " << cached_fib.query(10) << endl;
    cout << "fib[100] = " << cached_fib.query(100) << endl;
    cout << "fib[1000] = " << cached_fib.query(1000) << endl
         << endl;
}

void test_comb() {
    auto cached_comb = OY::make_StaticGridCacheSolver<true, 1001, 501>(
        [](auto &self, int n, int m) -> mint {
            if (!m) return 1;
            if (!n) return 0;
            return self(self, n - 1, m - 1) + self(self, n - 1, m);
        });

    cout << "comb[1000, 500] = " << cached_comb.query(1000, 500) << endl
         << endl;
}

void test_dynamic_comb() {
    // 如果是多维搜索，可能题目只保证行和列的乘积的大小限制
    // 此时可以使用动态模板
    auto cached_comb1 = OY::make_DynamicGridCacheSolver<true>(
        [](auto &self, int n, int m) -> mint {
            if (!m) return 1;
            if (!n) return 0;
            return self(self, n - 1, m - 1) + self(self, n - 1, m);
        },
        500001, 3);
    cout << "comb[500000, 2] = " << cached_comb1.query(500000, 2) << endl;

    auto cached_comb2 = OY::make_DynamicGridCacheSolver<true>(
        [](auto &self, int n, int m) -> mint {
            if (!m) return 1;
            if (!n) return 0;
            return self(self, n - 1, m - 1) + self(self, n - 1, m);
        },
        501, 500);
    cout << "comb[500, 499] = " << cached_comb2.query(500, 499) << endl;
}

int main() {
    test_factorial();
    test_fibonacci();
    test_comb();
    test_dynamic_comb();
}
/*
#输出如下
fac[3] = 6
fac[4] = 24
fac[5] = 120
sum of fac[10000~20000] = 864712836

fib[10] = 89
fib[100] = 782204094
fib[1000] = 107579939

comb[1000, 500] = 159835829

comb[500000, 2] = 999749132
comb[500, 499] = 500

*/
