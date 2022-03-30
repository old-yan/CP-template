### 一、模板类别

​	数学：`Barrett` 约减。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   构造参数 `_ModType __P` ，表示模数。

2. 时间复杂度

   $O(1)$ ，包含一次除法运算。

3. 备注

   本模板用于处理以某数为除数的运算的加速。由于取余运算涉及到除法，而在同余乘法中，乘算之后紧接着取余运算，所以也可以用于加速以该数为模数的同余乘法。

   **注意：**本模板能够提高效率的情况为：模数在编译期未知，只有在运行期才知道对谁取余。如果编译期已知模数，那么本模板并不能提高效率。

   **注意：**

   要写出这个约减，需要理解它的数学原理。

   首先，模数须满足要求：

   ​	$2<=P<2^{32}$ （ `32` 位版）

   ​	$2<=P<2^{64}$ （ `64` 位版）

   接着，寻找和 $P$ 的乘积最接近 $2^{64}$ 的数：$Pinv=\lfloor \frac{2^{64}}{P} \rfloor$ ；

   注意， $Pinv$ 最后被下取整，所以 $P\cdot Pinv \in [2^{64}-P+1,2^{64}]$ 。

   不妨令 $r=2^{64}-P\cdot Pinv$ ，则有 $r\in [0,P)$ 。

   假设我们要处理的数字为 $z$ ，且 $z$ 满足 $z \in [0, 2^{64})$ 。

   我们必能找出 $z=c\cdot P + d, c\in [0, PInv], d\in [0, P)$  ，且该表示唯一。

   令 $z$ 与 $Pinv$ 相乘， $z\cdot Pinv=(c\cdot P+d)\cdot Pinv=c\cdot P\cdot Pinv+d\cdot Pinv $ 。

   替换变量，$z\cdot Pinv=c\cdot(2^{64}-r)+d\cdot Pinv=c\cdot 2^{64}+d\cdot Pinv-c\cdot r$ 。

   在此，我们需要对 $y=d\cdot Pinv-c\cdot r$ 的大小做估计。最小的情况下，$d$ 取零，$c$ 取 $Pinv$ ，$r$ 取 $P$ ，有 $y=0-Pinv\cdot P>=-2^{64}$  ；最大的情况下，$c$ 取零，$d$ 取 $P-1$ ，有 $y=(P-1)\cdot Pinv<P\cdot Pinv=2^{64}-r<=2^{64}$ 。所以 $y\in [2^{64}, 2^{64}) $ 。

   所以， $\lfloor \frac{z\cdot Pinv}{2^{64}} \rfloor=\lfloor \frac{c\cdot 2^{64}+y}{2^{64}} \rfloor$ 的取值只可能是 $c-1$ 或者 $c$ 。

#### 2.查询模数

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 3.快速求余

1. 数据类型

   输入参数 `uint64_t __a` ，表示要求余的数。要求传入非负数。

2. 时间复杂度

   $O(1)$ ，包含两次乘法运算，无除法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。

#### 4.快速乘法

1. 数据类型

   输入参数 `uint64_t __a` ，表示第一个乘数。

   输入参数 `uint64_t __b` ，表示第二个乘数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   如果 `_ModType` 为 `uint64_t` ，采取 `long double` 乘法；如果 `_ModType` 为 `uint32_t` ，采取 `64` 位乘法。

   **注意：** 因为本函数在 `64` 位下采取 `long double` 乘法，所以在 `64` 位下要求模数 `m_P` 在 `[2, 2^63)` 范围内，且 `__a*__b<m_P^2`  。

#### 5.64 位乘法

1. 数据类型

   输入参数 `uint64_t __a` ，表示第一个乘数。

   输入参数 `uint64_t __b` ，表示第二个乘数。

2. 时间复杂度

   $O(1)$ ，包含三次乘法运算，无除法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。

   **注意：** 本函数要求乘积在 `64` 位表示范围内。

#### 6. 128 位乘法

1. 数据类型

   输入参数 `uint64_t __a` ，表示第一个乘数。

   输入参数 `uint64_t __b` ，表示第二个乘数。

2. 时间复杂度

   $O(1)$ ，如果调用 `64` 位乘法，包含三次乘法运算，无除法运算；否则包含一次 `128` 位乘法运算和一次取模运算（除法运算）。

3. 备注

   如果 `__a*__b` 的结果在 `64` 位表示范围内，会转而调用快速乘法；否则转换为 `__uint128_t` 进行计算。

#### 7. long double 乘法

1. 数据类型

   输入参数 `uint64_t __a` ，表示第一个乘数。

   输入参数 `uint64_t __b` ，表示第二个乘数。

