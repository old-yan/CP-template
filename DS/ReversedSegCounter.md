### 一、模板类别

​	数据结构： 逆向线段树 `Counter` 。

​	练习题目：

1. [#3303. 「联合省选 2020 A」树](https://loj.ac/p/3303)
2. [P6018 [Ynoi2010] Fusion tree](https://www.luogu.com.cn/problem/P6018)
3. [P6623 [省选联考 2020 A 卷] 树](https://www.luogu.com.cn/problem/P6623)
4. [记忆](https://ac.nowcoder.com/acm/problem/274793)


### 二、模板功能

#### 1.建立表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Key` ，表示键类型。需要为无符号数。

   模板参数 `typename Mapped` ，表示值类型。
   
   模板参数 `bool MaintainSize` ，表示是否维护表的大小。
   
   模板参数 `bool MaintainGlobalInfo` ，表示是否维护所有键的异或和，以及所有值的和。
   
   模板参数 `bool GloballyModify` ，表示是否支持全局异或/加一/减一。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板与 `SegCounter` 的区别是， `SegCounter` 是从数字的高位到低位，从根到叶把数字插入树中； `ReversedSegCounter` 是从数字的低位到高位，从根到叶把数字插入树中。
   
   正因如此，本模板内无法基于贪心进行最大异或匹配，也无法在值域区间上进行区间查询。但相应的，本模板具备将全局的键同时加一/减一的操作能力。
   
   当 `Key` 为 `uint32_t` 时，键的值域为 `[0, 2^31)` ；当 `Key` 为 `uint64_t` 时，键的值域为 `[0, 2^63)` 。

#### 2.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(n)$ 。

#### 3.查询是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示表是否为空。

2. 时间复杂度

   $O(1)$ 。

#### 4.查询表大小(size)

1. 数据类型

   返回类型 `size_type` ，表示表的大小。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法仅当 `MaintainSize` 为 `true` 时可用。

#### 5.增加若干个某元素(add)

1. 数据类型

   输入参数 `Key key` ，表示单点增加的键。

   输入参数 `Mapped inc​` ，表示增加的值。

2. 时间复杂度

   $O(\log n)$ 。
   
#### 6.移除某元素(remove)

1. 数据类型

   输入参数 `Key key` ，表示移除的键。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   若该键在树中，则将其移除。

#### 7.单点查询(query)

1. 数据类型

   输入参数 `Key key` ，表示查询的键。

2. 时间复杂度

   $O(\log n)$ 。


#### 8.查询某键所在结点(find)

1. 数据类型

   输入参数 `Key key​` ，表示要查找的键。

   返回类型 `node*` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   若未找到该键，返回空结点。

#### 9.全局异或(globally_bitxor)

1. 数据类型

   输入参数 `Key xor_by` ，表示要异或的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   将树中的所有键均异或 `xor_by` 。

#### 10.全局加一(globally_plus_one)

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   将树中的所有键均加一。
   
   在 `Key` 为 `uint32_t` 时，键 `0x7fffffff(2147483647)` 会转变为 `0` ；在 `Key` 为 `uint64_t` 时，键 `0x7fffffffffffffff` 会转变为 `0` ；若有特殊需求可以先删除此结点，并特殊处理。

#### 11.全局减一(globally_minus_one)

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   将树中的所有键均减一。
   
   在 `Key` 为 `uint32_t` 时，键 `0` 会转变为 `0x7fffffff(2147483647)` ；在 `Key` 为 `uint64_t` 时，键 `0` 会转变为 `0x7fffffffffffffff` ；若有特殊需求可以先删除此结点，并特殊处理。

#### 12.合并(merge)

1. 数据类型

   输入参数 `table_type &rhs` ，表示要被合并掉的表。

2. 时间复杂度

   均摊 $O(\log n)$ 。
   
3. 备注
   
   操作之后， `rhs` 失效。
   
#### 13.对每对键值调用回调(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要调用的回调。

2. 时间复杂度

    $O(n)$ 。
   
3. 备注
   
   本方法会对树进行遍历，并在每个叶结点处调用 `call(key, cnt)` 。
   
### 三、模板示例

```c++
#include "DS/ReversedSegCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::REVSEGCNT::Table<uint32_t, int, true, true, true>;
    Counter S1;
    S1.add(1, 201);
    S1.add(3, 103);
    S1.add(5, 55);
    cout << "S1: " << S1 << endl;
    cout << "key xor sum = " << S1.key_xorsum() << endl;
    cout << "mapped sum = " << S1.mapped_sum() << endl;

    Counter S2;
    S2.add(2, 71);
    S2.add(3, 11);
    S2.add(6, 60);
    cout << "S2: " << S2 << endl;
    cout << "key xor sum = " << S2.key_xorsum() << endl;
    cout << "mapped sum = " << S2.mapped_sum() << endl;

    S1.merge(S2);
    cout << "after merge:\n";
    cout << "S1: " << S1 << endl;
    cout << "key xor sum = " << S1.key_xorsum() << endl;
    cout << "mapped sum = " << S1.mapped_sum() << endl;
    cout << "S2: " << S2 << endl;
    cout << "key xor sum = " << S2.key_xorsum() << endl;
    cout << "mapped sum = " << S2.mapped_sum() << endl;

    // 枚举 S1 对象
    auto call = [](uint32_t k, int v) {
        cout << "S1[" << k << "] == " << v << endl;
    };
    S1.enumerate(call);

    S1.globally_bitxor(15);
    cout << "after bitxor by 15:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_plus_one();
    cout << "after plus one:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_bitxor(15);
    cout << "after bitxor by 15:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_minus_one();
    cout << "after minus one:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);
    cout << endl;
}

void test_node_fetch() {
    // 可以根据结点指针，获取其最新值
    using Counter = OY::REVSEGCNT::Table<uint32_t, int, true, true, true>;
    Counter S;
    S.add(1, 200);
    S.add(3, 100);
    S.add(5, 50);

    auto it = S.find(3);
    cout << "it->key() = " << it->key() << endl;
    S.globally_bitxor(15);
    it->fetch();
    cout << "after bitxor by 15: it->key() = " << it->key() << endl;
    S.globally_plus_one();
    it->fetch();
    cout << "after plus one: it->key() = " << it->key() << endl;
}

int main() {
    test();
    test_node_fetch();
}
```

```
#输出如下
S1: {1*201,5*55,3*103}
key xor sum = 7
mapped sum = 359
S2: {2*71,6*60,3*11}
key xor sum = 1
mapped sum = 142
after merge:
S1: {2*71,6*60,1*201,5*55,3*114}
key xor sum = 6
mapped sum = 501
S2: {}
key xor sum = 0
mapped sum = 0
S1[2] == 71
S1[6] == 60
S1[1] == 201
S1[5] == 55
S1[3] == 114
after bitxor by 15:
S1: {12*114,10*55,14*201,9*60,13*71}
S1[12] == 114
S1[10] == 55
S1[14] == 201
S1[9] == 60
S1[13] == 71
after plus one:
S1: {10*60,14*71,13*114,11*55,15*201}
S1[10] == 60
S1[14] == 71
S1[13] == 114
S1[11] == 55
S1[15] == 201
after bitxor by 15:
S1: {0*201,4*55,2*114,1*71,5*60}
S1[0] == 201
S1[4] == 55
S1[2] == 114
S1[1] == 71
S1[5] == 60
after minus one:
S1: {0*71,4*60,1*114,3*55,2147483647*201}
S1[0] == 71
S1[4] == 60
S1[1] == 114
S1[3] == 55
S1[2147483647] == 201

it->key() = 3
after bitxor by 15: it->key() = 12
after plus one: it->key() = 13

```

