### 一、模板类别

​	数学：原根。

  练习题目：
  
1. [P6091 【模板】原根](https://www.luogu.com.cn/problem/P6091)
2. [Primitive Root](https://judge.yosupo.jp/problem/primitive_root)(https://github.com/yosupo06/library-checker-problems/issues/851)

### 二、模板功能

​	本模板需要进行分解质因数，而分解质因数有两种不同的策略：一种是在一定的值域范围内进行筛法预处理；一种是没有筛法预处理，通过 `Pollard-Rho` 方法进行概率分解。前者在值域范围内运行速度较快（约为后者 `1.5` 倍），后者没有预处理耗时且适用的值域范围更大（可至 `10^18` ）。

   目前本模板只准备了 `Pollard-Rho` 方法。

#### 1.建立模板

1. 数据类型

   模板参数 `typename Tp` ，表示模数类。要求可以通过 `Tp::mod()` 获取模数，通过 `typename Tp::mod_type` 获取模数类的内部类型。

2. 时间复杂度

   可以认为是 $O(n^0.25)$ 。

3. 备注

   在建立模板求原根之前，最好先使用静态方法判断原根的有无。


#### 2.判断某数有没有原根(has_primitive_root)

1. 数据类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

    $O(n^0.25)$ 。

3. 备注

   本方法为静态方法；在求原根之前，最好先使用本方法判断原根的有无。

#### 3.检查是否为原根(is_primitive_root)

1. 数据类型

   输入参数 `Tp a` ，表示要检查的数字。

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(\log^2 n)$ 。

#### 4.查询最小原根(query_min)

1. 数据类型

2. 时间复杂度

   $O(n^{0.25} \cdot \log^2 n)$ 。

#### 5.查询随机原根(query_random)

1. 数据类型

2. 时间复杂度

   $O(n^{0.25} \cdot \log^2 n)$ 。

#### 6.查询原根数量(count)

1. 数据类型

2. 时间复杂度

   $O(n^{0.25})$ 。

#### 7.对所有原根调用回调(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要调用的回调。

2. 时间复杂度

   $O(n^{0.25} \cdot \log^2 n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/PollardRho.h"
#include "MATH/PrimitiveRoot.h"
#include "MATH/StaticModInt32.h"

int main() {
    static constexpr uint32_t P = 101;
    using mint = OY::StaticModInt32<P, true>;

    using Solver = OY::PROOT::PollardRhoSolver<mint, OY::PollardRho>;
    if (Solver::has_primitive_root()) {
        Solver sol;
        cout << "primitive root count = " << sol.count() << endl;
        cout << "minimal primitive root = " << sol.query_min() << endl;
        sol.enumerate([](mint proot) {
            cout << "primitive root: " << proot << endl;
        });
    }
}
```

```
#输出如下
primitive root count = 40
minimal primitive root = 2
primitive root: 2
primitive root: 8
primitive root: 27
primitive root: 7
primitive root: 28
primitive root: 11
primitive root: 75
primitive root: 98
primitive root: 89
primitive root: 53
primitive root: 40
primitive root: 59
primitive root: 34
primitive root: 35
primitive root: 55
primitive root: 18
primitive root: 72
primitive root: 86
primitive root: 63
primitive root: 50
primitive root: 99
primitive root: 93
primitive root: 74
primitive root: 94
primitive root: 73
primitive root: 90
primitive root: 26
primitive root: 3
primitive root: 12
primitive root: 48
primitive root: 61
primitive root: 42
primitive root: 67
primitive root: 66
primitive root: 46
primitive root: 83
primitive root: 29
primitive root: 15
primitive root: 38
primitive root: 51

```

