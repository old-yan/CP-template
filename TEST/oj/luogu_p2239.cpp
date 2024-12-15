#include "IO/FastIO.h"
#include "MISC/Spiral.h"

/*
[P2239 [NOIP2014 普及组] 螺旋矩阵](https://www.luogu.com.cn/problem/P2239)
*/
/**
 * 本题为螺旋矩阵模板题
 */

int main() {
    uint32_t n, i, j;
    cin >> n >> i >> j;
    OY::SpiralSquare S(n);
    cout << S.index_of({i - 1, j - 1}) + 1;
}