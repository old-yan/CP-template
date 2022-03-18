### 一、模板类别

​	数学：二次剩余算法。

### 二、模板功能

   本模板用于判定二次剩余，以及求解同余平方根。需要注意的是，本模板只解决模数为质数的情况。

#### 1.判定二次剩余

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   输入参数 `_ModType a` ，表示要判定的数值。

   输入参数 `_ModType P` ，表示模数。限定为质数。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指模数大小。

3. 备注

   本模板用于判定二次剩余。即设 `x ^ 2 ≡ a mod P` ，已知 `a,P` ，本模板可以判断是否存在合理的 `x` 。

#### 2.求解二次剩余的根

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   输入参数 `_ModType a` ，表示要求解的数值。

   输入参数 `_ModType P` ，表示模数。限定为质数。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指模数大小。

3. 备注

   本模板用于求解二次剩余的根。即设 `x ^ 2 ≡ a mod P` ，已知 `a,P` ，本模板可以求出最小的 `x` 。若不存在，返回 `0` 。
   
   **注意：** 若已知 `x ^ 2 ≡ a mod P`，易得 `(P - x) ^ 2 ≡ a mod P`，这是一对根。本模板只解出较小的那个根。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Cipolla.h"

int main() {
    uint32_t a = 2;
    uint32_t P = 7;
    auto x = OY::Cipolla32(a, P);
    cout << x << " * " << x << " ≡  2 mod 7\n";
    cout << P - x << " * " << P - x << " ≡  2 mod 7\n";

    uint32_t a2 = 3;
    bool can = OY::isQuadraticResidue(a2, P);
    if (can) {
        cout << "3 is quadratic residue mod 7\n";
    } else {
        cout << "3 is not quadratic residue mod 7\n";
    }
}
```

```
#输出如下
3 * 3 ≡  2 mod 7
4 * 4 ≡  2 mod 7
3 is not quadratic residue mod 7

```

