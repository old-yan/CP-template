#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/KMP.h"
#include "STR/RollbackKMP.h"
#include "STR/StrHash.h"
#include "STR/ZAlgorithm.h"

/*
[P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
*/
/**
 * 本题可以有多种解法
 * 若 a 为回文串， b 为回文串，那么 a 和 b 的循环节必须相同，才能保证 a + b 也为回文串
 * 而找循环节在 P3538 中有哈希的做法
 * 但是，找循环节最佳的做法是 kmp
 * 当然也可以用 Z 算法做
 */

static constexpr uint32_t N = 2000000;
using mint = OY::mintu32;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
using hash_type = table_type::hash_type;
namespace OY {
    namespace GHASH {
        template <size_type L>
        struct Hash<hash_type, L> {
            size_type operator()(const auto &x) const { return Hash<size_t, L>()(*(size_t *)(&x)); }
        };
    }
}

#include "MATH/Eratosthenes.h"
uint32_t find_pattern_hash(const table_type &S) {
    static OY::Eratosthenes::Sieve<N, false, true, false> es;
    uint32_t len = S.m_presum.size() - 1;
    for (auto &&[p, c] : es.decomposite(len))
        while (c--) {
            uint32_t pl = len / p;
            if (S.query_value(0, len - pl - 1) == S.query_value(pl, len - 1)) len = pl;
        }
    return len;
}

uint32_t find_pattern_kmp(const std::string &s) {
    OY::KMP_string kmp(s);
    // OY::RollbackKMP_string kmp(s);
    int val = kmp.query_Pi(s.size() - 1);
    if (val * 2 >= s.size() && s.size() % (s.size() - val) == 0)
        return s.size() - val;
    else
        return s.size();
}

uint32_t find_pattern_z(const std::string &s) {
    OY::ZAlgorithm_string Z(s);
    int i = 1;
    while (i < s.size() && (i + Z.query_Z(i) != s.size() || s.size() % i)) i++;
    return i == s.size() ? s.size() : i;
}

OY::GHASH::UnorderedMap<hash_type, uint32_t, false, 22> GS;
int main() {
    uint32_t n;
    cin >> n;
    std::string s[n];
    uint32_t maxlen = 0;
    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t m;
        cin >> m >> s[i];
        maxlen = std::max(maxlen, m);
    }
    hash_type::s_info.prepare_unit(maxlen), hash_type::s_info.prepare_unit_inv(maxlen);
    for (uint32_t i = 0; i < n; i++) {
        table_type S(s[i]);
        uint32_t pl = find_pattern_hash(S);
        // uint32_t pl = find_pattern_kmp(s);
        // uint32_t pl = find_pattern_z(s);
        hash_type val = S.query_hash(0, pl - 1);
        ans += ++(GS.insert(val).m_ptr->m_mapped) * 2 - 1;
    }
    cout << ans;
}