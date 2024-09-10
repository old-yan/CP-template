#include "DS/CartesianTree.h"
#include "IO/FastIO.h"

/*
[Cartesian Tree](https://judge.yosupo.jp/problem/cartesian_tree)(https://github.com/yosupo06/library-checker-problems/issues/554)
*/
/**
 * 本题为笛卡尔树模板题
 */

static constexpr uint32_t N = 1000000;
int main() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> fa(n);
    auto lchild_call = [&](uint32_t p, uint32_t lc) {
        if (~lc) fa[lc] = p;
    };
    auto rchild_call = [&](uint32_t p, uint32_t rc) {
        if (~rc) fa[rc] = p;
    };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto rt = OY::Cartesian::solve<uint32_t, OY::Cartesian::StaticBufferWrap<N>::type>(n, read, lchild_call, rchild_call, std::greater<uint32_t>(), 0);
    fa[rt] = rt;
    for (auto e : fa) cout << e << ' ';
}