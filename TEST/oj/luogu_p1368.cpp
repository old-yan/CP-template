#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/HashLCP.h"
#include "STR/MinimalSequence.h"
#include "STR/SuffixArray.h"

/*
[P1368 【模板】最小表示法](https://www.luogu.com.cn/problem/P1368)
*/
/**
 * 本题可以有多种解法
 * 首先，最小表示法标准做法可以有 O(n) 的小常数做法
 * 显然可以用哈希做
 * 也可以用后缀数组做
 */

void solve_minimal_sequence() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    uint32_t i = OY::get_minimal_rotation(arr.begin(), arr.end());
    for (uint32_t j = i; j < n + i; j++) cout << arr[j % n] << ' ';
}

void solve_hash() {
    using mint = OY::mintu32;
    using lcp_type = OY::HASHLCP::LCP<std::vector<uint32_t>, mint, 300001>;
    using hash_type = lcp_type::table_type::hash_type;
    uint32_t n;
    cin >> n;
    hash_type::s_info.prepare_unit(n);
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    arr.insert(arr.end(), arr.begin(), arr.end());
    lcp_type LCP(arr.begin(), arr.end());

    uint32_t ans = 0;
    for (uint32_t i = 1; i < n; i++)
        if (LCP.compare(i, i + n - 1, ans, ans + n - 1) < 0) ans = i;
    for (uint32_t j = ans; j < n + ans; j++) cout << arr[j % n] << ' ';
}

void solve_SA() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    OY::SA::SuffixArray<false, false, 600000> SA(n * 2, [&](uint32_t i) {
        return arr[i < n ? i : i - n] + 1;
    });

    uint32_t rnk = 0;
    while (SA.query_sa(rnk) >= n) rnk++;
    uint32_t ans = SA.query_sa(rnk);
    for (uint32_t j = ans; j < n + ans; j++) cout << arr[j % n] << ' ';
}

int main() {
    solve_minimal_sequence();
    // solve_hash();
    // solve_SA();
}
