#include "DS/LazySegTree2D.h"
#include "IO/FastIO.h"

/*
[P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)
*/
/**
 * 本题为二维的区间修改线段树模板题
*/
struct Node {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    using node_type = Node;
    static value_type op(const value_type &x, const value_type &y) { return x > y ? x : y; }
    static void map(const modify_type &modify, node_type *x, uint32_t row_len, uint32_t column_len) { x->m_val = std::max(x->m_val, modify); }
    static void com(const modify_type &modify, node_type *x) { x->m_modify = std::max(x->m_modify, modify); }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return bool(m_modify); }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() {}
};
// 本题交上去，会有一两个点在超时边缘。可以在查询的时候剪一下枝就能快三分之一。
using Tree = OY::LazySeg2D::Tree<Node, OY::LazySeg2D::Ignore, false, uint32_t, 1 << 22>;
int main() {
    uint32_t W, H, q;
    cin >> W >> H >> q;
    Tree S(W, H);
    for (uint32_t i = 0; i < q; i++) {
        uint32_t w, h, z, x, y;
        cin >> w >> h >> z >> x >> y;
        uint32_t pre_max = S.query(x, x + w - 1, y, y + h - 1);
        uint32_t now = pre_max + z;
        S.add(x, x + w - 1, y, y + h - 1, now);
    }
    cout << S.query_all();
}