#include "IO/FastIO.h"
#include "STR/KMP.h"
#include "STR/RollbackKMP.h"
#include "STR/ZAlgorithm.h"

/*
[P5410 【模板】扩展 KMP/exKMP（Z 函数）](https://www.luogu.com.cn/problem/P5410)
*/
/**
 * 本题作为字符串模板题，有多种解法
 * 首先，肯定可以用 Z 算法/扩展 kmp 求出 Z 函数
 * 其次，可以使用前缀函数间接计算出 Z 函数
 */

void solve_z() {
    std::string a, b;
    cin >> a >> b;
    OY::ZAlgorithm_string zb(b);
    uint64_t sum = b.size() + 1;
    for (int i = 1; i < b.size(); i++)
        sum ^= uint64_t(i + 1) * (zb.query_Z(i) + 1);
    cout << sum << endl;
    sum = 0;
    for (int i = 0, l = -1, r = -1, len = a.size(); i < len; i++) {
        uint64_t z = zb.adjust(l, r, i, len, a);
        sum ^= (i + 1) * (z + 1);
    }
    cout << sum << endl;
}

void solve_kmp() {
    std::string a, b;
    cin >> a >> b;
    OY::KMP_string kmp_b(b);
    // OY::RollbackKMP_string kmp_b(b);

    std::vector<int> zb(b.size());
    for (int i = 1; i < b.size(); i++)
        if (int p = kmp_b.query_Pi(i); p) zb[i - p + 1] = p;
    if (b.size() > 1 && zb[1])
        for (int i = 1; i < zb[1]; i++) zb[i + 1] = zb[1] - i;
    for (int i = zb[1] + 1; i + 1 < b.size(); i++) {
        int t = i;
        if (zb[i] && !zb[i + 1])
            for (int j = 1; j < zb[i] && zb[i + j] <= zb[j]; j++) zb[i + j] = std::min(zb[j], zb[i] - j), t = i + j;
        i = t;
    }
    uint64_t sum = b.size() + 1;
    for (int i = 1; i < b.size(); i++) sum ^= uint64_t(i + 1) * (zb[i] + 1);
    cout << sum << endl;

    std::vector<int> za(a.size());
    for (int i = 0, last_pi = 0; i < a.size(); i++) {
        last_pi = kmp_b.jump(last_pi, a[i]);
        if (b[last_pi] == a[i]) last_pi++;
        if (last_pi) za[i - last_pi + 1] = last_pi;
    }
    for (int i = 0; i + 1 < a.size(); i++) {
        int t = i;
        if (za[i] && !za[i + 1])
            for (int j = 1; j < za[i] && za[i + j] <= zb[j]; j++) za[i + j] = std::min(zb[j], za[i] - j), t = i + j;
        i = t;
    }
    sum = 0;
    for (int i = 0; i < a.size(); i++) sum ^= uint64_t(i + 1) * (za[i] + 1);
    cout << sum << endl;
}

int main() {
    solve_z();
    // solve_kmp();
}