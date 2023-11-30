### 一、模板类别

​	数学：欧拉筛。

​	练习题目：

1. [P3383 【模板】线性筛素数](https://www.luogu.com.cn/problem/P3383)
2. [P3912 素数个数](https://www.luogu.com.cn/problem/P3912)

### 二、模板功能

#### 1.构造

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板中涉及的数字类型。

   模板参数 `size_type MAX_RANGE` ，表示最大打表范围。

   模板参数 `bool GetPhi` ，表示是否在筛的同时记录欧拉函数值。默认为 `false` 。

   模板参数 `bool GetSmallFactor` ，表示是否在筛的同时记录每个数的最小质因数。默认为 `false` 。

   模板参数 `bool GetBigFactor` ，表示是否在筛的同时记录每个数的最大质因数。默认为 `false` 。

   构造参数 `size_type range` ，表示实际打表范围，默认为 `MAX_RANGE` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板用于从小到大找出一定范围里的所有质数。
   
   欧拉筛时间复杂度为线性，但是实际运行速度不及埃氏筛。但是，在 `GetBigFactor` 为 `true` 的情况下，运行速度远远超过埃氏筛。
   
   每个模板参数都是可勾选的，可以将不需要的属性设为 `false` ，提高时间和空间效率。

#### 2.查询是否为质数(is_prime)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

#### 3.查询欧拉函数(get_Euler_Phi)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `GetPhi` 为 `true`。

#### 4.查询最小质因数(query_smallest_factor)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `GetSmallFactor` 为 `true` 。

   特别的，当 `i == 1` 时返回 `1` 。

#### 5.查询最大质因数(query_biggest_factor)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `GetBigFactor` 为 `true` 。

   特别的，当 `i == 1` 时返回 `1` 。

#### 6.查询第 k 个质数(query_kth_prime)

1. 数据类型

   输入参数 `size_type k` ，表示要查询的次序。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求 `k` 在 `[0, count())` 范围内。

#### 7.查询质数总数(count)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 8.分解质因数(decomposite)

1. 数据类型

   输入参数 `size_type n` ，表示要查询的数字。

   返回类型 `std::vector<SievePair>` ，其中 `SievePair` 包含 `m_prime` 和 `m_count` 两个属性，表示包含的质因子以及包含的数量。所有的 `SievePair` 按照 `prime` 升序排列。

2. 时间复杂度

   取决于质因数的数量，一般可以认为是常数较小的 $O(\log n)$ 。

3. 备注

   本方法要求模板参数 `GetSmallFactor` 为 `true` ，且 `n` 为正数。

#### 9.找出所有因数(get_factors)

1. 数据类型

   模板参数 `bool Sorted` ，表示是否要把返回的因数排序。默认为 `false` 。

   输入参数 `size_type n` ，表示要查询的数字。

   返回类型 `std::vector<size_type>` ，表示升序排列的所有因数。

2. 时间复杂度

   取决于因数的数量，一般可以认为是常数较小的 $O(n^\frac{1}{3}))$ 。

3. 备注

   本方法要求模板参数 `GetSmallFactor` 为 `true` ，且 `n` 为正数。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/EulerSieve.h"

int main() {
    OY::EulerSieve::Sieve<10000000, true, true, true> ps;
    cout << "number of primes in [0, 10000000]: " << ps.count() << endl;
    cout << "No.0 prime is: " << ps.query_kth_prime(0) << endl;
    cout << "No.1 prime is: " << ps.query_kth_prime(1) << endl;
    cout << "No.2 prime is: " << ps.query_kth_prime(2) << endl;
    cout << "No.3 prime is: " << ps.query_kth_prime(3) << endl;
    cout << "No.4 prime is: " << ps.query_kth_prime(4) << endl;
    cout << "No.500000 prime is: " << ps.query_kth_prime(500000) << endl;

    cout << "1234567 is prime?" << (ps.is_prime(1234567) ? "yes\n" : "no\n");
    cout << "1234571 is prime?" << (ps.is_prime(1234571) ? "yes\n" : "no\n");

    cout << "smallest prime factor of 30: " << ps.query_smallest_factor(30) << endl;
    cout << "biggest prime factor of 30: " << ps.query_biggest_factor(30) << endl;

    uint32_t A = 2 * 3 * 3 * 5 * 5 * 97 * 101;
    auto pf = ps.decomposite(A);
    for (auto [p, c] : pf) {
        cout << p << '^' << c << endl;
    }

    uint32_t B = 2 * 3 * 5 * 7;
    auto fs = ps.get_factors<true>(B);
    for (auto f : fs) {
        cout << B << " % " << f << " = " << B % f << endl;
    }

    // 如果不想进行各种麻烦操作，只想知道质数有几个，可以把模板参数设为 false
    OY::EulerSieve::Sieve<10000000> simple_ps;
    cout << "number of primes in [0, 10000000]: " << simple_ps.count() << endl;
}
```

```
#输出如下
number of primes in [0, 10000000]: 664579
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.500000 prime is: 7368791
1234567 is prime?no
1234571 is prime?no
smallest prime factor of 30: 2
biggest prime factor of 30: 5
2^1
3^2
5^2
97^1
101^1
210 % 1 = 0
210 % 2 = 0
210 % 3 = 0
210 % 5 = 0
210 % 6 = 0
210 % 7 = 0
210 % 10 = 0
210 % 14 = 0
210 % 15 = 0
210 % 21 = 0
210 % 30 = 0
210 % 35 = 0
210 % 42 = 0
210 % 70 = 0
210 % 105 = 0
210 % 210 = 0
number of primes in [0, 10000000]: 664579

```