2. 时间复杂度

   $O(1)$ ，如果调用 `64` 位乘法，包含三次乘法运算，无除法运算；否则包含一次 `64` 位乘法运算，两次浮点乘算和一次浮点除算。

3. 备注

   如果 `__a*__b` 的结果在 `64` 位表示范围内，会转而调用快速乘法；否则借助 `long double` 进行计算。

   **注意：** 本函数要求模数 `m_P` 在 `[2, 2^63)` 范围内，且 `__a*__b<m_P^2`  。

   **注意：** 一般情况下本函数比 `128` 位乘法的速度更快。但是本函数不能用于 `2^63` 以上的模数，而且在 `loj` 等少数平台，本函数速度慢于 `128` 位乘法。如果有这样的情况，可以将所有用到 `multiply_ld` 的地方替换为 `multiply_128` ；或者直接将 `multiply_ld` 的实现替换为 `multiply_128` 的实现。

#### 8.快速幂

1. 数据类型

   输入参数 `uint64_t __a` ，表示底数。

   输入参数 `uint64_t __n` ，表示指数。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果 `_ModType` 为 `uint64_t` ，采取 `long double` 快速幂；如果 `_ModType` 为 `uint32_t` ，采取 `64` 位快速幂。

   **注意：** 因为本函数在 `64` 位下采取 `long double` 快速幂，所以在 `64` 位下要求模数 `m_P` 在 `[2, 2^63)` 范围内。

#### 9. 64 位快速幂

1. 数据类型

   输入参数 `uint64_t __a` ，表示底数。

   输入参数 `uint64_t __n` ，表示指数。

2. 时间复杂度

   $O(\log n)$ ，包含对数次乘法运算，无除法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。
   
   **注意：**本函数只适用于模数在 `32` 位范围内的情况。

#### 10. 128 位快速幂

1. 数据类型

   输入参数 `uint64_t __a` ，表示底数。

   输入参数 `uint64_t __n` ，表示指数。

2. 时间复杂度

   $O(\log n)$ ，包含对数次 `128` 位乘法运算，无除法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。

   在每一步乘法计算中，调用 `128` 位乘法。

#### 11. long double 快速幂

1. 数据类型

   输入参数 `uint64_t __a` ，表示底数。

   输入参数 `uint64_t __n` ，表示指数。

2. 时间复杂度

   $O(\log n)$ ，包含对数次 `long double` 乘法运算，无除法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。

   在每一步乘法计算中，调用 `long double` 位乘法。

   **注意：**本函数要求模数 `m_P` 在 `[2, 2^63)` 范围内。

#### 12.快速除法

1. 数据类型

   模板参数 `typename _Tp` ，表示被除数类型。

   输入参数 `_Tp __a` ，表示被除数。

2. 时间复杂度

   $O(1)$ ，包含两次乘法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。
   
#### 13.快速带余除法

1. 数据类型

   模板参数 `typename _Tp` ，表示被除数类型。

   输入参数 `_Tp __a` ，表示被除数。

   返回类型 `std::pair<_Tp, _Tp>` ，表示商和余数。

2. 时间复杂度

   $O(1)$ ，包含两次乘法运算。

3. 备注

   在底层运算中，除法运算耗时较大，而乘法运算耗时相对较小。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Barrett.h"

int main() {
    //设置模数为 1000000007
    OY::Barrett32 brt(1000000007);
    cout << "brt's mod number:" << brt.mod() << endl;
    cout << "1596842379856 mod 1000000007 = " << brt.mod(1596842379856) << endl;
    cout << "59865278 * 59846223 mod 1000000007 = " << brt.multiply(59865278, 59846223) << endl;
    cout << "pow(2,100) mod 1000000007 = " << brt.pow(2, 100) << endl;
    cout << "12345678987654321 // 1000000007 = " << brt.divide(12345678987654321) << endl;
    auto [quo, rem] = brt.divmod(1596842379856);
    cout << "1596842379856 = " << quo << " * 1000000007 + " << rem << endl;

    //如果想改变模数，可以重新声明一个变量，或者将原变量重新构造
    brt = OY::Barrett32(233);
    cout << "brt's mod number:" << brt.mod() << endl;
}
```

```
#输出如下
brt's mod number:1000000007
1596842379856 mod 1000000007 = 842368684
59865278 * 59846223 mod 1000000007 = 752066024
pow(2,100) mod 1000000007 = 976371285
12345678987654321 // 1000000007 = 12345678
1596842379856 = 1596 * 1000000007 + 842368684
brt's mod number:233

```
