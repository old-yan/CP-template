#include "DS/BiTrie.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

/*
[U109897 [HDU5536]Chip Factory](https://www.luogu.com.cn/problem/U109897)
*/
/**
 * 本题可以通过带删字典树解决
 */

void solve_segcounter() {
    uint32_t n;
    cin >> n;
    using Tree = OY::StaticSegCounter<uint32_t, uint32_t, false, false, false, 2000>;
    Tree S;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i], S.add(arr[i], 1);
    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        S.add(arr[i], -1);
        for (uint32_t j = i + 1; j != n; j++) {
            S.add(arr[j], -1);
            ans = std::max(ans, S.max_bitxor(arr[i] + arr[j])->key() ^ (arr[i] + arr[j]));
            S.add(arr[j], 1);
        }
        S.add(arr[i], 1);
    }
    cout << ans;
}

void solve_bitrie() {
    uint32_t n;
    cin >> n;
    using Tree = OY::StaticCountBiTrie32<31, 24000>;
    Tree S;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i], S.insert_one(arr[i]);
    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        S.erase_one(arr[i]);
        for (uint32_t j = i + 1; j != n; j++) {
            S.erase_one(arr[j]);
            ans = std::max(ans, S.max_bitxor(arr[i] + arr[j]).second);
            S.insert_one(arr[j]);
        }
        S.insert_one(arr[i]);
    }
    cout << ans;
}

int main() {
    solve_segcounter();
    // solve_bitrie();
}