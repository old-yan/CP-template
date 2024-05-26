#include "IO/FastIO.h"
#include "MATH/ExtendedEuclidean.h"

/*
[P1082 [NOIP2012 提高组] 同余方程](https://www.luogu.com.cn/problem/P1082)
*/
/**
 * 本题要对二元一次方程进行配平，需要用到扩展欧几里得算法
 */

int main() {
    uint32_t a, b;
    cin >> a >> b;
    cout << OY::ExtenedEuclideanSolver::solve(a, b, 1, 1).m_coef1 << endl;
}