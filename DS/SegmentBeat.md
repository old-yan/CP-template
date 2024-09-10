### 一、模板类别

​	数据结构：势能线段树。

​	练习题目：

1. [And or Max](https://csacademy.com/contest/round-70/task/and-or-max/statement/)
2. [Gorgeous Sequence](http://acm.hdu.edu.cn/showproblem.php?pid=5306)
3. [#6029. 「雅礼集训 2017 Day1」市场](https://loj.ac/p/6029)
4. [#6565. 最假女选手](https://loj.ac/p/6565)
5. [P3246 [HNOI2016] 序列](https://www.luogu.com.cn/problem/P3246)
6. [P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)
7. [P4314 CPU 监控](https://www.luogu.com.cn/problem/P4314)
8. [P4560 [IOI2014] Wall 砖墙](https://www.luogu.com.cn/problem/P4560)
9. [P6242 【模板】线段树 3](https://www.luogu.com.cn/problem/P6242)
10. [U180387 CTSN loves segment tree](https://www.luogu.com.cn/problem/U180387)
11. [#164. 【清华集训2015】V](https://uoj.ac/problem/164)
12. [#169. 【UR #11】元旦老人与数列](https://uoj.ac/problem/169)
13. [#170. Picks loves segment tree VIII](https://uoj.ac/problem/170)
14. [#228. 基础数据结构练习题](https://uoj.ac/problem/228)
15. [Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/243)

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树下标类型、树中结点在内存池上的下标类型。

   模板参数 `typename Node` ，表示结点类型。

   构造参数 `size_type length` ，表示线段树的覆盖范围为 `[0, length)`。

   构造参数 `InitMapping init_mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n) 。

3. 备注

   势能线段树的基础是普通线段树；如果不熟悉不了解普通线段树，则很难设计自定义功能的势能线段树。
   
   势能线段树处理的问题为单点/区间修改，单点/区间的查询。
   
   势能线段树并不一定带有懒惰标记，也就是说，势能线段树并不像普通线段树一样能够清晰地区分 `value_type` 和 `modify_type` ，实际使用势能线段树时，执行区间修改操作要想时间复杂度不退化到线性，则必须综合结点内的信息进行剪枝。

   势能线段树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让势能线段树工作。

   对于势能线段树来说，结点须满足以下要求：

   1. 实现成员函数 `set` ，接受单个参数，将值赋给本结点。本函数只在初始化时调用；
   2. 实现成员函数 `pushup` ，接受两个孩子结点的指针，聚合到当前结点。
   3. 实现成员函数 `pushdown` ，接受两个孩子结点的指针以及一个表示当前结点的区间长度的参数，将本结点的一些懒惰修改的信息传递给左右孩子结点，并清空本结点的懒惰信息。
   4. 定义静态函数 `map` ，接受修改信息、结点指针和结点的区间长度。在本函数内，尝试利用修改信息对结点做出修改；如果能够快速修改区间，就做出修改，返回 `true` ；如果不能快速修改，则不做修改，返回 `false` 。

   至此，势能线段树所需的结点功能已经足够。此外，还可以添加如下功能：

   1. 定义成员函数 `get` ，返回叶子结点的值。如果不定义本函数，则不能调用默认的单点查询；也无法直接打印整颗树。

   与普通线段树不同的，结点不存在自定义聚合操作和囤积操作的接口。聚合操作可以在 `pushup` 内完成；囤积操作可以在 `pushdown` 内完成。

   势能线段树的灵魂操作在 `map` 处。这里需要搞清楚，什么叫做“**能快速修改**”，什么叫做“**不能快速修改**”。

   假如某结点维护着一个区间的区间和，此时给此区间添加一个“区间翻倍”的修改量（令区间内每个元素都变为自身的二倍），结合区间长度信息，我们可以很容易地计算出修改后的新的区间和，这被称为“**能快速修改**”。

   假如某结点维护着一个区间的区间和，此时给此区间添加一个“区间平方”的修改量（令区间内每个元素都变为自身的平方），我们很难计算出修改后的新的区间和，这被称为“**不能快速修改**”。

   显然，在普通线段树中，所有的增量都是能快速修改的修改量。所以普通线段树的结点的 `map` 函数返回类型为 `void` ；而势能线段树的结点的 `map` 函数返回类型为 `bool` ，就是为了返回能否快速修改，通过这一信息指导之后的行为。

   往极限上思考，如果势能线段树的 `map` 函数全部返回 `false` （除叶子结点外，因为叶子结点是一定能够快速修改的），那么所有的区间修改操作都必须不断分治，最终通过若干个单点修改来实现。在这种情况下做区间修改，势能线段树比暴力做法还慢。

   如果势能线段树的 `map` 函数全部返回 `true` ，则恰为普通线段树的情况。

   在 `pushdown` 中发生的所有下传修改，全部对应着 `map` 返回 `true` 的修改，也就是能快速修改的修改。

   **注意：**

   本模板是 `ZkwTree` 模板的势能版本。理论上也可以有 `SegTree` 模板的势能版本，但是编写困难，暂待之后再写。

   **注意：**

   势能线段树的操作非常复杂，是少有的传递参数都很困难的模板。要想编写出合理的结点类型以解决新问题，需要非常熟悉模板的设计，熟悉 `C++` 语法。

   本模板仅提供了一种基础结点类型，即支持区间最大化、区间最小化、区间加操作，支持查询区间最值和区间和的结点，且可以通过模板参数勾选支持的和不支持的功能，这种结点所代表的势能线段树使用非常广泛。一般来说，给定的结点类型可以满足大部分使用需求；如果有新的结点需求，可以借鉴已有的结点类型进行设计。

   如果想看到更多的可用的结点类型，可以参照测试文件里的 `oj` 题目代码，提供了多种成熟可靠的势能线段树结点。

#### 2.建立线段树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到线段树里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示线段树要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到线段树里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到线段树里。


#### 5.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点赋值的下标。

   输入参数 `Modify &&modify​` ，表示在叶子结点处进行的修改操作。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示单点增值的下标。

   输入参数 `Modify &&modify​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.区间增值(add)

1. 数据类型

   输入参数 `size_type left​` ，表示增值的区间起始下标。

   输入参数 `size_type right` ，表示增值的区间结尾下标。（闭区间）

   输入参数 `Modify &&modify` ，表示增量大小。

2. 时间复杂度

   $O(\log n)\sim O(\log^2 n)$ ，取决于具体操作。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.单点查询(query)

1. 数据类型

   模板参数 `typename Getter`，表示从结点中取值、将值进行聚合时的助手类型。

   输入参数 `size_type i` ，表示查询的下标。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   在 `Getter` 类中，通过 `value_type` 规定了查询的返回类型；重载单个结点参数的圆括号运算符以从结点中拿到查询的值。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 9.区间查询(query)

1. 数据类型

   模板参数 `typename Getter`，表示从结点中取值、将值进行聚合时的助手类型。

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   在 `Getter` 类中，通过 `value_type` 规定了查询的返回类型；重载单个结点参数的圆括号运算符以从结点中拿到查询的值；重载两个 `value_type` 的参数的圆括号运算符以实现查询值的聚合。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 10.对所有位置的结点调用回调(do_for_each)

1. 数据类型

   输入参数 `Call &&call` ，表示在所有叶结点调用的回调。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   考虑将一颗线段树的每个位置的值输出，则需要依次调用若干次 `query` 函数，时间复杂度需要带个对数。

   此时可调用本方法，时间复杂度不带对数。
   
### 三、模板示例

```c++
#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

void test_chmin_chmax_add() {
    // 一棵支持区间最值修改的树
    OY::ChminChmaxAddTree<int, int, int, true, true, true> S(10, [](uint32_t i) { return i * 10; });
    cout << S << endl;

    // 最好把 node 类型取出来，因为这个 node 里定义了一些现成的修改类型和查询类型
    using node = decltype(S)::node;

    // Chmax{k} 增量表示使元素 x = std::max(x, k)
    S.add(0, 4, node::Chmax{15});
    cout << S << endl;

    // Chmin{k} 增量表示使元素 x = std::min(x, k)
    S.add(5, 9, node::Chmin{75});
    cout << S << endl;

    // Add{k} 顾名思义就是区间加
    S.add(0, 9, node::Add{100});
    cout << S << endl;

    // 查询的时候也可以针对性查询，因为结点里有 min、max、sum 所以一般来说就查询这三个信息
    cout << "max of S[0~4] = " << S.query<node::MaxGetter>(0, 4) << endl;

    cout << "min of S[5~9] = " << S.query<node::MinGetter>(5, 9) << endl;

    cout << "sum of S[4~5] = " << S.query<node::SumGetter>(4, 5) << endl;

    // 如果很熟悉结点里的信息，也可以自己写个 Getter
    // 比如这里写个查询区间最大值数量的 Getter
    struct MaxCountGetter {
        using value_type = std::pair<int, int>;
        std::pair<int, int> operator()(node *x) const { return std::make_pair(x->m_max1, x->m_max_cnt); }
        std::pair<int, int> operator()(const std::pair<int, int> &x, const std::pair<int, int> &y) const {
            if (x.first > y.first)
                return x;
            else if (x.first < y.first)
                return y;
            else
                return std::make_pair(x.first, x.second + y.second);
        }
    };
    auto res = S.query<MaxCountGetter>(6, 9);
    cout << "max of S[6~9] = " << res.first << endl;
    cout << "max count of S[6~9] = " << res.second << endl;

    // 新增区间 assign 操作
    S.add(0, 9, node::Assign{987});
    cout << S << endl;
}

void test_custom() {
    // 这里我们不妨设计一种结点，支持区间取以 2 为底的对数
    // 但是这种运算会让正数变零，变零后再运算就出错了
    // 那换一个操作吧，让每个元素变为以 2 为底的对数 + 1
    // gcc 编译器用 std::__lg 取对数，但是 MSVC 没有
    // 通用起见，我们用 std::bit_width 取对数
    struct Log2Helper {
        static int get(uint32_t x) {
            return std::bit_width(x); // bit_width(x)-1为对数值
        }
    };

    // 这个结点本身就得支持区间加，还支持查询区间最值
    // 再额外要求个查询区间和吧

    struct Node {
        struct ChLog {};
        struct Add {
            int add_by;
        };
        int m_max, m_min, m_sum, m_inc;
        // 重点在 map
        // 区间加信息很好办，一定是可以快速修改的，map 只会返回 true
        static bool map(const Add &inc, Node *x, int len) {
            x->m_max += inc.add_by, x->m_min += inc.add_by, x->m_sum += inc.add_by * len, x->m_inc += inc.add_by;
            return true;
        }
        // 区间取对数需要判断
        // 如果最大值和最小值取对数的变化相同，那么是可以快速修改的，返回 true
        // 如果最大值和最小值取对数的变化不同，那么不可以快速修改的，返回 false
        static bool map(const ChLog &chlog, Node *x, int len) {
            int max_log = Log2Helper::get(x->m_max);
            int min_log = Log2Helper::get(x->m_min);
            if (x->m_max - max_log == x->m_min - min_log) {
                return map(Add{max_log - x->m_max}, x, len);
            }
            return false;
        }
        void set(int val) {
            m_max = m_min = m_sum = val;
        }
        // 聚合的信息不需多言
        void pushup(Node *lchild, Node *rchild) {
            m_max = std::max(lchild->m_max, rchild->m_max);
            m_min = std::min(lchild->m_min, rchild->m_min);
            m_sum = lchild->m_sum + rchild->m_sum;
        }
        // 需要下放的信息，就只有区间加的信息
        void pushdown(Node *lchild, Node *rchild, int len) {
            if (m_inc) {
                lchild->m_max += m_inc, lchild->m_min += m_inc, lchild->m_sum += m_inc * len / 2, lchild->m_inc += m_inc;
                rchild->m_max += m_inc, rchild->m_min += m_inc, rchild->m_sum += m_inc * len / 2, rchild->m_inc += m_inc;
                // 下放后记得清空
                m_inc = 0;
            }
        }
    };

    // 测试一下功能
    OY::SegBeat::Tree<Node> S(10, [](uint32_t i) { return rand(); });
    using node = decltype(S)::node;
    struct SumGetter {
        using value_type = int;
        int operator()(node *x) const { return x->m_sum; }
        int operator()(int x, int y) const { return x + y; }
    };
    // 因为我们没写 get ，所以不能用默认方式打印树
    auto print_S = [&] {
        for (int i = 0; i < 10; i++) cout << S.query<SumGetter>(i) << " \n"[i == 9];
    };
    print_S();
    S.add(0, 4, node::ChLog{});
    print_S();
    S.add(0, 9, node::Add{100});
    print_S();
    S.add(6, 9, node::ChLog{});
    print_S();
    S.add(1, 8, node::ChLog{});
    print_S();
    S.add(0, 9, node::Add{100});
    print_S();
    S.add(0, 9, node::ChLog{});
    print_S();
    // 此时，我们发现树中的值越来越统一
    // 正是这种趋同的倾向，使得 node::map(Chlog,...) 的返回值的 true 的比例越来越高，使得势能线段树不至于退化为暴力修改
}

int main() {
    test_chmin_chmax_add();
    test_custom();
}
```

```
#输出如下
[0, 10, 20, 30, 40, 50, 60, 70, 80, 90]
[15, 15, 20, 30, 40, 50, 60, 70, 80, 90]
[15, 15, 20, 30, 40, 50, 60, 70, 75, 75]
[115, 115, 120, 130, 140, 150, 160, 170, 175, 175]
max of S[0~4] = 140
min of S[5~9] = 150
sum of S[4~5] = 290
max of S[6~9] = 175
max count of S[6~9] = 2
[987, 987, 987, 987, 987, 987, 987, 987, 987, 987]
41 18467 6334 26500 19169 15724 11478 29358 26962 24464
6 15 13 15 15 15724 11478 29358 26962 24464
106 115 113 115 115 15824 11578 29458 27062 24564
106 115 113 115 115 15824 14 15 15 15
106 7 7 7 7 14 4 4 4 15
206 107 107 107 107 114 104 104 104 115
8 7 7 7 7 7 7 7 7 7

```

