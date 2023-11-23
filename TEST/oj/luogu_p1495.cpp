#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem.h"

/*
[P1495 【模板】中国剩余定理（CRT）/ 曹冲养猪](https://www.luogu.com.cn/problem/P1495)
*/
/**
 * 本题为中国剩余定理模板题
*/

int main() {
    uint32_t n;
    cin >> n;
    OY::ChineseRemainderTheorem crt(3);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t div, rem;
        cin >> div >> rem;
        crt.add(rem, div);
    }
    cout << crt.query();
}