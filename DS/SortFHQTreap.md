### 一、模板类别

​	数据结构：区间排序平衡树。

​	练习题目：

1. [P2824 [HEOI2016/TJOI2016] 排序](https://www.luogu.com.cn/problem/P2824)
2. [T258904 sort（区间排序）](https://www.luogu.com.cn/problem/T258904)
3. [草原追击](https://ac.nowcoder.com/acm/contest/64221/J)


### 二、模板功能

#### 1.建立区间排序平衡树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中结点下标、大小的类型。

   类型设定 `info_type = typename std::conditional<std::is_void<InfoType>::value, Ignore, InfoType>::type` ，表示要维护的信息类型。（由于当 `InfoType` 为 `void` 时，不维护任何信息，但此时很多函数的参数类型为 `void` 会导致无法通过编译，所以需要一个占位类型。姑且令其为 `Ignore` 类型）

   模板参数 `typename KeyType` ，表示键类型，要求为数字类型。

   模板参数 `typename InfoType` ，表示要维护的信息类型。
   
   模板参数 `typename Compare` ，表示键类型的比较函数的类型。

   模板参数 `MaintainType Maintain ` ，表示本模板维护的信息类型。默认为 `MAINTAIN_RANGE_REVERSE` ，表示支持反向的区间信息查询。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1<<22` 。

   构造参数 `size_type length` ，表示线段树的覆盖范围为 `[0, length)`。

   构造参数 `InitKeyMapping &&key_mapping` ，表示线段树中，从下标到键的映射函数。

   构造参数 `InitMapping &&mapping` ，表示在初始化时，从下标到信息值的映射函数。


   构造参数 `const info_type &default_info ` ，表示信息聚合时的默认值。

2. 时间复杂度

    $O(n)$ ，此处  `n` 指区间长度。

3. 备注

   区间排序线段树是在线段树的基础上实现的，但本模板并不依赖于线段树模板。

   本模板不要求所有的键必须唯一，不同位置处的键可以重复。

   模板参数 `InfoType` 可以设为 `void` ，表示不维护额外信息。此时本模板仅维护每个位置的键。如果 `InfoType` 不为 `void` ，则可维护相关区间聚合信息，本模板要求 `InfoType` 支持加法运算符，通过加法运算符进行聚合。排序线段树要求信息类型的加法运算符满足**结合律**。

   模板参数 `Maintain` 为 `MAINTAIN_NONE` 时，不维护区间信息，只能进行单点的键/值的查询；为 `MAINTAIN_RANGE` 时，维护正向的区间信息，在这种情况下，只有在信息聚合满足交换律或者从不进行任何区间递降排序操作时，才可以正确无误地进行区间信息查询；为 `MAINTAIN_RANGE_REVERSE` 时，维护正向和反向的区间信息，在这种情况下，无论信息聚合满足不满足交换律，无论是否进行区间递降排序操作，都不影响区间信息查询。

#### 2.建立区间排序线段树

1. 数据类型

   本构造函数不传递 `mapping` 函数以及 `default_info` 。

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

   输入参数 `const info_type &info​` ，表示要修改为的信息值。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）
   
   当 `InfoType` 为 `void` 时，不需要传递第三个参数。

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

   返回类型 `InfoType` ，表示查询结果。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 8.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `InfoType` ，表示查询结果。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.查询全部(query_all)

1. 数据类型

   返回类型 `InfoType` ，表示查询结果。
   
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
    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    OY::SortFHQ::Tree<int, void, std::less<int>, OY::SortFHQ::MAINTAIN_NONE, 1000> S(10, [&](int i) {
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
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->get() << " \n"[i == 9];
    cout << endl;
}

void test_sum_tree() {
    // 区间排序平衡树可以轻易地维护区间和
    // 先给出一个长度为 10 的排列
    std::string A[10] = {"b", "a", "c", "d", "c", "a", "b", "d", "c", "a"};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 平衡树支持非数字做元素
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i]; };
    OY::SortFHQ::Tree<std::string, std::string, std::less<std::string>, OY::SortFHQ::MAINTAIN_RANGE_REVERSE, 1000> S(10, key_mapping, info_mapping, "");
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

struct XorClass {
    int val;
    XorClass() = default;
    XorClass(int val) : val(val) {}
    XorClass operator+(const XorClass &rhs) const { return {val ^ rhs.val}; }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const XorClass &x) { return out << x.val; }
};
void test_xor_tree() {
    // 维护其他的运算，就需要创建新类
    // 比如想维护异或和，那就创建一个加号执行异或的类
    int keys[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    XorClass infos[10] = {1000, 500, 900, 100, 800, 400, 600, 700, 300, 200};

    // 建立区间排序平衡树
    // 不妨就令每个键 v 对应的值恰为 100 * v
    auto key_mapping = [&](int i) { return keys[i]; };
    auto info_mapping = [&](int i) { return infos[i]; };
    // 由于异或满足交换律所以 MAINTAIN_RANGE 够用了
    OY::SortFHQ::Tree<int, XorClass, std::less<int>, OY::SortFHQ::MAINTAIN_RANGE, 1000> S(10, key_mapping, info_mapping, {});
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;

    cout << endl;
}

int main() {
    test_normal_tree();
    test_sum_tree();
    test_xor_tree();
}
```

```
#输出如下
10 5 9 1 8 4 6 7 3 2
{{10}, {5}, {9}, {1}, {8}, {4}, {6}, {7}, {3}, {2}}
{{10}, {9, 8, 5, 1}, {4}, {6}, {7}, {3}, {2}}
{{10}, {9, 8}, {1, 4, 5, 6}, {7}, {3}, {2}}
10 9 8 1 4 5 6 7 3 2

b a c d c a b d c a
{{b:b}, {a:a}, {c:c}, {d:d}, {c:c}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = dcabdc
{{b:b}, {d:d, c:c, c:c, a:a}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = caabdc
{{b:b}, {d:d, c:c}, {a:a, a:a, b:b, c:c}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = aabcdc

{{10:1000}, {5:500}, {9:900}, {1:100}, {8:800}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 796
{{10:1000}, {9:900, 8:800, 5:500, 1:100}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 456
{{10:1000}, {9:900, 8:800}, {1:100, 4:400, 5:500, 6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 456


```

