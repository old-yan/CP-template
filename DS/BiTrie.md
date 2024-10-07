### 一、模板类别

​	数据结构： `01` 字典树。

​	练习题目：

1. [1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
2. [1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)
3. [P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551)
4. [U109895 [HDU4825]Xor Sum](https://www.luogu.com.cn/problem/U109895)
5. [U109897 [HDU5536]Chip Factory](https://www.luogu.com.cn/problem/U109897)
6. [U109923 [Codechef REBXOR]Nikitosh and xor](https://www.luogu.com.cn/problem/U109923)
7. [Set Xor-Min](https://judge.yosupo.jp/problem/set_xor_min)(https://github.com/yosupo06/library-checker-problems/issues/499)

### 二、模板功能


#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标类型。
   
   模板参数 `typename Key` ，表示字典树中的数字类型，默认为 `uint32_t` 。

   模板参数 `size_t L` ，表示对每个数字的 `[0, L)` 位，从高位到低位进行考虑。默认为 `30` 。
   
   模板参数 `typename Info` ，表示结点维护的信息的类。 

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `01` 字典树本质和线段树一样，无非是在树中下降时，走左路还是走右路的选择比较自由。
    
   在基础模板的基础上，特殊地建立一个 `CountTree` ，可以更加方便地添加一个数字、删除一个数字，并且支持求第 `k` 大异或、求某异或结果的排名等等。

#### 2.查询是否为空(empty)

1. 数据类型

   返回类型 `bool*` ，表示字典树是否为空。

2. 时间复杂度

   $O(1)$ 。
   
#### 3.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。
   
#### 4.插入元素(insert)

1. 数据类型

   输入参数 `Key number` ，表示要插入的数字。

   输入参数 `Modify &&modify` ，表示从根到叶，对一路上的结点所做的操作。

   返回类型 `node*` ，表示插入元素后的叶结点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。
   
3. 备注

    `modify` 操作会对从根到叶的每个结点按顺序执行（包括叶结点，不包括根结点）。

#### 5.删除叶子结点(erase)

1. 数据类型

   输入参数 `Key number` ，表示要删除的值。

   输入参数 `Judger &&judge` ，表示对叶子结点进行删除之前进行的检查。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数会从叶子一路上溯，一旦发现另一侧的孩子为空，则继续上溯删除；否则退出。
   
   如果树中无指定数字，则不做删除。
   
   **注意：**

    `judger` 参数提供了对叶子结点是否删除的一个条件判断。例如，当插入多个重复的数字，而只删除一个该数字时，并不会把叶子结点删除，而只会把叶子结点上的标记减一；直到叶子结点上的标记降到零，才会真正把这个叶子结点删除。

#### 6.追溯(trace)

1. 数据类型

   输入参数 `Key number` ，表示要追溯的值。

   输入参数 `Modify &&modify` ，表示从叶到根，对一路上的结点执行的回调。默认为 `Ignore` ，表示不做操作。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

    `modify` 操作会对从叶到根的每个结点按顺序执行（不包括叶结点，包括根结点）。

   如果树中无指定数字，会从最接近的祖先结点处上溯。

#### 7.查询是否包含某数字(contains)

1. 数据类型

   输入参数 `Key x` ，表示要查询的数字。
   
   返回类型 `const node*` ，表示查询到的结点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。
   
3. 备注

   当找不到时，返回的并非是 `nullptr` ，而需要通过 `is_null` 判断是否空结点。

#### 8.查询最小异或(min_bitxor)

1. 数据类型

   输入参数 `Key number` ，表示要查询的数字。

   输入参数 `Judger &&judge` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseQueryJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<node*, Key>` ，前者为适配的叶结点指针，后者为适配情况下的逐位判定相同与否的 `mask` 。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中至少含有一个可选的元素。

#### 9.查询最大按位异或(max_bitxor)

1. 数据类型

   输入参数 `Key number` ，表示要查询的数字。

   输入参数 `Judger &&judge` ，表示每次面临左右孩子分叉时，最佳分叉的检查条件。当检查通过时，选择最佳分叉往下走；不通过时，选择较差分叉往下走。默认为 `BaseQueryJudger` ，表示只要分叉非空即可。

   返回类型 `std::pair<node*, Key>` ，前者为适配的叶结点指针，后者为适配情况下的按位异或值。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

#### 10.枚举每个数字(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要对每个叶子结点及其代表的数字调用的回调函数。

2. 时间复杂度

    $O(n\cdot L)$ 。

### 三、模板示例

```c++
#include "DS/BiTrie.h"
#include "IO/FastIO.h"

int A[] = {4, 6, 9, 2, 3, 5, 4, 4, 1, 5};
void test_normal() {
    using BiTrie = OY::BiTrie::Tree<uint32_t, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert(a);
        cout << S << endl;
    }

    // 查询是否包含某元素
    if (!S.contains(1)->is_null())
        cout << "S contains 1\n";
    else
        cout << "S doesn't contain 1\n";
    if (!S.contains(7)->is_null())
        cout << "S contains 7\n";
    else
        cout << "S doesn't contain 7\n";

    // 查询与 8 的最大异或：6(00110) 和 8(01000) 最大程度不同
    auto res3 = S.max_bitxor(8).second;
    cout << (res3 ^ 8) << " xor 8 = " << res3 << endl;

    // 查询与 6 的最大异或：9(01001) 和 6(00110) 最大程度不同
    auto res4 = S.max_bitxor(6).second;
    cout << (res4 ^ 6) << " xor 6 = " << res4 << endl
         << endl;
}

void test_info() {
    // 通过自定义 info 可以维护自定义信息
    // 比如这个 info 可以维护最大出现次数
    struct info {
        int m_max_cnt;
    };
    using BiTrie = OY::BiTrie::Tree<uint32_t, 5, info>;

    BiTrie S;
    for (int a : A) {
        auto ptr = S.insert(a);
        int cnt = ++(ptr->m_max_cnt);
        S.trace(a, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
        cout << S << endl;
    }

    // 查询出现一次或一次以上的元素与 6 的最大异或：  9(01001) 和 6(00110) 最大程度不同
    auto judge1 = [](BiTrie::node *p) { return p->m_max_cnt >= 1; };
    auto res1 = S.max_bitxor(6, judge1).second;
    cout << (res1 ^ 6) << " xor 6 = " << res1 << endl;

    // 查询出现两次或两次以上的元素与 6 的最大异或：  5(00101) 和 6(00110) 最大程度不同
    auto judge2 = [](BiTrie::node *p) { return p->m_max_cnt >= 2; };
    auto res2 = S.max_bitxor(6, judge2).second;
    cout << (res2 ^ 6) << " xor 6 = " << res2 << endl;

    // 查询出现三次或三次以上的元素与 6 的最大异或：  4(00100) 和 6(00110) 最大程度不同
    auto judge3 = [](BiTrie::node *p) { return p->m_max_cnt >= 3; };
    auto res3 = S.max_bitxor(6, judge3).second;
    cout << (res3 ^ 6) << " xor 6 = " << res3 << endl
         << endl;
}

void test_erasable() {
    // 统计某个字符串的出现次数，用次数做差来作为搜索依据
    using BiTrie = OY::BiTrie::CountTree<uint32_t, int, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert_one(a);
        cout << S << endl;
    }

    // 查询与 6 的最小异或
    auto res = S.min_bitxor(6).second;
    cout << (res ^ S._mask() ^ 6) << " xnor 6 = " << res << endl;

    // 查询与 6 的最大异或
    res = S.max_bitxor(6).second;
    cout << (res ^ 6) << " xor 6 = " << res << endl;

    // 可以求出第 k 大异或
    // 注意，此处的 k 大异或是不去重的
    cout << "kth:\n";
    for (int k = 0; k < S.root()->count(); k++) {
        auto res = S.kth_bitxor(6, k).second;
        cout << "No." << k << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
    }

    // 甚至，还可以求出某个异或值是第几大异或
    // 注意，此处的 k 大异或是不去重的
    // 假如题目要求，查询与 6 的异或是排第几
    cout << "rank:\n";
    for (int res = 0; res <= S._mask(); res++)
        if (!S.contains(res ^ 6)->is_null()) {
            auto rnk = S.rank_bitxor(6, res);
            cout << "No." << rnk << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
        }
}

int main() {
    test_normal();
    test_info();
    test_erasable();
}
```

```
#输出如下
{4}
{4, 6}
{4, 6, 9}
{2, 4, 6, 9}
{2, 3, 4, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
S contains 1
S doesn't contain 7
6 xor 8 = 14
9 xor 6 = 15

{4}
{4, 6}
{4, 6, 9}
{2, 4, 6, 9}
{2, 3, 4, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
9 xor 6 = 15
5 xor 6 = 3
4 xor 6 = 2

{4*1}
{4*1, 6*1}
{4*1, 6*1, 9*1}
{2*1, 4*1, 6*1, 9*1}
{2*1, 3*1, 4*1, 6*1, 9*1}
{2*1, 3*1, 4*1, 5*1, 6*1, 9*1}
{2*1, 3*1, 4*2, 5*1, 6*1, 9*1}
{2*1, 3*1, 4*3, 5*1, 6*1, 9*1}
{1*1, 2*1, 3*1, 4*3, 5*1, 6*1, 9*1}
{1*1, 2*1, 3*1, 4*3, 5*2, 6*1, 9*1}
25 xnor 6 = 0
9 xor 6 = 15
kth:
No.0: 6 ^ 6 = 0
No.1: 4 ^ 6 = 2
No.2: 4 ^ 6 = 2
No.3: 4 ^ 6 = 2
No.4: 5 ^ 6 = 3
No.5: 5 ^ 6 = 3
No.6: 2 ^ 6 = 4
No.7: 3 ^ 6 = 5
No.8: 1 ^ 6 = 7
No.9: 9 ^ 6 = 15
rank:
No.0: 6 ^ 6 = 0
No.1: 4 ^ 6 = 2
No.4: 5 ^ 6 = 3
No.6: 2 ^ 6 = 4
No.7: 3 ^ 6 = 5
No.8: 1 ^ 6 = 7
No.9: 9 ^ 6 = 15

```

