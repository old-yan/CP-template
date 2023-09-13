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
}

void test_sqrt_add() {
    // 一棵支持区间开方修改的树
    OY::ChsqrtAddTree<int, int, int> S(10, [](uint32_t i) { return rand(); });
    cout << S << endl;

    // 最好把 node 类型取出来，因为这个 node 里定义了一些现成的修改类型和查询类型
    using node = decltype(S)::node;

    // Chsqrt{} 增量表示使元素 x = sqrtl(x)
    S.add(0, 4, node::Chsqrt{});
    cout << S << endl;

    // Add{k} 顾名思义就是区间加
    // 这里注意，Add 一般来说只加正数；一定要加负数，加完之后也要保证结果是正的
    S.add(5, 9, node::Add{-10000});
    cout << S << endl;

    // 查询的时候也可以针对性查询，因为结点里有 min、max、sum 所以一般来说就查询这三个信息
    cout << "max of S[0~4] = " << S.query<node::MaxGetter>(0, 4) << endl;

    cout << "min of S[5~9] = " << S.query<node::MinGetter>(5, 9) << endl;

    cout << "sum of S[4~5] = " << S.query<node::SumGetter>(4, 5) << endl;
}

void test_mod() {
    // 一棵支持区间取余修改的树
    OY::ChmodTree<int, int> S(10, [](uint32_t i) { return rand(); });
    cout << S << endl;

    // 最好把 node 类型取出来，因为这个 node 里定义了一些现成的修改类型和查询类型
    using node = decltype(S)::node;

    // Chmod{k} 增量表示使元素 x = x % k
    S.add(0, 4, node::Chmod{100});
    cout << S << endl;

    S.add(5, 9, node::Chmod{155});
    cout << S << endl;

    // 查询的时候也可以针对性查询，因为结点里有 max、sum 所以一般来说就查询这两个信息
    cout << "max of S[0~4] = " << S.query<node::MaxGetter>(0, 4) << endl;

    cout << "sum of S[4~5] = " << S.query<node::SumGetter>(4, 5) << endl;
}

void test_bit_and_or() {
    // 一棵支持区间按位或、按位与的树
    OY::BitAndOrTree<uint32_t, true, true> S(10, [](uint32_t i) { return rand(); });
    cout << S << endl;

    // 最好把 node 类型取出来，因为这个 node 里定义了一些现成的修改类型和查询类型
    using node = decltype(S)::node;

    // 让所有数字都变成奇数
    S.add(0, 9, node::ChBitor{0b1});
    cout << S << endl;

    // 保留所有数字在二进制中的个位、百位、万位
    // 先看一下所有数字的二进制表示
    for (int i = 0; i < 10; i++) cout << std::bitset<16>(S.query(i)).to_string() << " \n"[i == 9];
    S.add(0, 9, node::ChBitand{0b10101});
    cout << S << endl;
    for (int i = 0; i < 10; i++) cout << std::bitset<16>(S.query(i)).to_string() << " \n"[i == 9];

    // 查询的时候也可以针对性查询，因为结点里有 min、max 所以一般来说就查询这三个信息
    cout << "max of S[0~4] = " << S.query<node::MaxGetter>(0, 4) << endl;

    cout << "min of S[5~9] = " << S.query<node::MinGetter>(5, 9) << endl;
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
    OY::SegBeat::Tree<Node, 1 << 10> S(10, [](uint32_t i) { return rand(); });
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
    test_sqrt_add();
    test_mod();
    test_bit_and_or();
    test_custom();
}
/*
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
[41, 18467, 6334, 26500, 19169, 15724, 11478, 29358, 26962, 24464]
[6, 135, 79, 162, 138, 15724, 11478, 29358, 26962, 24464]
[6, 135, 79, 162, 138, 5724, 1478, 19358, 16962, 14464]
max of S[0~4] = 162
min of S[5~9] = 1478
sum of S[4~5] = 5862
[5705, 28145, 23281, 16827, 9961, 491, 2995, 11942, 4827, 5436]
[5, 45, 81, 27, 61, 491, 2995, 11942, 4827, 5436]
[5, 45, 81, 27, 61, 26, 50, 7, 22, 11]
max of S[0~4] = 81
sum of S[4~5] = 87
[32391, 14604, 3902, 153, 292, 12382, 17421, 18716, 19718, 19895]
[32391, 14605, 3903, 153, 293, 12383, 17421, 18717, 19719, 19895]
0111111010000111 0011100100001101 0000111100111111 0000000010011001 0000000100100101 0011000001011111 0100010000001101 0100100100011101 0100110100000111 0100110110110111
[5, 5, 21, 17, 5, 21, 5, 21, 5, 21]
0000000000000101 0000000000000101 0000000000010101 0000000000010001 0000000000000101 0000000000010101 0000000000000101 0000000000010101 0000000000000101 0000000000010101
max of S[0~4] = 21
min of S[5~9] = 5
5447 21726 14771 11538 1869 19912 25667 26299 17035 9894
13 15 14 14 11 19912 25667 26299 17035 9894
113 115 114 114 111 20012 25767 26399 17135 9994
113 115 114 114 111 20012 15 15 15 14
113 7 7 7 7 15 4 4 4 14
213 107 107 107 107 115 104 104 104 114
8 7 7 7 7 7 7 7 7 7

*/