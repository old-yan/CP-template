#include "DS/TagSegTree2D.h"
#include "DS/TagZkwTree2D.h"
#include "IO/FastIO.h"

/*
[P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)
*/
/**
 * 本题需要对二维空间实现区域修改、区域查询
 * 可以使用标记永久化的二维线段树
 */

struct Node {
    using value_type = uint32_t;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    value_type m_val, m_tag;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get_tag() const { return m_tag; }
    void set_tag(const value_type &tag) { m_tag = tag; }
};
struct Zero {
    template <typename... Args>
    uint32_t operator()(Args...) const { return 0; }
};
using Tree = OY::TagSeg2D::Tree<Node, Zero, false, uint32_t, 1 << 11, 1 << 22>;
// using Tree = OY::TagZkw2D::Tree<Node, 1 << 23>;

int main() {
    uint32_t W, H, q;
    cin >> W >> H >> q;
    Tree S(W + 1, H + 1);
    for (uint32_t i = 0; i < q; i++) {
        uint32_t w, h, z, x, y;
        cin >> w >> h >> z >> x >> y;
        uint32_t pre_max = S.query(x, x + w - 1, y, y + h - 1);
        uint32_t now = pre_max + z;
        S.add(x, x + w - 1, y, y + h - 1, now);
    }
    cout << S.query_all();
}