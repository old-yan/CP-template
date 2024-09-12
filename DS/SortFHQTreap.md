### 一、模板类别

​	数据结构：区间排序平衡树。

​	练习题目：

1. [P2824 [HEOI2016/TJOI2016] 排序](https://www.luogu.com.cn/problem/P2824)
2. [T258904 sort（区间排序）](https://www.luogu.com.cn/problem/T258904)
3. [草原追击](https://ac.nowcoder.com/acm/problem/257803)


### 二、模板功能

#### 1.建立区间排序平衡树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中结点下标、大小的类型。

   类型设定 `info_type = typename std::conditional<std::is_void<InfoType>::value, VoidInfo, InfoType>::type` ，表示要维护的信息类型。（由于当 `InfoType` 为 `void` 时，不维护任何信息，但此时很多函数的参数类型为 `void` 会导致无法通过编译，所以需要一个占位类型。姑且令其为 `VoidInfo` 类型）

   模板参数 `typename KeyType` ，表示键类型，要求为数字类型。

   模板参数 `typename Monoid` ，表示要维护的幺半群类型。
   
   模板参数 `typename Compare` ，表示键类型的比较函数的类型。

   构造参数 `size_type length` ，表示线段树的覆盖范围为 `[0, length)`。

   构造参数 `InitKeyMapping key_mapping` ，表示线段树中，从下标到键的映射函数。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到信息值的映射函数。

2. 时间复杂度

    $O(n)$ ，此处  `n` 指区间长度。

3. 备注

   区间排序平衡树是在平衡树的基础上实现的，但本模板并不依赖于平衡树模板。

   本模板不要求所有的键必须唯一，不同位置处的键可以重复。

   本模板通过模板参数 `typename Monoid` 确定幺半群。幺半群须满足以下要求：
   
1. 声明 `value_type` 为值类型；

2. 声明 `sum_type` 为值类型，且可以和 `value_type` 双向类型转换；

3. 定义静态函数 `op` ，接受两个 `sum_type` 参数，返回它们的聚合值；

4. 定义静态函数 `identity` ，无输入参数，返回幺元。

5. 定义静态函数 `reversed` ，输入一个 `sum_type` 参数，返回其翻转值。所谓翻转值，即若 `S1 = op(a, b, c)` ， `S2 = op(c, b, a)` ，则 `S2` 和 `S1` 互为翻转值。


   半群的 `value_type` 可以设为 `void` ，表示不维护额外信息。此时 `Monoid` 不需要有 `sum_type` ， `op` ， `identity` 以及 `reversed`  函数，本模板仅维护每个位置的键。
   
   在半群的 `value_type` 不为 `void` 时， `sum_type` 可以为 `void` 。此时；也不需要有 `op` ， `identity` 以及 `reversed`  函数，本模板仅维护每个位置的键和额外信息，但不维护区间查询信息。
   
   在半群的 `value_type` 和 `sum_type` 均不为 `void` ，且有 `op` 函数时，必须有 `identity` ，但是 `reversed` 函数为可选项。若有 `reversed` 函数，表示本幺半群可以通过 `reversed` 函数快速获取翻转值，从而避免了维护翻转值信息的时空开销；若无 `reversed` 函数，表示本幺半群无法快速计算翻转值，所以必须在模板内付出开销维护翻转值。

#### 2.建立区间排序平衡树

1. 数据类型

   本构造函数不传递 `mapping` 函数。

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   本构造函数用于 `InfoType` 为 `void` 的情况。

#### 3.查询区间大小(size)

1. 数据类型

   返回类型 `size_type` ，表示维护的区间大小。

2. 时间复杂度

    $O(1)$ 。



#### 4.单点修改(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点修改的下标。

   输入参数 `KeyType key` ，表示要修改为的键值。

   输入参数 `const info_type &val​` ，表示要修改为的信息值。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）
   
   当 `value_type` 为 `void` 时，不需要传递第三个参数。

#### 5.区间排序(sort)

1. 数据类型

   模板参数 `bool Reversed` ，表示是否为逆序排序。

   输入参数 `size_type left` ，表示区间排序的起始下标。

   输入参数 `size_type right` ，表示区间排序的结尾下标。（闭区间）

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）
   
   当 `Reversed` 为 `false` 时，按照一般习惯进行非降序/递增排序。
   
   当 `Reversed` 为 `true` 时，进行非升序/递降排序。

#### 6.获取某结点(get_node)

1. 数据类型

   输入参数 `size_type i​` ，表示要获取的结点的下标。

   返回类型 `node*` ，表示结点指针。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.单点查询(query)

1. 数据类型

   输入参数 `size_type i​` ，表示要查询的结点的下标。

   返回类型 `sum_type` ，表示查询结果。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 8.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `sum_type` ，表示查询结果。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.查询全部(query_all)

1. 数据类型

   返回类型 `sum_type` ，表示查询结果。
   
2. 时间复杂度

   $O(1)$ 。

#### 10.树上二分查询右边界(max_right)

1. 数据类型
   
   输入参数 `size_type left` ，表示左边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最大右边界。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`  ，均有 `judge(query(left, i))` 为真。而当 `i>r` 时，有 `judge(query(left, i))` 为假。显然，`r` 的最大值为 `m_size-1` 。

   如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`  。所以 `r` 的最小值为 `left-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 11.树上二分查询左边界(min_left)

1. 数据类型
   
   输入参数 `size_type right` ，表示右边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最小左边界。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right))` 为真。而当 `i<l` 时，有 `judge(query(i, right))` 为假。显然，`l` 的最小值为 `0` 。

   如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`  。所以 `l` 的最大值为 `right+1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


### 三、模板示例

```c++
#include "DS/SortFHQTreap.h"
#include "IO/FastIO.h"

void test_normal_tree() {
    // 基础的排序平衡树可以实现打乱元素，然后查询每个位置的元素
    // 先给出一个长度为 10 的排列
    int A[10] = {11, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 这棵树 value_type = void, sum_type = void
    OY::SortFHQTreap<int> S(10, [&](int i) {
        return A[i];
    });
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    S.sort<false>(3, 6);
    cout << S << endl;

    // 访问每个下标的元素
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_key << " \n"[i == 9];
    cout << endl;
}

void test_prod_tree() {
    // 区间排序平衡树可以轻易地维护区间乘积
    // 先给出一个长度为 10 的排列
    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 不妨就令每个键 v 对应的值恰为键的十倍
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i] * 10; };
    // SumType 比 ValueType 高一级，但是可以互相转换
    auto S = OY::make_SortFHQ<uint32_t, int, int64_t, 1, std::less<uint32_t>, std::plus<int64_t>>(10, {}, key_mapping, info_mapping);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl
         << endl;
}

void test_sum_tree() {
    // 区间排序平衡树可以轻易地维护区间和
    // 先给出一个长度为 10 的排列
    std::string A[10] = {"b", "a", "c", "d", "c", "a", "b", "d", "c", "a"};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    struct Monoid {
        using value_type = std::string;
        static value_type identity() { return ""; }
        static value_type op(value_type x, value_type y) { return x + y; }
    };
    // 平衡树支持非数字做元素
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i]; };
    OY::SORTFHQ::Tree<std::string, Monoid, std::less<std::string>> S(10, key_mapping, info_mapping);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;

    cout << endl;
}

int main() {
    test_normal_tree();
    test_prod_tree();
    test_sum_tree();
}
```

```
#输出如下
11 5 9 1 8 4 6 7 3 2
{{11}, {5}, {9}, {1}, {8}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8, 5, 1}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8}, {1, 4, 5, 6}, {7}, {3}, {2}}
11 9 8 1 4 5 6 7 3 2

10 5 9 1 8 4 6 7 3 2
{{10:100}, {5:50}, {9:90}, {1:10}, {8:80}, {4:40}, {6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 181
{{10:100}, {9:90, 8:80, 5:50, 1:10}, {4:40}, {6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 143
{{10:100}, {9:90}, {8:80}, {1:10, 4:40, 5:50, 6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 132

b a c d c a b d c a
{{b:b}, {a:a}, {c:c}, {d:d}, {c:c}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = dcabdc
{{b:b}, {d:d, c:c, c:c, a:a}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = caabdc
{{b:b}, {d:d, c:c}, {a:a, a:a, b:b, c:c}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = aabcdc

```

