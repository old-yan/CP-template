#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

/*
[P3538 [POI2012] OKR-A Horrible Poem](https://www.luogu.com.cn/problem/P3538)
*/
/**
 * 本题为字符串哈希模板题
 * 对于每次查询循环节，只要枚举其长度的因子即可
 */

static constexpr uint32_t N = 500000;
void solve_hash() {
    using mint = OY::mintu32;
    using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
    using hash_type = table_type::hash_type;
    uint32_t n;
    std::string s;
    cin >> n >> s;
    hash_type::s_info.prepare_unit(n);

    table_type S(s.begin(), s.end());
    OY::Eratosthenes::Sieve<N, false, true, false> es(n);

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        l--, r--;
        uint32_t len = r - l + 1, res = len;
        // 先把所有的 2 因子去掉
        while (len % 2 == 0)
            if (S.query_value(l, l + len / 2 - 1) == S.query_value(l + len / 2, l + len - 1))
                res /= 2, len /= 2;
            else {
                len >>= std::countr_zero(len);
                break;
            }
        // 再去掉其他因子
        while (len > 1) {
            uint32_t cur = es.query_smallest_factor(len);
            uint32_t pl = res / cur;
            if (S.query_value(l, l + res - pl - 1) == S.query_value(l + pl, l + res - 1)) res = pl;
            len /= cur;
        }
        cout << res << endl;
    }
}

int main() {
    solve_hash();
}