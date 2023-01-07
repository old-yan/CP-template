### 一、模板类别

​	数据结构：使用伸展树实现的动态数组。

### 二、模板功能

​	本数据结构分为指针版和静态数组版两个版本。本模板为静态数组版。

#### 1.构造动态数组

1. 数据类型

   模板参数 `_typename _Tp` ，表示元素类型。

   模板参数 `_typename _Wrap` ，表示封装层。本参数用于编辑元素类型，实现区间修改和查询。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本数据结构没有正式名字，我将其命名为动态数组。

   下文以 `splayarr` 代指本数据结构。
   
   与 `FHQTreap` 实现的动态数组相比，伸展树实现的动态数组效率更高。
   
   与指针版相比，静态数组版的空间效率更高；但是空间上限需要预先指定，超出后会出错。
   
   顾名思义，动态数组是数组的拓展，所以接口和功能和 `std::vector` 极为相像。动态数组的优势在于可以以对数时间复杂度在任意位置完成插入、删除的功能；而且动态数组提供了区间翻转的功能。此外，这次更新后，动态数组可以实现子数组修改、子数组查询、子数组序列化的功能。

#### 2.构造动态数组

1. 数据类型

   构造参数 `_Iterator __first` ，表示区间开始位置。

   构造参数 `_Iterator __last` ，表示区间结尾位置。（开区间）

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法以区间来进行填充数组，速度较一个一个地插入要快。

#### 3.构造动态数组

1. 数据类型

   构造参数 `uint32_t __length` ，表示数组长度。

   构造参数 `_Tp __val` ，表示初始值。默认为 `_Tp` 类的默认示例。

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法以区间来进行填充数组，速度较一个一个地插入要快。

#### 4.构造动态数组

1. 数据类型

   构造参数 `_Iterator __first` ，表示区间开始位置。

   构造参数 `_Iterator __last` ，表示区间结尾位置。（开区间）

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法以区间来进行填充数组，速度较一个一个地插入要快。

#### 5.构造动态数组

1. 数据类型

   构造参数 `const _Sequence &__seq` ，表示以它为构造源。

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法以区间来进行填充数组，速度较一个一个地插入要快。

   `_Sequence` 可以为 `std::vector<_Tp>` 或者 `std::string>` 等等。

#### 6.构造动态数组

1. 数据类型

   构造参数 `std::initializer_list<_Tp> __seq` ，表示以它构造源。

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法以区间来进行填充数组，速度较一个一个地插入要快。

#### 7.清空

1. 数据类型

2. 时间复杂度

   $O(n)$ 。


#### 8.插入元素

1. 数据类型：

   参数 `uint32_t __pos` ，表示要插入的下标位置。

   参数 `_Tp __key`​ ，表示要插入的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n]`）

#### 9.插入数组

1. 数据类型：

   参数 `uint32_t __pos` ，表示要插入的下标位置。

   参数 `splayarr &__other`​ ，表示要插入的数组。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n]`）

   在本函数执行完后，被插入的数组会被清空。

#### 10.插入数组

