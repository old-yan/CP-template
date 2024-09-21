### 一、模板类别

​	数据结构： 基于无旋树堆的 `Counter` 。

​	练习题目：

1. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
2. [P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
3. [P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
4. [P8496 [NOI2022] 众数](https://www.luogu.com.cn/problem/P8496)
5. [P9233 [蓝桥杯 2023 省 A] 颜色平衡树](https://www.luogu.com.cn/problem/P9233)
6. [Point Set Range Frequency](https://judge.yosupo.jp/problem/point_set_range_frequency)(https://github.com/yosupo06/library-checker-problems/issues/769)


### 二、模板功能

#### 1.建立表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Key` ，表示键类型。

   模板参数 `typename Mapped` ，表示值类型。
   
   模板参数 `bool RangeQuery` ，表示是否进行区间查询。
   
   模板参数 `bool MaintaiSize` ，表示是否维护表的大小。
   
   模板参数 `template <typename> typename BufferType` ，表示内存池类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `Counter` 处理的问题为元素数量维护。本模板类似 `python` 中的 `Counter` ，可以进行增加减少元素数量、拷贝、合并等操作。
    
   本模板功能严格强于 `python` 中的 `Counter` ，因为本模板的键为有序排列。当 `RangeUpdate` 为 `true` 时，可以对一定范围内的键的数量和进行查询；此外，还可以根据对键设立阈值将一个表分割成两个表。
   
   本模板基于无旋树堆维护元素数量。
   
   在维护元素数量时，遇到元素数量变为零时会自动将元素删除。

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

   输入参数 `Key key` ，表示单点增加的键。

2. 时间复杂度

   $O(\log n)$ 。

#### 7.单点查询(query)

1. 数据类型

   输入参数 `Key key` ，表示查询的键。

2. 时间复杂度

   $O(\log n)$ 。


#### 8.区间查询(query)

1. 数据类型

   输入参数 `Key key_low​` ，表示区间查询的开头下标。

   输入参数 `Key key_high​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

#### 9.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.查询第k个元素(kth)

1. 数据类型

   输入参数 `Mapped k` ，表示要查询第 `k` 个元素。
   
   返回类型 `const node*` ，表示查询到的叶结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法的元素排名基于 `0` ，当传递 `k==0` 时，表示查询最小的元素。
   
   本方法不对入参进行检查，须自行保证 `k<query_all()` 。
   
#### 11.合并(merge)

1. 数据类型

   输入参数 `table_type &rhs` ，表示要被合并掉的表。

2. 时间复杂度

   均摊 $O(\log n)$ 。
   
3. 备注
   
   操作之后， `rhs` 失效。
   
#### 12.按键分裂(split_by_key)

1. 数据类型

   输入参数 `Key key` ，表示要按之分裂的键阈值。
   
   返回类型 `table_type` ，表示分裂出的第二张表。

2. 时间复杂度

    $O(\log n)$ 。
   
3. 备注
   
   操作之后，本表失去后面的一部分；返回后面的一部分形成的表。
   
   分裂标准为，键小于 `key` 的留在原表；键大于等于 `key` 的去新表里。
   
   本方法仅当 `MaintainSize` 为 `false` 时可用，因为当分裂时无法维护正确的表的大小。
   
#### 13.对每对键值调用回调(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要调用的回调。

2. 时间复杂度

    $O(n)$ 。
   
3. 备注
   
   本方法会对树进行遍历，并在每个叶结点处调用 `call(key, cnt)` 。
   
### 三、模板示例

```c++
#include "DS/FHQCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::FHQCNT::Table<uint32_t, int, false, false>;
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
    using Counter = OY::FHQCNT::Table<uint64_t, int, true, true>;
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
    cout << "S.kth(0) = " << S.kth(0)->m_key << endl;
    cout << "S.kth(199) = " << S.kth(199)->m_key << endl;
    cout << "S.kth(200) = " << S.kth(200)->m_key << endl;
    cout << "S.kth(317) = " << S.kth(317)->m_key << endl;
    cout << "S.kth(318) = " << S.kth(318)->m_key << endl;
    cout << "S.kth(367) = " << S.kth(367)->m_key << endl
         << endl;
}

int main() {
    test();
    test_range_query();
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

```

