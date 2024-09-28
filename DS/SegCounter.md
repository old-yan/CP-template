### 一、模板类别

​	数据结构： 基于线段树的 `Counter` 。

​	练习题目：

1. [1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
2. [P1903 [国家集训队] 数颜色 / 维护队列](https://www.luogu.com.cn/problem/P1903)
3. [P3157 [CQOI2011] 动态逆序对](https://www.luogu.com.cn/problem/P3157)
4. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
5. [P3759 [TJOI2017] 不勤劳的图书管理员](https://www.luogu.com.cn/problem/P3759)
6. [P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551)
7. [P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
8. [P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
9. [P8496 [NOI2022] 众数](https://www.luogu.com.cn/problem/P8496)
10. [P9233 [蓝桥杯 2023 省 A] 颜色平衡树](https://www.luogu.com.cn/problem/P9233)
11. [U109895 [HDU4825]Xor Sum](https://www.luogu.com.cn/problem/U109895)
12. [U109897 [HDU5536]Chip Factory](https://www.luogu.com.cn/problem/U109897)
13. [U109923 [Codechef REBXOR]Nikitosh and xor](https://www.luogu.com.cn/problem/U109923)
14. [速度即转发](https://ac.nowcoder.com/acm/problem/217863)
15. [parent 树上启发式合并](https://ac.nowcoder.com/acm/problem/274852)


### 二、模板功能

#### 1.建立表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Key` ，表示键类型。需要为无符号数。

   模板参数 `typename Mapped` ，表示值类型。
   
   模板参数 `bool RangeQuery` ，表示是否进行区间查询。
   
   模板参数 `bool MaintaiSize` ，表示是否维护表的大小。
   
   模板参数 `bool GloballyBitxor` ，表示是否支持全局异或。

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `Counter` 处理的问题为元素数量维护。本模板类似 `python` 中的 `Counter` ，可以进行增加减少元素数量、拷贝、合并等操作。
    
   本模板功能严格强于 `python` 中的 `Counter` ，因为本模板的键为有序排列。当 `RangeUpdate` 为 `true` 时，可以对一定范围内的键的数量和进行查询；此外，还可以根据对键设立阈值将一个表分割成两个表。
   
   本模板基于动态开点的线段树维护元素数量，但是会将树中单个延伸的边折叠起来，从而保证树的期望高度与元素数量有关，而与值域无关。换句话说，如果树中只有两三个元素，那么维护时的复杂度可以认为是 $O(1)$ 。
   
   在维护元素数量时，遇到元素数量变为零时会自动将元素删除。
   
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

#### 7.全局异或(globally_bitxor)

1. 数据类型

   输入参数 `Key xor_by` ，表示要异或的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   将树中的所有键均异或 `xor_by` 。

#### 8.单点查询(query)

1. 数据类型

   输入参数 `Key key` ，表示查询的键。

2. 时间复杂度

   $O(\log n)$ 。


#### 9.区间查询(query)

1. 数据类型

   输入参数 `Key key_low​` ，表示区间查询的开头下标。

   输入参数 `Key key_high​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `mapped` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   本方法返回键位于 `key_low` 及 `key_high` 之间的项的值的和。


#### 10.异或意义下区间查询(query_bitxor)

1. 数据类型

   输入参数 `Key key_low​` ，表示区间查询的开头下标。

   输入参数 `Key key_high​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `mapped` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键与 `xor_by` 的异或结果位于 `key_low` 及 `key_high` 之间的项的值的和。

   特别的，当 `xor_by` 为零时，本方法等价于 `query` 方法。

#### 11.前缀和查询(presum)

1. 数据类型

   输入参数 `Key key​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `mapped` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键小于等于 `key` 的项的值的和。


#### 12.异或意义下前缀和查询(presum_bitxor)

1. 数据类型

   输入参数 `Key key​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `mapped` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键与 `xor_by` 的异或结果小于等于 `key` 的项的值的和。

   特别的，当 `xor_by` 为零时，本方法等价于 `presum` 方法。

#### 13.查询第k个元素(kth)

1. 数据类型

   输入参数 `Mapped k` ，表示要查询第 `k` 个元素。
   
   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法仅当 `RangeQuery` 为 `true` 时可用。
   
   本方法的元素排名基于 `0` ，当传递 `k==0` 时，表示查询最小的元素。
   
   本方法不对入参进行检查，须自行保证 `k<query_all()` 。
   
#### 14.查询异或意义下第k个元素(kth_bitxor)

1. 数据类型

   输入参数 `Mapped k` ，表示要查询第 `k` 个元素。

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法仅当 `RangeQuery` 为 `true` 时可用。

   本方法的元素排名基于 `0` ，当传递 `k==0` 时，表示查询与 `xor_by` 的异或结果最小的元素。

   本方法不对入参进行检查，须自行保证 `k<query_all()` 。

   特别的，当 `xor_by` 为零时，本方法等价于 `kth` 方法。

#### 15.最小值查询(minimum)

1. 数据类型

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   使用本方法时请确保树为空。

   本方法返回键最小的结点。

#### 16.异或意义下最小值查询(min_bitxor)

1. 数据类型

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   使用本方法时请确保树为空。

   本方法返回键与 `xor_by` 异或结果最小的结点。

   特别的，当 `xor_by` 为零时，本方法等价于 `minimum` 方法。

#### 17.最大值查询(maximum)

1. 数据类型

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   使用本方法时请确保树为空。

   本方法返回键最大的结点。

#### 18.异或意义下最大值查询(max_bitxor)

1. 数据类型

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   使用本方法时请确保树为空。

   本方法返回键与 `xor_by` 异或结果最大的结点。

   特别的，当 `xor_by` 为零时，本方法等价于 `maximum` 方法。

#### 19.查询smaller_bound(smaller_bound)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键小于 `key` 的情况下，键最大的结点。

   若无这样的结点，返回空指针。

#### 20.查询异或意义下smaller_bound(smaller_bound_bitxor)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键和 `xor_by` 的异或结果小于 `key` 的情况下，键与 `xor_by` 异或结果最大的结点。

   若无这样的结点，返回空指针。

#### 21.查询lower_bound(lower_bound)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键大于等于 `key` 的情况下，键最小的结点。

   若无这样的结点，返回空指针。

#### 22.查询异或意义下lower_bound(lower_bound_bitxor)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键和 `xor_by` 的异或结果大于等于 `key` 的情况下，键与 `xor_by` 异或结果最小的结点。

   若无这样的结点，返回空指针。

#### 23.查询upper_bound(upper_bound)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键大于 `key` 的情况下，键最小的结点。

   若无这样的结点，返回空指针。

#### 24.查询异或意义下upper_bound(upper_bound_bitxor)

1. 数据类型

   输入参数 `Key key` ，表示要比较的键值。

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回键和 `xor_by` 的异或结果大于 `key` 的情况下，键与 `xor_by` 异或结果最小的结点。

   若无这样的结点，返回空指针。

#### 25.按键分裂(split_by_key)

1. 数据类型

   输入参数 `Key key` ，表示要按之分裂的键阈值。

   返回类型 `table_type` ，表示分裂出的第二张表。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   操作之后，本表失去后面的一部分；返回后面的一部分形成的表。

   分裂标准为，键小于 `key` 的留在原表；键大于等于 `key` 的去新表里。

   本方法仅当 `MaintainSize` 为 `false` 时可用，因为当分裂时无法维护正确的表的大小。

#### 26.异或意义下按键分裂(split_by_key_bitxor)

1. 数据类型

   输入参数 `Key key` ，表示要按之分裂的键阈值。

   输入参数 `Key xor_by` ，表示异或对象。

   返回类型 `table_type` ，表示分裂出的第二张表。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   操作之后，本表失去一部分；返回失去的一部分形成的表。

   分裂标准为，键和 `xor_by` 的异或结果小于 `key` 的留在原表；键和 `xor_by` 的异或结果大于等于 `key` 的去新表里。

   本方法仅当 `MaintainSize` 为 `false` 时可用，因为当分裂时无法维护正确的表的大小。

#### 27.合并(merge)

1. 数据类型

   输入参数 `table_type &rhs` ，表示要被合并掉的表。

2. 时间复杂度

   均摊 $O(\log n)$ 。
   
3. 备注
   
   操作之后， `rhs` 失效。
   
#### 28.对每对键值调用回调(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要调用的回调。

2. 时间复杂度

    $O(n)$ 。
   
3. 备注
   
   本方法会对树进行遍历，并在每个叶结点处调用 `call(key, cnt)` 。
   
### 三、模板示例

```c++
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::SEGCNT::Table<uint32_t, int, false, false, false>;
    Counter S1;
    S1.add(1, 200);
    S1.add(3, 100);
    S1.add(5, 50);
    cout << "S1: " << S1 << endl;

    Counter S2;
    S2.add(2, 70);
    S2.add(3, 10);
    S2.add(6, 60);
    cout << "S2: " << S2 << endl;

    S1.merge(S2);
    cout << "after merge:\n";
    cout << "S1: " << S1 << endl;
    cout << "S2: " << S2 << endl;

    S2 = S1.split_by_key(3);
    cout << "after split by 3:\n";
    cout << "S1: " << S1 << endl;
    cout << "S2: " << S2 << endl;

    // 枚举 S1 对象
    auto call = [](uint32_t k, int v) {
        cout << "S1[" << k << "] == " << v << endl;
    };
    S1.enumerate(call);
    cout << endl;
}

void test_range_query() {
    using Counter = OY::SEGCNT::Table<uint64_t, int, true, true, false>;
    Counter S;
    S.add(1, 200);
    S.add(3999999, 100);
    S.add(500000000000, 50);
    S.add(3999999, 10);
    S.add(4999999, 8);
    cout << "S: " << S << endl;

    cout << "size = " << S.size() << endl;
    cout << "tot = " << S.query_all() << endl;
    cout << "S.query(3999999, 4999999) = " << S.query(3999999, 4999999) << endl;
    cout << "S.kth(0) = " << S.kth(0)->key() << endl;
    cout << "S.kth(199) = " << S.kth(199)->key() << endl;
    cout << "S.kth(200) = " << S.kth(200)->key() << endl;
    cout << "S.kth(317) = " << S.kth(317)->key() << endl;
    cout << "S.kth(318) = " << S.kth(318)->key() << endl;
    cout << "S.kth(367) = " << S.kth(367)->key() << endl
         << endl;
}

void test_bitxor() {
    using Counter = OY::SEGCNT::Table<uint64_t, int, true, false, false>;
    Counter S;
    S.add(31, 1);
    S.add(35, 5);
    S.add(48, 10);
    S.add(55, 50);
    S.add(57, 100);
    S.add(60, 500);
    S.add(62, 1000);
    cout << "S: " << S << endl;

    // 寻找和 21 异或结果最小的结点
    auto p = S.min_bitxor(21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果最大的结点
    p = S.max_bitxor(21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果略小于 40 的结点
    p = S.smaller_bound_bitxor(40, 21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果略大于 40 的结点
    p = S.upper_bound_bitxor(40, 21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果位于 [35, 45] 的结点的值的和
    auto cnt_35_45 = S.query_bitxor(35, 45, 21);
    cout << "sum(35 <= (key^21) <=45) = " << cnt_35_45 << endl;

    // 异或意义下进行分裂
    auto S2 = S.split_by_key_bitxor(43, 21);
    // 验证 S 的键和 21 的异或，肯定小于 43
    cout << "S: " << S << endl;
    S.enumerate([](uint64_t key, int cnt) {
        cout << key << " ^ 21 = " << (key ^ 21) << endl;
    });

    // 验证 S2 的键和 21 的异或，肯定大于等于 43
    cout << "S2: " << S2 << endl;
    S2.enumerate([](uint64_t key, int cnt) {
        cout << key << " ^ 21 = " << (key ^ 21) << endl;
    });
}

int main() {
    test();
    test_range_query();
    test_bitxor();
}
```

```
#输出如下
S1: {1*200,3*100,5*50}
S2: {2*70,3*10,6*60}
after merge:
S1: {1*200,2*70,3*110,5*50,6*60}
S2: {}
after split by 3:
S1: {1*200,2*70}
S2: {3*110,5*50,6*60}
S1[1] == 200
S1[2] == 70

S: {1*200,3999999*110,4999999*8,500000000000*50}
size = 4
tot = 368
S.query(3999999, 4999999) = 118
S.kth(0) = 1
S.kth(199) = 1
S.kth(200) = 3999999
S.kth(317) = 4999999
S.kth(318) = 500000000000
S.kth(367) = 500000000000

S: {31*1,35*5,48*10,55*50,57*100,60*500,62*1000}
31 ^ 21 = 10
35 ^ 21 = 54
48 ^ 21 = 37
60 ^ 21 = 41
sum(35 <= (key^21) <=45) = 1610
S: {31*1,48*10,55*50,60*500}
31 ^ 21 = 10
48 ^ 21 = 37
55 ^ 21 = 34
60 ^ 21 = 41
S2: {35*5,57*100,62*1000}
35 ^ 21 = 54
57 ^ 21 = 44
62 ^ 21 = 43

```

