#include "DS/DivideTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
*/
/**
 * 本题类似 P2617 P3380，但是不带修改
 * 因为不带修改，所以不需要使用二维线段树再进行多行查询
 * 如果每一行都是在上一行的基础上，添加一个数字，那么查询就可以只针对单行
 * 这样就可以使用可持久化线段树
 *
 * 然而，本题的问题，使用划分树或者小波树最佳
 */
int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto S = OY::make_DivideTree<int, std::less<int>, 1 << 24>(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    // auto S = OY::WaveLetTree<int, uint64_t, 200000>(
    //     n, [](auto...) {
    //         int x;
    //         cin >> x;
    //         return x;
    //     });
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << S.quantile(l - 1, r - 1, k - 1) << endl;
    }
}