1. 数据类型：

   参数 `uint32_t __pos` ，表示要插入的下标位置。

   参数 `splayarr &&__other`​ ，表示要插入的数组。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n]`）

   在本函数执行完后，被插入的数组会被清空。

#### 11.更新某下标的值

1. 数据类型：

   参数 `uint32_t __pos` ，表示要更新的下标位置。

   参数 `_Tp __key`​ ，表示要更新为的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 12.删除元素

1. 数据类型：

   参数 `uint32_t __pos` ，表示要删除的下标位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 13.删除区间

1. 数据类型：

   参数 `uint32_t __left` ，表示要删除的区间的起始下标。

   参数 `uint32_t __right` ，表示要删除的区间的结尾下标。（闭区间）

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 14.从区间构建

1. 数据类型：

   输入参数 `_Iterator __first` ，表示区间开始位置。

   输入参数 `_Iterator __last` ，表示区间结尾位置。（开区间）

2. 时间复杂度

   $O(n)$ 。

#### 15.在开头插入元素

1. 数据类型：

   参数 `_Tp __key` ，表示要插入的值。

2. 时间复杂度

   $O(\log n)$ 。


#### 16.删除开头元素

1. 数据类型：

2. 时间复杂度

   $O(\log n)$ 。


#### 17.在结尾插入元素

1. 数据类型：

   参数 `_Tp __key` ，表示要插入的值。

2. 时间复杂度

   $O(\log n)$ 。

#### 18.删除结尾元素

1. 数据类型：

2. 时间复杂度

   $O(\log n)$ 。

#### 19.查找某下标的元素所在结点

1. 数据类型：

   参数 `uint32_t __pos` ，表示要查找的下标位置。

   返回类型 `node_ref` ，返回该下标对应的结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回的是该下标位置元素所在的结点的指针。

   慎对这个指针的 `key` 属性进行修改，如果进行了修改，需要同时对其调用 `update` 。如果缺少了 `update` ，可能使得区间查询结果失真。

   **注意：**由于 `splayarr` 的各种操作都可能会改变原数组结构，所以在进行后续数组操作后，禁止对本指针的 `key` 进行修改。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 20.查找开头元素

1. 数据类型

   返回类型 `node_ref` ，返回对应的结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回的是头元素所在的结点的指针。

   慎对这个指针的 `key` 属性进行修改，如果进行了修改，需要同时对其调用 `update` 。如果缺少了 `update` ，可能使得区间查询结果失真。

   **注意：**由于 `splayarr` 的各种操作都可能会改变原数组结构，所以在进行后续数组操作后，禁止对本指针的 `key` 进行修改。


#### 21.查找结尾元素

1. 数据类型

   返回类型 `node_ref` ，返回对应的结点。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法返回的是尾元素所在的结点的指针。

   慎对这个指针的 `key` 属性进行修改，如果进行了修改，需要同时对其调用 `update` 。如果缺少了 `update` ，可能使得区间查询结果失真。

   **注意：**由于 `splayarr` 的各种操作都可能会改变原数组结构，所以在进行后续数组操作后，禁止对本指针的 `key` 进行修改。


#### 22.查询数组大小

1. 数据类型

   返回类型 `uint32_t` ，表示数组大小。

2. 时间复杂度

   $O(1)$ 。


#### 23.查询数组是否为空

1. 数据类型

   返回类型 `bool` ，表示数组是否为空。

2. 时间复杂度

   $O(1)$ 。

#### 24.截取子数组

1. 数据类型

   参数 `uint32_t __left` ，表示要获取的子区间的起始下标。

   参数 `uint32_t __right` ，表示要获取的子区间的结尾下标。（闭区间）

   返回类型 `splayarr` ，即与原数组类型相同。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法是将原数组的一部分截了出来，所以调用之后，原数组会将相应区间切掉。

#### 25.子数组（用于修改）

1. 数据类型

   参数 `uint32_t __left` ，表示要观察的子区间的起始下标。

   参数 `uint32_t __right` ，表示要观察的子区间的结尾下标。（闭区间）

   返回类型 `nodesToUpdate` ，表示包含且仅包含要观察的子区间的元素的子树的根。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法对原数组的结构进行了腾挪移动，使得要观察的子数组恰好形成一棵子树，此时可以通过访问子树的根修改这个区间的信息。

   **注意：**

   1. 由于子数组所形成的子树，其根往往不是整棵树的根（除非子数组为数组本身），所以在进行修改操作之后，需要自下而上进行 `update` 。为实现这一目的，所以返回类型并非 `node_ref` 而是 `nodesToUpdate` ，这一数据结构记录了从整棵树的根到子树的根的路径，在析构时自动自下而上 `update` 。所以，请在 `nodesToUpdate` 对象析构之前进行修改。
   2. 由于 `splayarr` 的各种操作都可能会改变原数组结构，所以，在获取 `nodesToUpdate` 对象之后，请勿随意调用其他函数，改变树的形态。

#### 26.子数组（用于观察）

1. 数据类型

   参数 `uint32_t __left` ，表示要观察的子区间的起始下标。

   参数 `uint32_t __right` ，表示要观察的子区间的结尾下标。（闭区间）

   返回类型 `node_ref` ，表示包含且仅包含要观察的子区间的元素的子树的根。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法对原数组的结构进行了腾挪移动，使得要观察的子数组恰好形成一棵子树，此时可以通过访问子树的根读取这个区间的信息。

   **注意：**

   1. 本方法返回类型为 `node_ref` ，只能读，不能写。
   2. 由于 `splayarr` 的各种操作都可能会改变原数组结构，所以，在获取子树的根之后，请勿随意调用其他函数，改变树的形态。否则可能会失效。

#### 27.区间查询

1. 数据类型

   参数 `uint32_t __left` ，表示要查询的子区间的起始下标。

   参数 `uint32_t __right` ，表示要查询的子区间的结尾下标。（闭区间）

   返回类型 `auto` ，表示这一区间的查询值。

2. 时间复杂度

   $O(\log n)$ 。

#### 28.查询结点下标

1. 数据类型

   参数 `node_ref __it` ，表示要查询的结点。

   返回类型 `uint32_t` ，表示该结点的下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   **注意：** 本方法要求保证要查询的结点在数组中。否则会引发错误。


#### 29.二分插入

1. 数据类型

   参数 `_Tp __key` ，表示要插入的元素。

   参数 `_Compare __comp` ，表示具体的比较函数。默认为 `_Tp` 类的小于函数。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   调用本方法时请保证数组有序。

   如果有多个大小相等的元素，那么最新插入的元素排在最右。

#### 30.二分删除

1. 数据类型

   参数 `_Tp __key` ，表示要删除的元素。

   参数 `_Compare __comp` ，表示具体的比较函数。默认为 `_Tp` 类的小于函数。

   返回类型 `bool` ，表示是否删除成功。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   调用本方法时请保证数组有序。

   如果有多个大小相等的元素，那么删除最左侧的元素。

   如果元素不存在，返回 `false` 。

#### 31.二分查找 lower_bound

1. 数据类型

   参数 `_Tp __key` ，表示要查找的阈值。

   参数 `_Compare __comp` ，表示具体的比较函数。默认为 `_Tp` 类的小于函数。

   返回类型 `node_ref` ，表示查找到的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   调用本方法时请保证数组有序。

   本方法查找数组中第一个大于等于（也就是不小于） `__key` 的位置。

   如果数组中所有元素均小于 `__key` ，返回空值。

#### 32.二分查找 upper_bound

1. 数据类型

   参数 `_Tp __key` ，表示要查找的阈值。

   参数 `_Compare __comp` ，表示具体的比较函数。默认为 `_Tp` 类的小于函数。

   返回类型 `node_ref` ，表示查找到的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   调用本方法时请保证数组有序。

   本方法查找数组中第一个 `__key` 小于其的位置。

   如果数组中所有元素均小于等于 `__key` ，返回空值。

#### 33.合并其他数组

1. 数据类型

   输入参数 `splayarr &__other` ，即与原数组类型相同的另一数组。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法使用后，另一数组会接在本数组之后；另一数组变为空数组。

#### 34.合并其他数组

1. 数据类型

   输入参数 `splayarr &&__other` ，即与原数组类型相同的另一数组。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法使用后，另一数组会接在本数组之后；另一数组变为空数组。

#### 35.转为序列

1. 数据类型

   模板参数 `typename _Sequence` ，表示要转为的序列类型 。默认为 `std::vector<_Tp>` 。

2. 时间复杂度

   $O(n)$ 。

#### 36.转为子序列

1. 数据类型

   模板参数 `typename _Sequence` ，表示要转为的序列类型。默认为 `std::vector<_Tp>` 。

   输入参数 `uint32_t __left` ，表示要截取的子序列的起始下标。

   输入参数 `uint32_t __right` ，表示要截取的子序列的结尾下标。

2. 时间复杂度

   $O(m+\log n)$ ，这里 `m` 表示子序列长度。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `__left<=__right` ）


### 三、模板示例

```c++
#include "DS/SplayArray_static.h"
#include "IO/FastIO.h"
#include <numeric>

