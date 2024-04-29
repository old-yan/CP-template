#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "TEST/std_gcd_lcm.h"

#include <random>
std::mt19937 my_rand;

void test_bitor() {
    cout << "test bitor transform:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[i | j] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_bitor_transform<true>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_bitor_transform<true>(arr2.begin(), arr2.end());
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_bitor_transform<false>(res.begin(), res.end());
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_bitor_transform<false>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_bitor_transform<false>(arr2.begin(), arr2.end());

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int len = 1; len < n; len <<= 1)
        for (int i = 0; i < n; i++)
            if (!(i & len)) transformer.add_transfer(i, i | len);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

void test_bitand() {
    cout << "test bitand transform:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[i & j] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_bitand_transform<true>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_bitand_transform<true>(arr2.begin(), arr2.end());
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_bitand_transform<false>(res.begin(), res.end());
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_bitand_transform<false>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_bitand_transform<false>(arr2.begin(), arr2.end());

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int len = 1; len < n; len <<= 1)
        for (int i = 0; i < n; i++)
            if (i & len) transformer.add_transfer(i, i & ~len);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

void test_bitxor() {
    cout << "test bitxor transform:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[i ^ j] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_bitxor_transform<true>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_bitxor_transform<true>(arr2.begin(), arr2.end());
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_bitxor_transform<false>(res.begin(), res.end());
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    cout << endl;
}

void test_max() {
    cout << "test max transform:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[std::max(i, j)] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_max_transform<true>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_max_transform<true>(arr2.begin(), arr2.end());
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_max_transform<false>(res.begin(), res.end());
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_max_transform<false>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_max_transform<false>(arr2.begin(), arr2.end());

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int i = 0; i + 1 < n; i++)
        transformer.add_transfer(i, i + 1);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

void test_min() {
    cout << "test min transform:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[std::min(i, j)] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_min_transform<true>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_min_transform<true>(arr2.begin(), arr2.end());
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_min_transform<false>(res.begin(), res.end());
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_min_transform<false>(arr1.begin(), arr1.end());
    OY::FASTTRANS::fast_min_transform<false>(arr2.begin(), arr2.end());

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int i = n - 1; i - 1 >= 0; i--)
        transformer.add_transfer(i, i - 1);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

void test_gcd() {
    cout << "test gcd transform:\n";
    static constexpr int n = 16;
    // 制造随机数据，注意 gcd 卷积的区间下标 0 处元素往往为 0
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    arr1[0] = 0;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    arr2[0] = 0;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ans[std::gcd(i, j)] += arr1[i] * arr2[j];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // gcd 卷积变换需要枚举质数，这里做一个简单的预处理
    std::vector<uint32_t> primes;
    auto isprime = [&](uint32_t x) {
        if (x < 2) return false;
        for (uint32_t i = 2; i < x; i++)
            if (x % i == 0) return false;
        return true;
    };
    for (uint32_t i = 1; i <= 1000; i++)
        if (isprime(i)) primes.push_back(i);
    auto query_kth_prime = [&](uint32_t k) { return primes[k]; };

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_gcd_transform<true>(arr1.begin(), arr1.end(), query_kth_prime);
    OY::FASTTRANS::fast_gcd_transform<true>(arr2.begin(), arr2.end(), query_kth_prime);
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_gcd_transform<false>(res.begin(), res.end(), query_kth_prime);
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_gcd_transform<false>(arr1.begin(), arr1.end(), query_kth_prime);
    OY::FASTTRANS::fast_gcd_transform<false>(arr2.begin(), arr2.end(), query_kth_prime);

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int p = 1; p < n; p++)
        if (isprime(p))
            for (int i = (n - 1) / p * p; i > 0; i -= p)
                transformer.add_transfer(i, i / p);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

