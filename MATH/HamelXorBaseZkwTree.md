### 一、模板类别

​	数学：异或线性基线段树。

​	练习题目：

1. [P5607 [Ynoi2013] 无力回天 NOI2017](https://www.luogu.com.cn/problem/P5607)

### 二、模板功能

#### 1.构造异或线性基线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板里设计的数量类型。

   类型设定 `base_type = HamelXorBase<Tp, MAX_WIDTH>` ，表示基础的异或线性基的类型。

   模板参数 `typename Tp` ，表示异或线性基的元素类型。可以为 `uint32_t` 或者是 `uint64_t` 。

   模板参数 `size_type MAX_WIDTH` ，表示异或线性基的最大维数。

   构造参数 `size_type length` ，表示要维护的区间长度。

   构造参数 `InitMapping mapping` ，表示区间内，从编号到向量的映射。

2. 时间复杂度

   $O(n\cdot\log n\cdot\omega^2)$ ，此处 `w` 指线性基的维数。

3. 备注

   异或线性基的基础概念，在 `HamelXorBase` 模板中作了介绍。

   本模板为，当需要做区间异或修改、区间线性基查询时，加速查询的模板。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法。

#### 2.构造异或线性基线段树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到表里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示要维护的区间大小。

   输入参数 `InitMapping mapping` ，表示从结点编号到向量的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

#### 5.区间异或修改(range_xor)

1. 数据类型

   输入参数 `size_type left` ，表示要修改的区间左端的下标。

   输入参数 `size_type right` ，表示要修改的区间右端的下标。（闭区间）

   输入参数 `Tp val` ，表示对区间内的每个向量要做的异或修改。

2. 时间复杂度

    $(\log n\cdot\omega^2)$ 。

#### 6.枚举所有基向量(enumerate_base)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   输入参数 `Callback &&call` ，表示对基向量调用的回调函数。

2. 时间复杂度

    $O(\log n\cdot\omega^2)$ ，此处 `w` 指线性基的维数。

3. 备注

    本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。

    本方法可以查询从 `left` 到 `right` 区间内的向量所能形成的异或线性基的基向量。

#### 7.转为基础异或线性基(to_base_type)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   返回类型 `base_type` ，表示基础异或线性基的类型。

2. 时间复杂度

    $O(\log n\cdot\omega^2)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。

   本方法将 `left` 到 `right` 区间内的向量加入到异或线性基中，得到一个总的异或线性基。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法。

#### 8.单点向量查询(query)

1. 数据类型

   输入参数 `size_type pos` ，表示查询的下标。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

   $O(\log n)$，此处 `w` 指线性基的维数。


#### 9.查询最大异或(query_max_bitxor)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   输入参数 `Tp base` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(\log n\cdot\omega^2)$，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。
   
   本方法等价于将 `left` 到 `right` 区间内的向量加入到异或线性基中，然后在这个总的异或线性基中进行查询最大异或。
   
   本方法返回选择 `base` 以及异或线性基中的若干个基向量进行异或计算，所得的最大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/HamelXorBaseZkwTree.h"

#include <bitset>

int main() {
    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseZkwTree32<10> hxb(10, [](int i) { return 1 << i; });

    // 看看区间 [0, 2] 的线性基
    hxb.enumerate_base(0, 2, [](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 现在我们给区间 [2, 3] 异或上 0b1000
    hxb.range_xor(2, 3, 0b1000);
    
    // 看看区间 [0, 2] 的线性基
    hxb.enumerate_base(0, 2, [](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;
}
```

```
#输出如下
base in [0, 2]: 0000000001
base in [0, 2]: 0000000011
base in [0, 2]: 0000000110

base in [3, 5]: 0000001000
base in [3, 5]: 0000011000
base in [3, 5]: 0000110000

base in [0, 2]: 0000000001
base in [0, 2]: 0000000011
base in [0, 2]: 0000001110

base in [3, 5]: 0000010000
base in [3, 5]: 0000110000

```

