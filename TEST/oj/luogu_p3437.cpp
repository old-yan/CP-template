// #include "DS/TagZkwTree2D.h"
#include "DS/TagSegTree2D.h"
#include "IO/FastIO.h"

/*
[P3437 [POI2006] TET-Tetris 3D](https://www.luogu.com.cn/problem/P3437)
*/
/**
 * 本题需要对二维空间实现区域修改、区域查询
 * 可以使用标记永久化的二维线段树
 */

int main() {
    using Tree = OY::VectorTagMaxSeg2D<uint32_t>;
    Tree::_reserve(1100000, 100000);
    // using Tree = OY::TagMaxZkw2D<uint32_t>;
    
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