void test_lcm() {
    cout << "test lcm transform:\n";
    static constexpr int n = 16;
    // 制造随机数据，注意 lcm 卷积的区间下标 0 处元素往往为 0
    std::vector<int> arr1(n), arr2(n);
    for (auto &a : arr1) a = my_rand() % 100;
    arr1[0] = 0;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    arr2[0] = 0;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            uint32_t x = std::lcm(i, j);
            if (x < n) ans[x] += arr1[i] * arr2[j];
        }
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // lcm 卷积变换需要枚举质数，这里做一个简单的预处理
    std::vector<uint32_t> primes;
    auto isprime = [&](uint32_t x) {
        if (x < 2) return false;
        for (uint32_t i = 2; i < x; i++)
            if (x % i == 0) return false;
        return true;
    };
    for (uint32_t i = 1; i <= 1000; i++)
        if (isprime(i)) primes.push_back(i);
    auto query_kth_prime = [&](uint32_t k) { return primes[k]; };

    // 使用快速变换
    std::vector<int> res(n);
    OY::FASTTRANS::fast_lcm_transform<true>(arr1.begin(), arr1.end(), query_kth_prime);
    OY::FASTTRANS::fast_lcm_transform<true>(arr2.begin(), arr2.end(), query_kth_prime);
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i];
    OY::FASTTRANS::fast_lcm_transform<false>(res.begin(), res.end(), query_kth_prime);
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
    // 为了复用 arr1 arr2，这里把它们变回来
    OY::FASTTRANS::fast_lcm_transform<false>(arr1.begin(), arr1.end(), query_kth_prime);
    OY::FASTTRANS::fast_lcm_transform<false>(arr2.begin(), arr2.end(), query_kth_prime);

    // 使用自定义变换器
    std::vector<int> res2(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    for (int p = 1; p < n; p++)
        if (isprime(p))
            for (int i = 1; i * p < n; i++)
                transformer.add_transfer(i, i * p);
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    for (int i = 0; i < n; i++) res2[i] = arr1[i] * arr2[i];
    transformer.transform<false>(res2);
    cout << "res2: ";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << " \n"[i == res2.size() - 1];
    cout << endl;
}

#include "TREE/DoubleLCA.h"
#include "TREE/LinkTree.h"
#include "TREE/RandTree.h"
void test_tree_lca() {
    cout << "test tree lca:\n";
    static constexpr int n = 16;
    // 制造随机数据
    std::vector<int> arr1(n), arr2(n), arr3(n);
    for (auto &a : arr1) a = my_rand() % 100;
    cout << "arr1: ";
    for (int i = 0; i < arr1.size(); i++) cout << arr1[i] << " \n"[i == arr1.size() - 1];
    for (auto &a : arr2) a = my_rand() % 100;
    cout << "arr2: ";
    for (int i = 0; i < arr2.size(); i++) cout << arr2[i] << " \n"[i == arr2.size() - 1];
    for (auto &a : arr3) a = my_rand() % 100;
    cout << "arr3: ";
    for (int i = 0; i < arr3.size(); i++) cout << arr3[i] << " \n"[i == arr3.size() - 1];
    // 制造随机树
    auto tree = OY::RandTree::make_tree<OY::LinkTree::Tree<bool, 100>>(n, my_rand);
    tree.set_root(0);
    auto D = OY::DoubleLCA::Table<decltype(tree)>(&tree);

    // 暴力计算结果
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                ans[D.calc(D.calc(i, j), k)] += arr1[i] * arr2[j] * arr3[k];
    cout << "ans:  ";
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];

    // 使用自定义变换器
    std::vector<int> res(n);
    OY::FASTTRANS::FastTranformer transformer(n, 0);
    tree.tree_dp_vertex(tree.m_root, {}, {}, [&](uint32_t a) { if (~D.find_parent(a)) transformer.add_transfer(a, D.find_parent(a)); });
    transformer.transform<true>(arr1);
    transformer.transform<true>(arr2);
    transformer.transform<true>(arr3);
    for (int i = 0; i < n; i++) res[i] = arr1[i] * arr2[i] * arr3[i];
    transformer.transform<false>(res);
    cout << "res:  ";
    for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
}

