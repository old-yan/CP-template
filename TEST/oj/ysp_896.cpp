#include "IO/FastIO.h"
#include "MATH/GaussJordanBitxorElimination.h"

/*
[System of Linear Equations (Mod 2)](https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2)(https://github.com/yosupo06/library-checker-problems/issues/896)
*/
/**
 * 本题为异或方程组消元模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::GJBE::GaussJordanBitxorElimination<1 << 12, 1 << 12> GS(m, n);
    for (uint32_t i = 0; i != n; i++) {
        std::string s;
        cin >> s;
        for (uint32_t j = 0; j != m; j++) GS.coef(i, j) = s[j] - '0';
    }
    std::string result;
    cin >> result;
    for (uint32_t i = 0; i != n; i++) GS.result(i) = result[i] - '0';
    if (!GS.calc())
        cout << "-1\n";
    else {
        cout << GS.rank() << endl;
        for (uint32_t i = 0; i != m; i++) cout << GS.get_solution(i);
        cout << endl;
        auto call = [&](auto &&bitset) {
            for (uint32_t i = 0; i != m; i++) cout << (bitset[i] ? '1' : '0');
            cout << endl;
        };
        GS.enumerate_base(call);
    }
}