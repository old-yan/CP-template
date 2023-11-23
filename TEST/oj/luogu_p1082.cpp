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
    auto res = OY::ExtenedEuclideanSolver::solve(a, b);

    /**
     * 在扩展欧几里得算法算出的默认结果里，即便某个系数是负数，只需要调整一步就可以变为非负
    */
    int64_t ans = res.m_coef1;
    if (ans <= 0) ans += b / res.m_gcd;
    cout << ans;
}