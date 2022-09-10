### 一、模板类别

​	数学：埃拉托斯特尼筛。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `uint32_t _N` ，表示筛质数的范围。

   模板参数 `bool _Prime` ，表示是否将找出的所有质数收集起来。默认为 `true` 。

   模板参数 `bool _Phi` ，表示是否在筛的同时记录欧拉函数值。默认为 `false` 。

   模板参数 `bool _Small` ，表示是否在筛的同时记录每个数的最小质因数。默认为 `false` 。

   模板参数 `bool _Big` ，表示是否在筛的同时记录每个数的最大质因数。默认为 `false` 。

2. 时间复杂度

   $O(n\cdot \log \log n)$ ；如果要记录每个数的最大质因数，则为 $O(n\cdot \log n)$ 。

3. 备注

   本模板用于从小到大找出一定范围里的所有质数。
   
   本模板时间复杂度比线性高，但是实际运行速度远远超过线性的欧拉筛。
   
   每个模板参数都是可勾选的，可以将不需要的属性设为 `false` ，提高时间和空间效率。默认情况下，只有 `_Prime` 为 `true` ，表示只把找出的质数收集起来。

#### 2.查询是否为质数

1. 数据类型

   输入参数 `uint32_t __i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

#### 3.查询欧拉函数

1. 数据类型

   输入参数 `uint32_t __i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法要求模板参数 `_Phi` 为 `true` ，且 `__i` 为正数。

#### 4.查询最小质因数

1. 数据类型

   输入参数 `uint32_t __i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `_Small` 为 `true` ，且 `__i` 为正数。

   特别的，当 `__i==1` 时返回 `1` 。

#### 5.查询最大质因数

1. 数据类型

   输入参数 `uint32_t __i` ，表示要查询的数字。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `_Big` 为 `true` ，且 `__i` 为正数。

   特别的，当 `__i==1` 时返回 `1` 。

#### 6.查询第 k 个质数

1. 数据类型

   输入参数 `int __k` ，表示要查询的次序。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法要求模板参数 `_Prime` 为 `true` ，且 `__k` 在 `[0,count())` 范围内。

#### 7.查询质数总数

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 8.分解质因数

1. 数据类型

   输入参数 `uint32_t __n` ，表示要查询的数字。

   返回类型 `std::vector<node>` ，其中 `node` 包含 `prime` 和 `count` 两个属性，表示包含的质因子以及包含的数量。所有的 `node` 按照 `prime` 升序排列。

2. 时间复杂度

   取决于质因数的数量，一般可以认为是常数较大的 $O(1)$ 。

3. 备注

   本方法要求模板参数 `_Small` 为 `true` ，且 `__n` 为正数。

#### 9.找出所有因数

1. 数据类型

   输入参数 `uint32_t __n` ，表示要查询的数字。

   返回类型 `std::vector<uint32_t>` ，表示升序排列的所有因数。

2. 时间复杂度

   取决于因数的数量，一般可以认为是常数较大的 $O(\log n)$ 。

3. 备注

   本方法要求模板参数 `_Small` 为 `true` ，且 `__n` 为正数。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"

int main() {
    OY::EratosthenesSieve<100000000, true, true, true, true> ps;
    cout << "number of primes in [0,100000000]: " << ps.count() << endl;
    cout << "No.0 prime is: " << ps.queryKthPrime(0) << endl;
    cout << "No.1 prime is: " << ps.queryKthPrime(1) << endl;
    cout << "No.2 prime is: " << ps.queryKthPrime(2) << endl;
    cout << "No.3 prime is: " << ps.queryKthPrime(3) << endl;
    cout << "No.4 prime is: " << ps.queryKthPrime(4) << endl;
    cout << "No.5000000 prime is: " << ps.queryKthPrime(5000000) << endl;

    cout << "12345679 is prime?" << (ps.isPrime(12345679) ? "yes\n" : "no\n");
    cout << "12345701 is prime?" << (ps.isPrime(12345701) ? "yes\n" : "no\n");

    cout << "smallest prime factor of 30:" << ps.querySmallestFactor(30) << endl;
    cout << "biggest prime factor of 30:" << ps.queryBiggestFactor(30) << endl;

    uint32_t A = 2ull * 3 * 3 * 5 * 5 * 97 * 101;
    auto pf = ps.decomposite(A);
    for (auto [p, c] : pf) {
        cout << p << '^' << c << endl;
    }

    uint32_t B = 2 * 3 * 5 * 7;
    auto fs = ps.getFactors(B);
    for (auto f : fs) {
        cout << B << " % " << f << " = " << B % f << endl;
    }

    //如果不想进行各种麻烦操作，只想知道质数有几个，可以把模板参数设为 false
    OY::EratosthenesSieve<100000000, false, false, false, false> simple_ps;
    cout << "number of primes in [0,100000000]: " << simple_ps.count() << endl;
}
```

```
#输出如下
number of primes in [0,100000000]: 5761455
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.5000000 prime is: 86028157
12345679 is prime?no
12345701 is prime?yes
smallest prime factor of 30:2
biggest prime factor of 30:5
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
number of primes in [0,100000000]: 5761455

```

