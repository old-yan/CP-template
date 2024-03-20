#include "IO/FastIO.h"

/*
[P1923 【深基9.例4】求第 k 小的数](https://www.luogu.com.cn/problem/P1923)
*/
/**
 * 本题为经典分治题
 * 影响本题速度的，大概是输入输出效率吧
 */

uint32_t buf[5000000];
int main() {
    size_t n, k;
    cin >> n >> k;
    for (size_t i = 0; i != n; i++) cin >> buf[i];
    std::nth_element(buf, buf + k, buf + n);
    cout << buf[k];
}