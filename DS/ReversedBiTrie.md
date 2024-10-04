### 一、模板类别

​	数据结构： 从低位到高位的 `01` 字典树。

​	练习题目：

1. [P11160 【MX-X6-T6】機械生命体](https://www.luogu.com.cn/problem/P11160)

### 二、模板功能

​		本模板与 `01` 字典树的结构大致相同，区别在于， `01` 字典树是从高位到低位插入数字，而本模板为从低位到高位插入数字。正因为如此，本模板无法执行贪心取最大异或、最小异或的任务，但是可以执行最长相同二进制低位的任务。


#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标类型。
   
   模板参数 `typename Key` ，表示字典树中的数字类型。

   模板参数 `size_t L` ，表示对每个数字的 `[0, L)` 位，从高位到低位进行考虑。
   
   模板参数 `typename Info` ，表示结点维护的信息的类。 
   
   模板参数 `template<typename>typename BufferType` ，表示内存池类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注
    
   在基础模板的基础上，特殊地建立一个 `CountTree` ，可以更加方便地添加一个数字、删除一个数字。

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
   
   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

#### 8.根据低位二进制分割树(split_by_lowbits)

1. 数据类型

   输入参数 `Key key` ，表示要查询的数字。
   
   输入参数 `size_type low_length` ，表示要根据 `key` 的最低的多少个位进行分割。

   返回类型 `tree_type` ，表示分割出的树。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数将满足特定二进制低位要求的元素分割出来。

#### 9.根据低位二进制进行贪心查询(find_greedy)

1. 数据类型

   输入参数 `Key key` ，表示要查询的数字。

   返回类型 `std::pair<node*, size_type>` ，前者为查询到的子树的根结点，后者为该子树的元素与 `key` 的保持相同的二进制低位长度。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   本函数贪心获取出一棵子树，这个子树的元素满足，与 `key` 拥有最长的相同的二进制低位。

#### 10.全局加(globally_add)

1. 数据类型

   输入参数 `Key inc` ，表示要加的数值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数令树内的每个元素的键值都增加 `inc` ，当溢出时，相当于对 `pow(2, L)` 取模。
   
#### 11.合并(merge)

1. 数据类型

   输入参数 `tree_type &rhs` ，表示要合并的另一棵树。
   
   输入参数 `Func &&func` ，表示同一位置的结点合并时调用的回调函数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数执行后， `rhs` 会被清空。
   
#### 12.枚举每个数字(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示要对每个叶子结点及其代表的数字调用的回调函数。

2. 时间复杂度

    $O(n\cdot L)$ 。

### 三、模板示例

```c++
#include "DS/ReversedBiTrie.h"
#include "IO/FastIO.h"

int A[] = {4, 6, 9, 2, 3, 2, 4, 4, 10, 5};
void test_normal() {
    using BiTrie = OY::REVBITRIE::Tree<uint32_t, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert(a);
        cout << S << endl;
    }

    // 查询是否包含某元素
    if (S.contains(1))
        cout << "S contains 1\n";
    else
        cout << "S doesn't contain 1\n";
    if (S.contains(7))
        cout << "S contains 7\n";
    else
        cout << "S doesn't contain 7\n";

    cout << endl;
}

void test_info() {
    // 通过自定义 info 可以维护自定义信息
    // 比如这个 info 可以维护最大出现次数
    struct info {
        int m_max_cnt;
    };
    using BiTrie = OY::REVBITRIE::Tree<uint32_t, 5, info>;

    BiTrie S;
    for (int a : A) {
        auto ptr = S.insert(a);
        int cnt = ++(ptr->m_max_cnt);
        S.trace(a, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
        cout << S << endl;
    }

    // 查询模 4 余 2 的元素的最大出现次数
    // 两种写法

    // 第一种
    auto S2 = S.split_by_lowbits(2, 2);
    // 先把 {2,6} 分割出来
    cout << "S2 = " << S2 << endl;
    // 但是因为这个模板是没有 pushup 动作的，所以直接查询 root() 是不对的，需要手动更新一下
    S2.trace(2, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->lchild()->m_max_cnt, p->rchild()->m_max_cnt); });
    auto res = S2.root()->m_max_cnt;
    S.merge(S2);
    cout << "max count = " << res << endl;

    // 第二种直接在原树找
    auto sub = S.find_greedy(2, 2);
    if (sub.second) {
        cout << "max count = " << sub.first->m_max_cnt << endl;
    }

    cout << endl;
}

void test_erasable() {
    // 统计某个字符串的出现次数，用次数做差来作为搜索依据
    using BiTrie = OY::REVBITRIE::CountTree<uint32_t, int, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert_one(a);
        cout << S << endl;
    }

    // 把模 8 余 2 的元素分割出来
    auto S2 = S.split_by_lowbits(2, 3);
    cout << S2 << endl;
    // 将其键值全部加 2
    S2.globally_add(2);
    cout << S2 << endl;
    // 合并回 S，查询有几个 4
    S.merge(S2);
    auto it = S.find_greedy(4);
    if (it.second == 5) {
        cout << "count of 4 : " << it.first->count() << endl;
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
{4,6}
{4,6,9}
{4,2,6,9}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,10,6,9,3}
{4,2,10,6,9,5,3}
S contains 1
S contains 7

{4}
{4,6}
{4,6,9}
{4,2,6,9}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,10,6,9,3}
{4,2,10,6,9,5,3}
S2 = {2,10,6}
max count = 2
max count = 2

{4*1}
{4*1,6*1}
{4*1,6*1,9*1}
{4*1,2*1,6*1,9*1}
{4*1,2*1,6*1,9*1,3*1}
{4*1,2*2,6*1,9*1,3*1}
{4*2,2*2,6*1,9*1,3*1}
{4*3,2*2,6*1,9*1,3*1}
{4*3,2*2,10*1,6*1,9*1,3*1}
{4*3,2*2,10*1,6*1,9*1,5*1,3*1}
{2*2,10*1}
{4*2,12*1}
count of 4 : 5

```

