#include "IO/FastIO.h"
#include "MISC/Spiral.h"

#include <cmath>

/*
[P8668 [蓝桥杯 2018 省 B] 螺旋折线](https://www.luogu.com.cn/problem/P8668)
*/
/**
 * 本题为螺旋矩阵模板题
 * 假设方阵边长为 a*2+1
 * 那么 (x,y) 转换到 (y+a,x+a)
 */

int main() {
    int x, y;
    cin >> x >> y;
    uint32_t a = std::max<int>(abs(x), abs(y));
    OY::SpiralSquare S(a * 2 + 1);
    cout << S.index_of({a, a}) - S.index_of({y + a, x + a});
}