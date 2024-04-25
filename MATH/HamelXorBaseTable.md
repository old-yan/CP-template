### 一、模板类别

​	数学：前缀异或线性基。

​	练习题目：

1. [Operation](https://acm.hdu.edu.cn/showproblem.php?pid=6579)

### 二、模板功能

#### 1.构造前缀异或线性基

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板里设计的数量类型。

   类型设定 `base_type = HamelXorBase<Tp, MAX_WIDTH>` ，表示基础的异或线性基的类型。

   模板参数 `typename Tp` ，表示异或线性基的元素类型。可以为 `uint32_t` 或者是 `uint64_t` 。

   模板参数 `size_type MAX_WIDTH` ，表示异或线性基的最大维数。

   构造参数 `size_type length` ，表示要维护的区间长度。

   构造参数 `InitMapping mapping` ，表示区间内，从编号到向量的映射。

2. 时间复杂度

   $O(n\cdot\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   异或线性基的基础概念，在 `HamelXorBase` 模板中作了介绍。

   本模板为，当需要做区间线性基查询时，加速查询的模板。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法。

#### 2.构造前缀异或线性基

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

#### 5.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基维数。

#### 6.预留空间(reserve)

1. 数据类型

   输入参数 `size_type length` ，表示预备维护的区间长度。

2. 时间复杂度

    $O(\omega\cdot n)$ ，此处 `w` 指线性基维数。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 7.在尾部添加元素(push_back)

1. 数据类型

   输入参数 `Tp mask` ，表示在末尾添加的向量元素。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基维数。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 8.移除尾部元素(pop_back)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法是为了动态收缩区间而准备的。
   
#### 9.枚举所有基向量(enumerate_base)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   输入参数 `Callback &&call` ，表示对基向量调用的回调函数。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

    本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。

    本方法可以查询从 `left` 到 `right` 区间内的向量所能形成的异或线性基的基向量。

#### 10.转为基础异或线性基(to_base_type)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   返回类型 `base_type` ，表示基础异或线性基的类型。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。

   本方法将 `left` 到 `right` 区间内的向量加入到异或线性基中，得到一个总的异或线性基。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法。

#### 11.查询最大异或(query_max_bitxor)

1. 数据类型

   输入参数 `size_type left` ，表示查询区间左端的下标。

   输入参数 `size_type right` ，表示查询区间右端的下标。（闭区间）

   输入参数 `Tp base` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(\omega)$，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `left <= right` 且二者均在 `[0, n)` 之间。
   
   本方法等价于将 `left` 到 `right` 区间内的向量加入到异或线性基中，然后在这个总的异或线性基中进行查询最大异或。
   
   本方法返回选择 `base` 以及异或线性基中的若干个基向量进行异或计算，所得的最大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTable.h"

#include <bitset>

int main() {
    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseTable32<10> hxb(10, [](int i) { return 1 << i; });

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 把 [0, 2] 的线性基单独拿出来
    auto hxb_0_to_2 = hxb.to_base_type(0, 2);
    hxb_0_to_2.enumerate_base([](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;
}
```

```
#输出如下
base in [3, 5]: 0000100000
base in [3, 5]: 0000010000
base in [3, 5]: 0000001000

base in [0, 2]: 0000000100
base in [0, 2]: 0000000010
base in [0, 2]: 0000000001

```

