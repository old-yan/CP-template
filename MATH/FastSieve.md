### 一、模板类别

​	数学：极速质数筛。

​	练习题目：

1. [P2398 GCD SUM](https://www.luogu.com.cn/problem/P2398)
2. [P3383 【模板】线性筛素数](https://www.luogu.com.cn/problem/P3383)
3. [P3912 素数个数](https://www.luogu.com.cn/problem/P3912)
4. [U151263 GCD 卷积](https://www.luogu.com.cn/problem/U151263)
5. [Enumerate Primes](https://judge.yosupo.jp/problem/enumerate_primes)(https://github.com/yosupo06/library-checker-problems/issues/158)
6. [Gcd Convolution](https://judge.yosupo.jp/problem/gcd_convolution)(https://github.com/yosupo06/library-checker-problems/issues/749)


### 二、模板功能

#### 1.构造

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板中各种数值的类型。

   模板参数 `size_type MAX_RANGE` ，表示打表的最大范围。

   构造参数 `size_type range` ，表示实际要打表的范围。默认为 `MAX_RANGE` 。

2. 时间复杂度

   $O(n\cdot \log \log n)$ 。

3. 备注

   本模板用于从小到大找出一定范围里的所有质数。
   
   欧拉筛的时间复杂度为线性，但是实际运行速度并不如埃氏筛。本筛是在埃氏筛的基础上优化而来。
   
   对于 `1e9` 规模以下的质数表，本模板都可以在 `600ms` 左右打完。（洛谷 `555ms` ， `atcoder` `599ms` ， `codeforces`  `601ms` ，本地 `MSVC`  `524ms` ，本地 `gcc` `550ms` ， `leetcode`  `870ms` ）

#### 2.将所有数的判质结果转到bitset(to_bitset)

1. 数据类型

2. 时间复杂度

   $O(\frac n {\log n})$ 。
   
3. 备注

   本方法是为了方便查询某数是否为质数。

#### 3.查询从小到大的第 k 个质数(query_kth_prime)

1. 数据类型

   输入参数 `uint32_t k` ，表示要查询的次序。

2. 时间复杂度

   $O(1)$ 。


#### 4.查询质数总数(count)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/FastSieve.h"

int main() {
    // 筛法找质数
    OY::FASTSIEVE::Sieve<100000000> ps;
    cout << "number of primes in [0, 100000000]: " << ps.count() << endl;
    cout << "No.0 prime is: " << ps.query_kth_prime(0) << endl;
    cout << "No.1 prime is: " << ps.query_kth_prime(1) << endl;
    cout << "No.2 prime is: " << ps.query_kth_prime(2) << endl;
    cout << "No.3 prime is: " << ps.query_kth_prime(3) << endl;
    cout << "No.4 prime is: " << ps.query_kth_prime(4) << endl;
    cout << "No.5000000 prime is: " << ps.query_kth_prime(5000000) << endl;

    // 但是这样很难查询某个数字是否为质数
    // 可以考虑把所有数字是否为质数的信息转到 bitset 里
    auto B = ps.to_bitset();
    cout << "1234567 is prime?" << (B[1234567] ? "yes" : "no") << endl;
    cout << "123457 is prime?" << (B[123457] ? "yes" : "no") << endl;
}
```

```
#输出如下
number of primes in [0, 100000000]: 5761455
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.5000000 prime is: 86028157
1234567 is prime?no
123457 is prime?yes

```

