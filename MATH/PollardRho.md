### 一、模板类别

​	数学： `Pollard Rho` 算法。

​	练习题目：

1. [P1075 [NOIP2012 普及组] 质因数分解](https://www.luogu.com.cn/problem/P1075)
2. [P4718 【模板】Pollard-Rho](https://www.luogu.com.cn/problem/P4718)
3. [P6091 【模板】原根](https://www.luogu.com.cn/problem/P6091)
4. [Factorize](https://judge.yosupo.jp/problem/factorize)(https://github.com/yosupo06/library-checker-problems/issues/41)
5. [Primitive Root](https://judge.yosupo.jp/problem/primitive_root)(https://github.com/yosupo06/library-checker-problems/issues/851)


### 二、模板功能

#### 1.找出一个非平凡因数(pick)

1. 数据类型

   输入参数 `uint64_t n` ，表示要查询的数。

2. 时间复杂度

    $O(n^\frac{1}{4})$ 。

3. 备注

   本函数用于找出某一数字的某个非平凡因数。

   本函数的时间复杂度为基于概率的时间复杂度。

   本模板与其他利用筛预处理，然后分解质因数的模板的区别是：

   1. 本模板的适用数值范围更大，可以用于 `[1, 2^62)` 范围内的数；
   2. 本模板无需预处理；
   3. 本模板的分解速度略慢。

   **注意：** 在传参时，请保证传入的参数为合数。

#### 2.分解质因数(decomposite)

1. 数据类型

   模板参数 `bool Sorted` ，表示返回的质因数是否按照升序排列。

   输入参数 `uint64_t n` ，表示要查询的数。

   返回类型 `std::vector<PollardRhoPair>` ，其中 `PollardRhoPair` 包含 `m_prime` 和 `m_count` 两个属性，表示包含的质因子以及包含的数量。

2. 时间复杂度

    $O(n^\frac{1}{4})$ 。

3. 备注

   本函数用于找出某一数字的所有质因数。

   本函数的时间复杂度为基于概率的时间复杂度。

#### 3.找出所有因数(get_factors)

1. 数据类型

   模板参数 `bool Sorted` ，表示返回的质因数是否按照升序排列。

   输入参数 `uint64_t n` ，表示要查询的数。

   返回类型 `std::vector<uint64_t>` ，表示升序排列的所有因数。

2. 时间复杂度

    $O(n^\frac{1}{3})$ 。

3. 备注

   本函数用于找出某一数字的所有因数。
   
   本函数首先找出所有质因子，再组合生成所有的因子。一个数字的因子数量，最多在其立方根数量级附近。

#### 4.求欧拉函数(get_Euler_Phi)

1. 数据类型

   输入参数 `uint64_t n` ，表示要查询的数。

2. 时间复杂度

    $O(n^\frac{1}{4})$ 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

int main() {
    uint64_t A = 2ull * 3 * 3 * 5 * 5 * 95986273 * 265988969;
    auto pairs = OY::PollardRho::decomposite<true>(A);
    for (auto &&pair : pairs) {
        cout << pair.m_prime << '^' << pair.m_count << endl;
    }

    uint64_t B = 2 * 3 * 5 * 7;
    auto fs = OY::PollardRho::get_factors<true>(B);
    for (auto f : fs) {
        cout << B << " % " << f << " = " << B % f << endl;
    }
}
```

```
#输出如下
2^1
3^2
5^2
95986273^1
265988969^1
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

```

