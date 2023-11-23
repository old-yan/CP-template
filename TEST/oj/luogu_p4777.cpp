#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem_ex.h"

/*
[P4777 【模板】扩展中国剩余定理（EXCRT）](https://www.luogu.com.cn/problem/P4777)
*/
/**
 * 本题为扩展中国剩余定理模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::ChineseRemainderTheorem_ex crt;
    for (uint32_t i = 0; i < n; i++) {
        uint64_t div, rem;
        cin >> div >> rem;
        crt.add(rem, div);
    }
    cout << crt.query();
}