int main() {
    test_bitor();
    test_bitand();
    test_bitxor();
    test_max();
    test_min();
    test_gcd();
    test_lcm();

    // 一个很罕见的树上卷积科技，理论上也可以用树形 dp 做到一样的效果
    test_tree_lca();
}
/*
#输出如下
test bitor transform:
arr1: 12 2 34 85 4 91 29 85 98 3 35 65 40 26 39 20
arr2: 19 4 97 6 9 9 43 39 46 26 36 53 60 71 12 9
ans:  228 94 5108 11328 220 3927 7716 48671 6922 3681 23184 45313 13266 39006 37058 114330
res:  228 94 5108 11328 220 3927 7716 48671 6922 3681 23184 45313 13266 39006 37058 114330
res2: 228 94 5108 11328 220 3927 7716 48671 6922 3681 23184 45313 13266 39006 37058 114330

test bitand transform:
arr1: 29 18 26 95 53 93 87 28 44 6 95 20 97 76 45 34
arr2: 7 2 58 93 60 6 22 86 16 27 34 6 39 61 26 8
ans:  111841 35608 76123 27849 75329 24045 19306 5556 39253 5798 11186 484 23764 7318 2414 272
res:  111841 35608 76123 27849 75329 24045 19306 5556 39253 5798 11186 484 23764 7318 2414 272
res2: 111841 35608 76123 27849 75329 24045 19306 5556 39253 5798 11186 484 23764 7318 2414 272

test bitxor transform:
arr1: 52 50 12 52 1 73 25 42 9 79 91 26 9 80 88 8
arr2: 78 23 99 0 86 73 82 83 20 69 59 44 54 95 47 36
ans:  41999 38572 35073 45520 42102 39415 33271 50276 43184 39535 41228 41577 45789 39998 41459 41758
res:  41999 38572 35073 45520 42102 39415 33271 50276 43184 39535 41228 41577 45789 39998 41459 41758

test max transform:
arr1: 42 11 0 44 81 29 78 75 2 0 71 48 41 67 96 58
arr2: 97 28 56 24 32 99 78 0 54 75 57 23 63 9 55 85
ans:  4074 2551 2968 10292 22301 27366 48438 31050 20376 27150 63234 39863 58429 51263 104395 106655
res:  4074 2551 2968 10292 22301 27366 48438 31050 20376 27150 63234 39863 58429 51263 104395 106655
res2: 4074 2551 2968 10292 22301 27366 48438 31050 20376 27150 63234 39863 58429 51263 104395 106655

test min transform:
arr1: 42 35 81 30 20 56 38 51 74 70 8 5 94 46 42 0
arr2: 3 70 79 6 85 15 25 88 45 18 38 45 17 91 3 3
ans:  28452 65030 87384 17394 50600 28148 23924 47580 31165 18560 8682 8985 12212 8284 252 0
res:  28452 65030 87384 17394 50600 28148 23924 47580 31165 18560 8682 8985 12212 8284 252 0
res2: 28452 65030 87384 17394 50600 28148 23924 47580 31165 18560 8682 8985 12212 8284 252 0

test gcd transform:
arr1: 0 51 91 58 34 52 15 59 93 98 33 68 24 84 96 49
arr2: 0 0 79 67 9 66 0 6 33 70 76 10 18 9 23 52
ans:  0 302453 77822 40398 5559 20940 270 2287 3069 6860 2508 680 432 756 2208 2548
res:  0 302453 77822 40398 5559 20940 270 2287 3069 6860 2508 680 432 756 2208 2548
res2: 0 302453 77822 40398 5559 20940 270 2287 3069 6860 2508 680 432 756 2208 2548

test lcm transform:
arr1: 0 49 42 46 1 55 41 15 87 22 25 78 13 0 33 68
arr2: 0 33 44 28 58 81 24 12 30 28 15 88 35 53 77 83
ans:  0 1617 5390 4178 5413 10239 11777 1263 17115 4618 12337 13750 14249 2597 14804 33016
res:  0 1617 5390 4178 5413 10239 11777 1263 17115 4618 12337 13750 14249 2597 14804 33016
res2: 0 1617 5390 4178 5413 10239 11777 1263 17115 4618 12337 13750 14249 2597 14804 33016

test tree lca:
arr1: 91 48 98 50 0 84 98 91 42 0 25 68 39 44 96 64
arr2: 73 76 86 24 48 38 3 86 16 71 57 61 14 0 87 20
arr3: 97 68 40 39 72 53 40 4 19 94 3 55 80 39 85 28
ans:  295319760 1233912 86309052 300232 0 1612248 11760 4072128 31177140 37829460 80279848 228140 43680 42546960 709920 35840
res:  295319760 1233912 86309052 300232 0 1612248 11760 4072128 31177140 37829460 80279848 228140 43680 42546960 709920 35840

*/