### 一、模板类别

​	数学：树上异或线性基。

​	练习题目：

1. [P3292 [SCOI2016] 幸运数字](https://www.luogu.com.cn/problem/P3292)

### 二、模板功能

#### 1.构造树上异或线性基

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板里设计的数量类型。

   类型设定 `base_type = HamelXorBase<Tp, MAX_WIDTH>` ，表示基础的异或线性基的类型。

   模板参数 `typename Tp` ，表示异或线性基的元素类型。可以为 `uint32_t` 或者是 `uint64_t` 。

   模板参数 `size_type MAX_WIDTH` ，表示异或线性基的最大维数。

   构造参数 `Tree *rooted_tree` ，表示赖以构造的树。

   构造参数 `InitMapping mapping` ，表示从结点编号到向量的映射。

2. 时间复杂度

   $O(n\cdot\omega^2)$ ，此处 `w` 指线性基的维数。

3. 备注

   异或线性基的基础概念，在 `HamelXorBase` 模板中作了介绍。

   本模板为，当需要在树上做路径线性基查询时，加速查询的模板。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法。

#### 2.重置(reset)

1. 数据类型

   输入参数 `Tree *rooted_tree` ，表示赖以构造的树。

   输入参数 `InitMapping mapping` ，表示从结点编号到向量的映射。

2. 时间复杂度

   $O(n\cdot\omega^2)$ ，此处 `w` 指线性基的维数。

#### 3.枚举所有基向量(enumerate_base)

1. 数据类型

   输入参数 `size_type up` ，表示一个在上方的结点的编号。

   输入参数 `size_type down` ，表示一个在下方的结点的编号。

   输入参数 `Callback &&call` ，表示对基向量调用的回调函数。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

    本方法不做参数合法性检查，请保证 `down` 是 `up` 的子孙结点。

    本方法可以查询由 `up` 到 `down` 这两点形成的简单路径上的所有结点处的向量所能形成的异或线性基的基向量。

#### 4.转为基础异或线性基(to_base_type)

1. 数据类型

   输入参数 `size_type up` ，表示一个在上方的结点的编号。

   输入参数 `size_type down` ，表示一个在下方的结点的编号。

   返回类型 `base_type` ，表示基础异或线性基的类型。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `down` 是 `up` 的子孙结点。

   本方法将由 `up` 到 `down` 这两点形成的简单路径上的所有结点处的向量加入到异或线性基中，得到一个总的异或线性基。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法

#### 5.转为基础异或线性基(to_base_type)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

   输入参数 `size_type lca` ，表示最近公共祖先结点的编号。

   返回类型 `base_type` ，表示基础异或线性基的类型。

2. 时间复杂度

    $O(\omega^2)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `down` 是 `up` 的子孙结点。

   本方法将由 `a` 到 `b` 这两点形成的简单路径上的所有结点处的向量加入到异或线性基中，得到一个总的异或线性基。

   本模板只用于找出异或线性基，但是本模板所包含的方法并不丰富。如果想使用更丰富的功能，可以在找出一组异或线性基之后，转为 `base_type` ，再去调用 `base_type` 的丰富的成员方法

#### 6.查询最大异或(query_max_bitxor)

1. 数据类型

   输入参数 `size_type up` ，表示一个在上方的结点的编号。

   输入参数 `size_type down` ，表示一个在下方的结点的编号。

   输入参数 `Tp base` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(\omega)$，此处 `w` 指线性基的维数。

3. 备注

   本方法不做参数合法性检查，请保证 `down` 是 `up` 的子孙结点。
   
   本方法等价于将由 `up` 到 `down` 这两点形成的简单路径上的所有结点处的向量加入到异或线性基中，然后在这个总的异或线性基中进行查询最大异或。
   
   本方法返回选择 `base` 以及异或线性基中的若干个基向量进行异或计算，所得的最大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTree.h"
#include "TREE/LinkTree.h"

#include <bitset>

int main() {
    // 建树
    OY::LinkTree::Tree<bool, 1000> S(6);
    S.add_edge(2, 1);
    S.add_edge(1, 0);
    S.add_edge(1, 4);
    S.add_edge(4, 3);
    S.add_edge(4, 5);
    S.prepare(), S.set_root(2);

    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseTree32<8> hxb(&S, [](int i) { return 1 << i; });

    // 看看 1 到 5 的路径上的线性基
    hxb.enumerate_base(1, 5, [](uint32_t base) {
        cout << "base in path(1 -> 5): " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;

    // 把 3 到 5 的路径的线性基单独拿出来
    auto hxb_3_to_5 = hxb.to_base_type(3, 5, 4);
    hxb_3_to_5.enumerate_base([](uint32_t base) {
        cout << "base in path(3 -> 5): " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;
}
```

```
#输出如下
base in path(1 -> 5): 00000010
base in path(1 -> 5): 00010000
base in path(1 -> 5): 00100000

base in path(3 -> 5): 00100000
base in path(3 -> 5): 00010000
base in path(3 -> 5): 00001000

```

