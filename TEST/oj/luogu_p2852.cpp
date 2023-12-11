#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"
#include "STR/SuffixArray.h"

#include <map>

/*
[P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
*/
/**
 * 本题求子串出现次数，可以有多种做法
 * 可以使用后缀数组解决
 * 也可以使用字符串哈希解决
 */

void solve_SA() {
    uint32_t n, k;
    cin >> n >> k;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) {
        cin >> a, a++;
    }

    OY::SA::SuffixArray<std::vector<uint32_t>, 20002> SA(arr.begin(), arr.end());
    SA.get_rank(), SA.get_height();

    std::vector<uint32_t> stack;
    uint32_t cursor = 0;
    uint32_t ans = 0;
    for (uint32_t l = 1, r = 1; l + k - 1 <= n; l++) {
        if (cursor < stack.size() && stack[cursor] < l) cursor++;
        while (r < l + k - 1) {
            while (cursor < stack.size() && SA.query_height(stack.back()) > SA.query_height(r)) stack.pop_back();
            stack.push_back(r++);
        }
        ans = std::max(ans, SA.query_height(stack[cursor]));
    }
    cout << ans << endl;
}

void solve_hash() {
    using mint = OY::mint998244353;
    using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, 500000>;
    using hash_type = table_type::hash_type;

    uint32_t n, k;
    cin >> n >> k;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;

    table_type S(arr);

    auto check = [&](int len) {
        std::map<hash_type, int> mp;
        for (int l = 0, r = len - 1; r < arr.size(); l++, r++) {
            if (++mp[S.query(l, r)] == k) return true;
        }
        return false;
    };

    uint32_t low = 0, high = n - k + 1;
    while (low < high) {
        auto mid = (low + high + 1) / 2;
        if (check(mid))
            low = mid;
        else
            high = mid - 1;
    }
    cout << low << endl;
}

int main() {
    solve_SA();
    // solve_hash();
}