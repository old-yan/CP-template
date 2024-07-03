### 一、模板类别

​	数学：莫比乌斯函数，积性函数。

​	练习题目：

1. [#P2693. jzptab](https://new.bzoj.org:88/p/P2693)
2. [P1829 [国家集训队] Crash的数字表格 / JZPTAB](https://www.luogu.com.cn/problem/P1829)
3. [P2158 [SDOI2008] 仪仗队](https://www.luogu.com.cn/problem/P2158)
4. [P2257 YY的GCD](https://www.luogu.com.cn/problem/P2257)
5. [P2522 [HAOI2011] Problem b](https://www.luogu.com.cn/problem/P2522)
6. [P3312 [SDOI2014] 数表](https://www.luogu.com.cn/problem/P3312)
7. [P3327 [SDOI2015] 约数个数和](https://www.luogu.com.cn/problem/P3327)
8. [P3455 [POI2007] ZAP-Queries](https://www.luogu.com.cn/problem/P3455)
9. [P3704 [SDOI2017] 数字表格](https://www.luogu.com.cn/problem/P3704)
10. [P5221 Product](https://www.luogu.com.cn/problem/P5221)
11. [炫酷反演魔术](https://ac.nowcoder.com/acm/problem/244326)

### 二、模板功能

​		本模板包含一个莫比乌斯函数模板，及一个积性函数计算器。由于莫比乌斯函数本身也为积性函数，所以也可以使用积性函数计算器计算莫比乌斯模板。

#### 1.构造

1. 数据类型

   类型设定 `typename size_type` ，表示下标的类型。
   
   模板参数 `size_type MAX_RANGE` ，表示莫比乌斯函数的最大处理值域。
   
   模板参数 `bool RangeQuery` ，表示是否要查询区间莫比乌斯函数的和。
   
   构造参数 `size_type range` ，表示实际要处理的值域。默认为 `MAX_RANGE` 。

2. 时间复杂度

    $O(n)$ 。
   
3. 备注

   本模板在欧拉筛的同时计算出每个点的莫比乌斯函数值；也可以支持区间的莫比乌斯函数值的和的查询。
   
   推式子题目严重依赖经验。给出的所有例题，均带有式子推演。以下为推式子常用技巧：
   
   1.  $\Sigma_{i=1}^m [k|i]\cdot f(i)$ ，可以化简为 $\Sigma_{i=1}^{\lfloor \frac m k\rfloor} f(i\cdot k)$ 。（称之为等差数列枚举优化）
   2.  $\Sigma_{i=1}^n \Sigma_{j=1}^m \Sigma_{d|\gcd(i,j)} f(i,j,d)$ ，可以化简为 $\Sigma_{d=1}^m \Sigma_{i=1}^n \Sigma_{j=1}^m [d|i]\cdot[d|j]\cdot f(i,j,d)$ ，并进一步化简为 $\Sigma_{d=1}^m \Sigma_{i=1}^{\lfloor\frac n d\rfloor} \Sigma_{j=1}^{\lfloor\frac m d\rfloor} f(i\cdot d, j\cdot d,d)$ 。（称之为因数枚举优化）
   3.  $\Sigma_{i=1}^n \Sigma_{j=1}^m f(\gcd(i,j))\cdot g(i,j)$ ，可以化简为 $\Sigma_{d=1}^m \Sigma_{i=1}^n \Sigma_{j=1}^m f(d)\cdot g(i,j)\cdot [d==\gcd(i,j)] $ ，并进一步化简为 $\Sigma_{d=1}^m f(d)\cdot\Sigma_{i=1}^{\lfloor\frac n d\rfloor} \Sigma_{j=1}^{\lfloor\frac m d\rfloor} g(i\cdot d,h\cdot d)\cdot[\gcd(i,j)==1]$ 。（称之为互质优化）
   4.  $\Sigma_{d=1}^n f(d) \Sigma_{e=1}^{\lfloor\frac n d\rfloor} g(d,e)\cdot h(n,d\cdot e)$ ，可以化简为 $\Sigma_{S=1}^n h(n,S)\cdot\Sigma_{d|S} f(d)\cdot g(d,\frac S d)$ 。由于 $\Sigma_{d|S}f(d)\cdot g(d,\frac S d)$ 与 `n` 无关，所以往往可以被预处理出来，然后应对不同的 `n` 的多测情况。（称之为整体优化）
   5.  $[\gcd(i,j)==1]$ 可以化简为 $\Sigma_{d|\gcd(i,j)} \mu(d) $ 。由此进一步得出， $\Sigma_{i=1}^n \Sigma_{j=1}^m f(\gcd(i,j))\cdot g(i,j)$ ，可以被化简为 $\Sigma_{d=1}^m f(d)\cdot\Sigma_{i=1}^{\lfloor\frac n d\rfloor} \Sigma_{j=1}^{\lfloor\frac m d\rfloor} g(i\cdot d,j\cdot d)\cdot \Sigma_{e|\gcd(i,j)} \mu(e)$ ，并进一步化简为 $\Sigma_{d=1}^m f(d)\cdot \Sigma_{e=1}^{\lfloor\frac m d\rfloor} \mu(e)\cdot \Sigma_{i=1}^{\lfloor\frac n {d\cdot e}\rfloor} \Sigma_{j=1}^{\lfloor\frac m {d\cdot e}\rfloor} g(i\cdot d\cdot e,j\cdot d\cdot e)$ ，并进一步化简为 $\Sigma_{S=1}^m \Sigma_{i=1}^{\lfloor\frac n S\rfloor} \Sigma_{j=1}^{\lfloor\frac m S\rfloor} g(i\cdot S,j\cdot S)\cdot\Sigma_{d|S} f(d)\cdot \mu(\frac S d)$ 。此时，式子后半部分可以被预处理。
   6.  $\Sigma_{i=1}^n \Sigma_{j=1}^n f(a_i,a_j)$ ，满足 $1\le a_i\le u$ 的情况下，可以化简为 $\Sigma_{i=1}^u \Sigma_{j=1}^u f(i,j)\cdot cnt(i)\cdot cnt(j)$ 。
   7.  $\Sigma_{d=1}^{i\cdot j} [d|({i\cdot j})]$ 可以化简为 $\Sigma_{d1|i}\Sigma_{d2|j} [\gcd(d1,d2)==1]$ 。
   
   使用上述技巧秒杀题目的例子：
   
   1. 求 $\Sigma_{i=1}^n \Sigma_{j=1}^m \frac{i\cdot j}{\gcd(i,j)}$ ，应用技巧 `5` 立得 $\Sigma_{S=1}^m \Sigma_{i=1}^{\lfloor\frac n S\rfloor} i\cdot \Sigma_{j=1}^{\lfloor\frac m S\rfloor} j\cdot\Sigma_{d|S} \frac{S\cdot S\cdot\mu(\frac S d)} d$ 。
   2. 求 $\Sigma_{i=1}^n\Sigma_{j=1}^m [\gcd(i,j)==1]$ ，应用技巧 `5` 立得 $\Sigma_{S=1}^m\Sigma_{i=1}^{\lfloor\frac n S\rfloor} \Sigma_{j=1}^{\lfloor\frac m S\rfloor} \mu(S)$ 。
   3. 求 $\Sigma_{i=1}^n\Sigma_{j=1}^m [isprime(\gcd(i,j))]$ ，应用技巧 `5` 立得 $\Sigma_{S=1}^m\Sigma_{i=1}^{\lfloor\frac n S\rfloor} \Sigma_{j=1}^{\lfloor\frac m S\rfloor} \Sigma_{d|S} \mu(\frac S d)\cdot [isprime(d)]$ 。
   4. 求 $\Sigma_{i=1}^n\Sigma_{j=1}^m [\gcd(i,j)==k]$ ，应用技巧 `5` 立得 $\Sigma_{S=1}^{\lfloor\frac m k\rfloor}\Sigma_{i=1}^{\lfloor\frac n {S\cdot k}\rfloor} \Sigma_{j=1}^{\lfloor\frac m {S\cdot k}\rfloor} \mu(S)$ 。
   5. 求 $\prod_{i=1}^m f(i)^{\Sigma_{j=1}^m g(i,j)\cdot h(\lfloor\frac n {i\cdot j}\rfloor,\lfloor\frac m {i\cdot j}\rfloor)}$ ，应用类似技巧 `4` 得 $\prod_{S=1}^m \prod_{i|S} f(i)^{g(i,\frac S i)\cdot h(\lfloor\frac n S\rfloor,\lfloor\frac m S\rfloor)}$ ，即 $\prod_{S=1}^m {(\prod_{i|S} f(i)^{g(i,\frac S i)})}^{h(\lfloor\frac n S\rfloor,\lfloor\frac m S\rfloor)}$ 。
   6. 求 $\prod_{i=1}^n\prod_{j=1}^m \gcd(i,j)$ ，应用类似技巧 `3` 得 $\prod_{d=1}^m d^{\Sigma_{i=1}^n \Sigma_{j=1}^m [d==\gcd(i,j)]}$ ，即 $\prod_{d=1}^m d^{\Sigma_{i=1}^{\lfloor\frac n d\rfloor} \Sigma_{j=1}^{\lfloor\frac m d\rfloor} [\gcd(i,j)==1]}$ ，即 $\prod_{d=1}^m d^{\Sigma_{S=1}^{\lfloor\frac m d\rfloor} \lfloor\frac n {S\cdot d}\rfloor\lfloor\frac m {S\cdot d}\rfloor \mu(S)}$ 。应用类似技巧 `5` 得 $\prod_{T=1}^m\prod_{d|T} d^{\lfloor\frac n T\rfloor\lfloor\frac m T\rfloor \mu(\frac T d)}$ ，即 $\prod_{T=1}^m (\prod_{d|T} d^{\mu(\frac T d)})^{\lfloor\frac n T\rfloor\lfloor\frac m T\rfloor }$  。

#### 2.查询莫比乌斯函数值(query_mobius)

1. 数据类型

   输入参数 `size_type x` ，表示要查询的自变量。
   
   返回类型 `int` ，表示莫比乌斯函数值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法的返回值只可能是 `-1` ， `0` 或 `1` 。
   
#### 3.查询区间莫比乌斯函数值的和(query_mobius)

1. 数据类型

   输入参数 `size_type left` ，表示区间起始位置。

   输入参数 `size_type right` ，表示区间结尾位置。（闭区间）
   
   返回类型 `int` ，表示莫比乌斯函数值。

2. 时间复杂度

   $O(1)$ 。
   
#### 4.查询是否为质数(is_prime)

1. 数据类型

   输入参数 `size_type x` ，表示要查询的数字。
   
   返回类型 `bool` 。

2. 时间复杂度

   $O(1)$ 。

#### 5.计算积性函数(solve)

1. 数据类型

   输入参数 `size_type range` ，表示要处理的数值范围。

   输入参数 `Tp *array` ，表示要把积性函数写入到的数组指针。

   输入参数 `CalcPrime &&calc_prime` ，表示这个积性函数在质数处的计算方法。

   输入参数 `CalcPrimePow &&calc_prime_pow` ，表示这个积性函数在某质数的幂次处的计算方法。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   假设 `f(x)` 为积性函数，则本方法需要指定 `f(x)` 在 `x==p` 即自变量为质数时的计算方法；还需要指定 `x==p^c (c>=2)`  即自变量为质数的二次或二次以上幂时的计算方法。当含有本质不同的多个质因子时，可以根据积性函数的定义自行计算出函数值。

   在 `x==1` 时， `f(x)` 的值必定等于 `1` 。

#### 6.计算积性函数(solve)

1. 数据类型

   基本同上，不需要传递 `Tp *array` 。

   返回类型 `std::vector<Tp>` ，表示把积性函数存储到了这个数组里。

2. 时间复杂度

   同上。
   
#### 7.迪利克雷前缀和差分(partial_sum_Dirichlet_divisor/adjacent_difference_Dirichlet_divisor)

1. 数据类型

   输入参数 `size_type range` ，表示要处理的数值范围。
   
   输入参数 `Tp *array` ，表示数组指针。
   
   输入参数 `FindKthPrime &&find_kth_prime` ，表示一个用来找第 `k` 个质数的函数。

2. 时间复杂度

    $O(n\log\log n)$ 。
    
3. 备注

   本函数类似与标准库的 `partial_sum` 和 `adjacent_difference` ，最终效果是令 `a[i]` 处存放 `i` 的所有因子 `j` 处的 `a[j]` 的原数据之和；及其逆运算。
   
   本函数专门为处理数论而设计，数组长度应为 `range+1` ，下标 `0` 处不参与运算。

#### 8.迪利克雷后缀和差分(partial_sum_Dirichlet_multiple/adjacent_difference_Dirichlet_multiple)

1. 数据类型

   输入参数 `size_type range` ，表示要处理的数值范围。
   
   输入参数 `Tp *array` ，表示数组指针。
   
   输入参数 `FindKthPrime &&find_kth_prime` ，表示一个用来找第 `k` 个质数的函数。

2. 时间复杂度

    $O(n\log\log n)$ 。
    
3. 备注

   本函数类似与标准库的 `partial_sum` 和 `adjacent_difference` ，最终效果是令 `a[i]` 处存放 `i` 的所有倍数 `j` 处的 `a[j]` 的原数据之和；及其逆运算。
   
   本函数专门为处理数论而设计，数组长度应为 `range+1` ，下标 `0` 处不参与运算。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Mobius.h"

#include <random>

OY::MOBIUS::Table<100000, true> Mu;
void test0() {
    // 对于有两个或两个以上相同质因子的数字，莫比乌斯函数为 0
    int s = 3 * 11 * 11 * 17;
    cout << "\\mu(3 * 11 * 11 * 17) = " << Mu.query_mobius(s) << endl;

    s = 17 * 17 * 17;
    cout << "\\mu(17 * 11 * 17) = " << Mu.query_mobius(s) << endl;
}

void test1() {
    // 对于没有两个或两个以上相同质因子的数字
    // 如果质因子种类为偶数，莫比乌斯函数为 1
    int s = 3 * 7 * 11 * 17;
    cout << "\\mu(3 * 7 * 11 * 17) = " << Mu.query_mobius(s) << endl;
    s = 1;
    cout << "\\mu(1) = " << Mu.query_mobius(s) << endl;

    // 如果质因子种类为奇数，莫比乌斯函数为 -1
    s = 3 * 11 * 17;
    cout << "\\mu(3 * 11 * 17) = " << Mu.query_mobius(s) << endl;
    s = 2 * 3 * 5 * 7 * 17;
    cout << "\\mu(2 * 3 * 5 * 7 * 17) = " << Mu.query_mobius(s) << endl
         << endl;
}

void test_range_mobius() {
    cout << "\\mu(95) = " << Mu.query_mobius(95) << endl;
    cout << "\\mu(96) = " << Mu.query_mobius(96) << endl;
    cout << "\\mu(97) = " << Mu.query_mobius(97) << endl;
    cout << "\\mu(98) = " << Mu.query_mobius(98) << endl;
    cout << "\\mu(99) = " << Mu.query_mobius(99) << endl;
    cout << "\\mu(100) = " << Mu.query_mobius(100) << endl;
    cout << "\\mu(101) = " << Mu.query_mobius(101) << endl;
    cout << "\\mu(102) = " << Mu.query_mobius(102) << endl;
    cout << "\\mu(95~102) = " << Mu.query_mobius(95, 102) << endl
         << endl;
}

void test_multipicative() {
    cout << "test of multiplicative calculator:\n";
    // 尝试计算 f(x)=x的不同因子的数量
    // 众所周知，一个数包含的不同因子的数量，为积性函数
    auto calc_prime = [](int p) {
        // 计算 f(p)
        // 此处 p 为质数
        return 2; // 显然，质数只有两个因子，1 和自身
    };
    auto calc_prime_pow = [&](int p, int cnt, int low) {
        // 计算 f(p^cnt)
        // 此处 p 为质数，cnt 为次数，low = p ^ (cnt-1)
        // 显然，f(p^cnt)=f(p^(cnt-1))+1
        // 或者 f(p^cnt)=cnt+1
        return cnt + 1;
    };
    auto f = OY::MOBIUS::Multiplicative<100>().solve<int>(100, calc_prime, calc_prime_pow);
    cout << "count of factors of 36 = " << f[36] << endl;
    cout << "count of factors of 49 = " << f[49] << endl
         << endl;
}

void test_Dirichlet_divisor() {
    cout << "test of Dirichlet_divisor:\n";
    // 利用迪利克雷前缀和，求每个数字的因子数
    int a[101]{};
    for (int i = 1; i <= 100; i++) a[i] = 1;
    OY::MOBIUS::partial_sum_Dirichlet_divisor(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of factors of 36 = " << a[36] << endl;
    cout << "count of factors of 49 = " << a[49] << endl;
         
    // 利用迪利克雷前缀和，求每个数字的因子的因子数之和
    OY::MOBIUS::partial_sum_Dirichlet_divisor(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of count of factors of 36 = " << a[36] << endl;
    cout << "count of count of factors of 49 = " << a[49] << endl
         << endl;
}

void test_Dirichlet_multiple() {
    cout << "test of Dirichlet_multiple:\n";
    // 利用迪利克雷前缀和，求每个数字的倍数数量
    int a[101]{};
    for (int i = 1; i <= 100; i++) a[i] = 1;
    OY::MOBIUS::partial_sum_Dirichlet_multiple(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of multiples of 12 = " << a[12] << endl;
    cout << "count of multiples of 16 = " << a[16] << endl;
         
    // 利用迪利克雷前缀和，求每个数字的倍数倍数数量之和
    OY::MOBIUS::partial_sum_Dirichlet_multiple(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of count of multiples of 12 = " << a[12] << endl;
    cout << "count of count of multiples of 16 = " << a[16] << endl
         << endl;
}

void test_extra() {
    // 只有 1 的因子的莫比乌斯函数和为 1
    // 所以 [x==1] == \Sigma{d|x} \mu(d)
    for (int i = 1; i <= 10; i++) {
        int sum = 0;
        for (int divisor = 1; divisor <= i; divisor++)
            if (i % divisor == 0) {
                sum += Mu.query_mobius(divisor);
            }
        cout << "sum(\\mu(divisor of " << i << ")) = " << sum << endl;
    }
}

int main() {
    test0();
    test1();
    test_range_mobius();
    test_multipicative();
    test_Dirichlet_divisor();
    test_Dirichlet_multiple();
    test_extra();
}
```

```
#输出如下
\mu(3 * 11 * 11 * 17) = 0
\mu(17 * 11 * 17) = 0
\mu(3 * 7 * 11 * 17) = 1
\mu(1) = 1
\mu(3 * 11 * 17) = -1
\mu(2 * 3 * 5 * 7 * 17) = -1

\mu(95) = 1
\mu(96) = 0
\mu(97) = -1
\mu(98) = 0
\mu(99) = 0
\mu(100) = 0
\mu(101) = -1
\mu(102) = -1
\mu(95~102) = -2

test of multiplicative calculator:
count of factors of 36 = 9
count of factors of 49 = 3

test of Dirichlet_divisor:
count of factors of 36 = 9
count of factors of 49 = 3
count of count of factors of 36 = 36
count of count of factors of 49 = 6

test of Dirichlet_multiple:
count of multiples of 12 = 8
count of multiples of 16 = 6
count of count of multiples of 12 = 20
count of count of multiples of 16 = 14

sum(\mu(divisor of 1)) = 1
sum(\mu(divisor of 2)) = 0
sum(\mu(divisor of 3)) = 0
sum(\mu(divisor of 4)) = 0
sum(\mu(divisor of 5)) = 0
sum(\mu(divisor of 6)) = 0
sum(\mu(divisor of 7)) = 0
sum(\mu(divisor of 8)) = 0
sum(\mu(divisor of 9)) = 0
sum(\mu(divisor of 10)) = 0

```

