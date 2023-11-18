#include "DS/TagSegTree2D.h"
#include "IO/FastIO.h"

template <typename Tp>
struct Zero {
    template <typename... Args>
    Tp operator()(Args...) const { return 0; }
};
void test_sum_tree() {
    // 最可能用到的二维可区域加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::TagSeg2D::Tree<OY::TagSeg2D::BaseNode<int64_t>, Zero<int64_t>, false, uint64_t, 1000, 10000>;
    SumTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 1);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

template <typename ValueType, ValueType Min>
struct MaxNode {
    using value_type = ValueType;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    static value_type applied(const value_type &x) { return x; }
    static value_type default_tag() { return Min; }
    value_type m_val, m_tag;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get_tag() const { return m_tag; }
    void set_tag(const value_type &tag) { m_tag = tag; }
    void clear_tag() { m_val = m_tag = Min; }
};
template <typename Tp>
struct Min {
    template <typename... Args>
    Tp operator()(Args...) const { return -1; }
};
void test_max_tree() {
    // 可能用到的二维可区域最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::TagSeg2D::Tree<MaxNode<int64_t, -1>, Min<int64_t>, false, uint64_t, 1000, 10000>;
    MaxTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 100);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

template <typename ValueType>
struct ProdNode {
    using value_type = ValueType;
    static value_type op(const value_type &x, const value_type &y) { return x * y; }
    static value_type applied(const value_type &x) { return x; }
    static value_type applied(const value_type &x, int row_width, int col_width) { return pow(x, row_width * col_width); }
    static value_type default_tag() { return 1; }
    value_type m_val, m_tag;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get_tag() const { return m_tag; }
    void set_tag(const value_type &tag) { m_tag = tag; }
    void clear_tag() { m_val = m_tag = 1; }
};
void test_prod_tree() {
    // 展示用法的二维可区域乘的求积树
    cout << "test prod tree:\n";

    using ProdTree = OY::TagSeg2D::Tree<ProdNode<double>, OY::TagSeg2D::Ignore, true, uint32_t, 1000, 10000>;
    ProdTree S(5, 7);
    cout.precision(1);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(1, 3, 1, 4, 2);
    cout << S << endl;
    cout << S.query(0, 2, 0, 2) << endl;
}

int main() {
    test_sum_tree();
    test_max_tree();
    test_prod_tree();
}
/*
#输出如下
test sum tree:
0
250000001000000001

test max tree:
-1
100

test prod tree:
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
1.0
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
16.0

*/