#include "IO/FastIO.h"
#include "MATH/StaticMontgomeryModInt32.h"
#include "STR/HashLCP.h"
#include "STR/SALCP.h"

/*
[字串比较](https://ac.nowcoder.com/acm/problem/269902)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/76681/I
 * 纯模板
 */

void solve_sa() {
    uint32_t n, m, q;
    std::string s1, s2;
    cin >> n >> m >> q >> s1 >> s2;
    auto LCP = OY::make_SA_LCP<400000>(s1 + s2);
    while (q--) {
        uint32_t l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        auto res = LCP.compare(l1 - 1, r1 - 1, n + l2 - 1, n + r2 - 1);
        if (res < 0)
            cout << "<\n";
        else if (!res)
            cout << "=\n";
        else
            cout << ">\n";
    }
}

using mint = OY::mgint1000000007;
using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
using hash_type = table_type::hash_type;
auto hash_init = []() {
    hash_type::s_info.set_random_base(131);
    hash_type::s_info.prepare_unit(200000);
    hash_type::s_info.prepare_unit_inv(200000);
    return 0;
}();
void solve_hash() {
    uint32_t n, m, q;
    std::string s1, s2;
    cin >> n >> m >> q >> s1 >> s2;
    auto LCP = OY::make_hash_LCP<mint>(s1 + s2);
    while (q--) {
        uint32_t l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        auto res = LCP.compare(l1 - 1, r1 - 1, n + l2 - 1, n + r2 - 1);
        if (res < 0)
            cout << "<\n";
        else if (!res)
            cout << "=\n";
        else
            cout << ">\n";
    }
}

int main() {
    solve_sa();
    // solve_hash();
}
