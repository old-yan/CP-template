### 一、模板类别

​	数学：异或线性基。

​	练习题目：

1. [XOR](https://acm.hdu.edu.cn/showproblem.php?pid=3949)
2. [#114. k 大异或和](https://loj.ac/p/114)
3. [P3812 【模板】线性基](https://www.luogu.com.cn/problem/P3812)
4. [P3857 [TJOI2008] 彩灯](https://www.luogu.com.cn/problem/P3857)
5. [P4151 [WC2011] 最大XOR和路径](https://www.luogu.com.cn/problem/P4151)
6. [P4301 [CQOI2013] 新Nim游戏](https://www.luogu.com.cn/problem/P4301)
7. [P4570 [BJWC2011] 元素](https://www.luogu.com.cn/problem/P4301)
8. [P4869 albus就是要第一个出场](https://www.luogu.com.cn/problem/P4869)
9. [#36. 【清华集训2014】玛里苟斯](https://uoj.ac/problem/36)

### 二、模板功能

#### 1.构造异或线性基

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板里设计的数量类型。

   模板参数 `typename Tp` ，表示异或线性基的元素类型。可以为 `uint32_t` 或者是 `uint64_t` 。

   模板参数 `size_type MAX_WIDTH` ，表示异或线性基的最大维数。

   构造参数 `Tp mask` ，表示线性基的初始掩膜。

2. 时间复杂度

   $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   异或线性基的概念与线性代数中的线性相关/无关联系紧密。

   一组在异或运算上线性无关的非零向量，称为一组异或线性基。

    `n` 位二进制所能表达的元素中，一组异或线性基的最多元素数量为 `n` 。

   当 `MAX_WIDTH` 为正数时，异或线性基的维数是固定的；当 `MAX_WIDTH` 为零时，异或线性基的维数是可变的，可以通过静态方法 `set_width` 指定。

#### 2.插入线性基(insert)

1. 数据类型

   输入参数 `Tp mask` ，表示试图插入的一个向量。

   返回类型 `size_type` ，表示插入结果。

2. 时间复杂度

   $O(\omega)$ ，此处 `w` 表示向量维度大小。

3. 备注

   如果试图插入的向量，和之前已形成的线性基在异或运算上线性无关，则插入成功，并返回新插入的向量所能贡献的最高位。否则插入失败，返回 `-1` 。

#### 3.包含/可表达(contains)

1. 数据类型

   输入参数 `Tp mask` ，表示试图查找的一个向量。

   返回类型 `bool` ，表示分量是否包含在本异或线性基中。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   如果查询的向量，和之前已形成的线性基在异或运算上线性无关，则视为不包含，返回 `false` 。否则视为包含，返回 `true` 。

#### 4.基向量数量(base_count)

1. 数据类型

   返回类型 `size_type` ，表示本异或线性基中包含的基向量的数量。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

#### 5.包含/可表达的向量数量(total_count)

1. 数据类型

   返回类型 `Tp` ，表示本异或线性基中包含/可表达的向量的数量。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   显然， `total_count` 的返回值恰好等于 `pow(2, base_count())` 。

#### 6.包含/可表达的第k大向量(kth)

1. 数据类型

   输入参数 `Tp k` ，表示次序大小。

   输入参数 `Tp base = 0` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(w)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不会对入参进行合法性检查，须保证 `k < total_count()`  。

   本方法的排名基于 `0` ， `k == 0` 对应最小的向量。

   本方法返回选择 `base` 以及本异或线性基中的若干个基向量进行异或计算，所得的第 `k` 大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

#### 7.包含/可表达的第k大向量(kth_in_total)

1. 数据类型

   输入参数 `Tp k` ，表示次序大小。

   输入参数 `const Tp &total` ，表示包含/可表达的总向量数。

   输入参数 `Tp base = 0` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(w)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不会对入参进行合法性检查，须保证 `k < total_count()`  。

   本方法的排名基于 `0` ， `k == 0` 对应最小的向量。

   本方法返回选择 `base` 以及本异或线性基中的若干个基向量进行异或计算，所得的第 `k` 大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

#### 8.某向量在包含/可表达的所有向量中的大小排名(rank)

1. 数据类型

   输入参数 `Tp mask` ，表示查询的向量。

   返回类型 `Tp` ，表示查询到的排名。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不会对入参进行合法性检查，须保证 `mask` 必须为包含/可表达的向量 。

   本方法的排名基于 `0` ， `k == 0` 对应最小的向量。

   本方法返回选择本异或线性基中的若干个基向量进行异或计算，所得的结果中 `mask` 的排名。其中，其余基向量为可选（也可全部不选）。

#### 9.某向量在包含/可表达的所有向量中的大小排名(rank_in_total)

1. 数据类型

   输入参数 `Tp mask` ，表示查询的向量。

   输入参数 `const Tp &total` ，表示包含/可表达的总向量数。

   返回类型 `Tp` ，表示查询到的排名。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法不会对入参进行合法性检查，须保证 `mask` 必须为包含/可表达的向量 。

   本方法的排名基于 `0` ， `k == 0` 对应最小的向量。

   本方法返回选择本异或线性基中的若干个基向量进行异或计算，所得的结果中 `mask` 的排名。其中，其余基向量为可选（也可全部不选）。

#### 10.枚举所有基向量(enumerate_base)

1. 数据类型

   输入参数 `Callback &&call` ，表示对基向量调用的回调函数。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

#### 11.枚举所有包含/可表达的向量(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示对向量调用的回调函数。

2. 时间复杂度

    $O(n)$ ，此处 `n` 指 `total_count()` 的大小。

#### 12.查询是否绑定(is_bonded)

1. 数据类型

   输入参数 `size_type k1` ，表示某一位。

   输入参数 `size_type k2` ，表示另一位。

   返回类型 `bool` ，表示这两位是否绑定。

2. 时间复杂度

    $O(\omega)$ ，此处 `w` 指线性基的维数。

3. 备注

   本方法可以查询，在本异或线性基所包含/可表达的向量中， `k1` 和 `k2` 这两位是否一定是相同的值。

#### 13.查询最大异或(query_max_bitxor)

1. 数据类型

   输入参数 `Tp base` ，表示向量起点。

   返回类型 `Tp` ，表示查询到的向量结果。

2. 时间复杂度

    $O(\omega)$，此处 `w` 指线性基的维数。

3. 备注

   本方法返回选择 `base` 以及本异或线性基中的若干个基向量进行异或计算，所得的最大结果。其中， `base` 为必选，其余基向量为可选（也可全部不选）。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

#include <bitset>

void test_static() {
    // 维护一个最大位长为 8 的异或线性基
    OY::StaticHamelXorBase32<8> hxb{};
    // 插入向量
    hxb.insert(0b00001011);
    hxb.insert(0b01101001);
    hxb.insert(0b01101010);
    hxb.insert(0b01100001);

    // 输出基向量个数
    cout << "base count = " << hxb.base_count() << endl;

    // 枚举所有的基向量
    uint32_t index = 0;
    hxb.enumerate_base([&](uint32_t base) {
        cout << "No." << index++ << " base: " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;

    // 输出包含/可表达的向量个数
    cout << "total count = " << hxb.total_count() << endl;

    // 枚举所有的包含/可表达的向量，注意这个枚举并不按照大小顺序
    index = 0;
    hxb.enumerate([&](uint32_t mask) {
        cout << "No." << index++ << " mask: " << std::bitset<8>(mask).to_string() << endl;
    });
    cout << endl;

    // 通过第二个和第三个基向量，可以拼凑出 0b1000
    if (hxb.contains(0b1000))
        cout << "0b1000 is contained\n";
    else
        cout << "0b1000 is not contained\n";

    // 无法拼凑出 0b100
    if (hxb.contains(0b100))
        cout << "0b100  is contained\n";
    else
        cout << "0b100  is not contained\n";

    // 查询第 k 大向量
    for (int k = 0; k < hxb.total_count(); k++)
        cout << "No." << k << " mask: " << std::bitset<8>(hxb.kth(k)).to_string() << endl;
    cout << endl;

    // 查询每个向量的排名
    hxb.enumerate([&](uint32_t mask) {
        cout << "mask" << std::bitset<8>(mask).to_string() << "'s rank = " << hxb.rank(mask) << endl;
    });
    cout << endl;

    // 查询是否绑定
    // 显然，第 5 位和第 6 位永远是一样的值
    if (hxb.is_bonded(5, 6)) cout << "No.5 digit and No.6 digit is bonded\n";
    // 第 0 位和第 1 位可以是不同的值
    if (!hxb.is_bonded(0, 1)) cout << "No.0 digit and No.1 digit is not bonded\n";
    cout << endl;

    // 用的最多的，还是查询最大异或
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor()).to_string() << endl;

    // 有的时候会强制要求从某个向量处出发，找最大异或
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor(0b1000000)).to_string() << endl;

    cout << endl;
}

void test_dynamic() {
    // 动态维度的异或线性基，必须在使用前指定维度
    OY::DynamicHamelXorBase32::set_width(8);
    OY::DynamicHamelXorBase32 hxb;

    // 插入向量
    hxb.insert(0b00001011);
    hxb.insert(0b01101001);
    hxb.insert(0b01101010);
    hxb.insert(0b01100001);

    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor()).to_string() << endl;
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor(0b1000000)).to_string() << endl;
}

int main() {
    test_static();
    test_dynamic();
}
```

```
#输出如下
base count = 3
No.0 base: 01101001
No.1 base: 00001011
No.2 base: 00000011

total count = 8
No.0 mask: 00000000
No.1 mask: 00000011
No.2 mask: 00001011
No.3 mask: 00001000
No.4 mask: 01101001
No.5 mask: 01101010
No.6 mask: 01100010
No.7 mask: 01100001

0b1000 is contained
0b100  is not contained
No.0 mask: 00000000
No.1 mask: 00000011
No.2 mask: 00001000
No.3 mask: 00001011
No.4 mask: 01100001
No.5 mask: 01100010
No.6 mask: 01101001
No.7 mask: 01101010

mask00000000's rank = 0
mask00000011's rank = 1
mask00001011's rank = 3
mask00001000's rank = 2
mask01101001's rank = 6
mask01101010's rank = 7
mask01100010's rank = 5
mask01100001's rank = 4

No.5 digit and No.6 digit is bonded
No.0 digit and No.1 digit is not bonded

max bitxor = 01101010
max bitxor = 01001011

max bitxor = 01101010
max bitxor = 01001011

```