struct MyWrap {
    long long m_val, m_sum, m_inc;
    MyWrap() = default;
    MyWrap(long long _val) : m_val(_val), m_sum(_val), m_inc(0) {}
    long long val() const { return m_val; }
    long long info() const { return m_sum; }
    void add(long long inc, uint32_t size) {
        m_val += inc;
        m_sum += inc * size;
        m_inc += inc;
    }
    template <typename _NodeRef>
    void update(_NodeRef &__lc, _NodeRef &__rc) {
        push_down(__lc, __rc);
        m_sum = m_val;
        if (__lc) m_sum += __lc->key.m_sum;
        if (__rc) m_sum += __rc->key.m_sum;
    }
    template <typename _NodeRef>
    void push_down(_NodeRef &__lc, _NodeRef &__rc) {
        if (m_inc) {
            if (__lc) __lc->key.add(m_inc, __lc->size);
            if (__rc) __rc->key.add(m_inc, __rc->size);
            m_inc = 0;
        }
    }
};

int main() {
    // 动态数组的大部分接口都和 std::vector 一致，所以只展示不同的
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 如果不填模板参数，默认为 int 类型
    OY::SplayArray_static arr;
    arr.assign(A, A + 10);
    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    arr.insert(0, 100);
    cout << "insert 100: ";
    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    arr.erase(3);
    cout << "erase index 3: ";
    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    arr.erase(2, 4);
    cout << "erase index 2~4: ";
    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    auto sub1 = arr.sub_view(4, 6);
    for (int a : arr.to_sequence(sub1)) cout << a << ' ';
    cout << endl;
    auto sub2 = arr.sub_view(0, 1);
    for (int a : arr.to_sequence(sub2)) cout << a << ' ';
    cout << endl;

    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    auto sub3 = arr.cut(4, 6);
    for (int a : sub3.to_sequence()) cout << a << ' ';
    cout << endl;
    auto sub4 = arr.cut(0, 1);
    for (int a : sub4.to_sequence()) cout << a << ' ';
    cout << endl;

    for (int a : arr.to_sequence()) cout << a << ' ';
    cout << endl;

    for (int i = 0; i < sub3.size(); i++) sub3.update(i, i);
    cout << "assign value by index: ";
    for (int a : sub3.to_sequence()) cout << a << ' ';
    cout << endl;

    // *******************************************************************************************************
    // 默认的 VoidWrap 是无查询的
    //       SumWrap 自带区间和值查询
    //       MaxWrap 自带区间最大值查询
    //       MinWrap 自带区间最小值查询
    //       ReverseWrap 自带区间翻转修改
    // 我们来自定义个求区间 gcd 的，只具备区间查询功能，不支持区间修改
    struct gcd_wrap {
        long long operator()(long long x, long long y) const {
            return std::gcd(x, y);
        }
    };
    OY::SplayUpdateArray_static<long long, gcd_wrap> arr2{10000, 80000, 6000, 25000, 12000};
    arr2.join({400, 16000, 20000});
    for (auto a : arr2.to_sequence()) cout << a << ' ';
    cout << endl;
    auto sub5 = arr2.sub_view(1, 6);
    cout << "gcd of 1~6: " << sub5->key.m_info << endl;

    // *******************************************************************************************************
    // 我们再来自定义个支持区间 assign 的，但不存在区间查询功能
    // 本仿函数提供默认修改（也就是无修改的时候的值）
    struct no_modify_string {
        std::string operator()() const { return std::string(); }
    };
    // 本仿函数提供修改的效果
    struct modify_string {
        std::string operator()(std::string change, std::string old) const {
            if (change.size())
                return change;
            else
                return old;
        }
    };
    // 本仿函数提供两个修改的合并
    struct merge_modifies {
        std::string operator()(std::string new_change, std::string old_change) const {
            if (new_change.size())
                return new_change;
            else
                return old_change;
        }
    };
    OY::SplayModifyArray_static<std::string, std::string, no_modify_string, modify_string, merge_modifies> arr3{"hello", "cat", "dog", "world"};
    // 一定要用 sub_tree 函数来修改，而不是 sub_view
    arr3.sub_tree(1, 2)->key.add("change1");
    {
        auto sub = arr3.sub_tree(3, 3);
        sub->key.add("change2");
    }
    for (int i = 0; i < 4; i++) cout << arr3[i]->key.val() << (i == 3 ? '\n' : ' ');

    // *******************************************************************************************************

    // 当然我们也可以自定义实现既能区间修改也能区间查询的封装层
    // 注意，这里 MyWrap 只能在全局声明，不能在 main 里声明。因为在 main 里声明无法使用 template
    OY::SplayArray_static<long long, MyWrap> arr4{1, 2, 3, 4, 5, 6, 7, 8};
    arr4.sub_tree(1, 4)->key.add(10, 4);
    cout << arr4.query(2, 4) << '\n';

    //*******************************************************************************************************
    // 每当看到 pythoner 使用 sortedlist 的时候，是否有过羡慕？
    // 现在 C++er 也可以拥有 sortedlist 了
    // 而且这个 sortedlist 还可以定制额外的功能，比如求区间和
    using sortedlist = OY::SplaySumArray_static<int>;
    sortedlist arr5;
    arr5.bisect_insert(18);
    arr5.bisect_insert(15);
    arr5.bisect_insert(28);
    arr5.bisect_insert(18);
    arr5.bisect_insert(10);
    arr5.bisect_insert(18);
    arr5.bisect_insert(1);
    arr5.bisect_erase(18);
    for (auto a : arr5.to_sequence()) cout << a << ' ';
    cout << endl;
    cout << "sum of arr5[2~5] = " << arr5.query(2, 5) << endl;
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
11 5 9 12 8 4 6 15 7 7 
insert 100: 100 11 5 9 12 8 4 6 15 7 7 
erase index 3: 100 11 5 12 8 4 6 15 7 7 
erase index 2~4: 100 11 4 6 15 7 7 
15 7 7 
100 11 
100 11 4 6 15 7 7 
15 7 7 
100 11 
4 6 
assign value by index: 0 1 2 
10000 80000 6000 25000 12000 400 16000 20000 
gcd of 1~6: 200
hello change1 change1 change2
42
1 10 15 18 18 28 
sum of arr5[2~5] = 79

```

