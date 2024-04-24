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
[987, 987, 987, 987, 987, 987, 987, 987, 987, 987]
41 18467 6334 26500 19169 15724 11478 29358 26962 24464
6 15 13 15 15 15724 11478 29358 26962 24464
106 115 113 115 115 15824 11578 29458 27062 24564
106 115 113 115 115 15824 14 15 15 15
106 7 7 7 7 14 4 4 4 15
206 107 107 107 107 114 104 104 104 115
8 7 7 7 7 7 7 7 7 7

*/