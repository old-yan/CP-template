#include "DS/BiTrie.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[U109923 [Codechef REBXOR]Nikitosh and xor](https://www.luogu.com.cn/problem/U109923)
*/
/**
 * 区间异或和即前缀和数组的两点异或
 */

void solve_segcounter() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];

    std::vector<uint32_t> pre_max(n);
    using Tree = OY::SEGCNT::Table<uint32_t, uint32_t, false, false, false, OY::StaticBufferWrapWithCollect<800000>::type>;
    Tree S;
    uint32_t cur_max = 0, cur_xor = 0, ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        S.add(cur_xor, 1);
        cur_xor ^= arr[i];
        cur_max = std::max(cur_max, S.max_bitxor(cur_xor)->key() ^ cur_xor);
        pre_max[i] = cur_max;
    }
    cur_max = 0, cur_xor = 0;
    S.clear();
    for (uint32_t i = n - 1; i; i--) {
        S.add(cur_xor, 1);
        cur_xor ^= arr[i];
        cur_max = std::max(cur_max, S.max_bitxor(cur_xor)->key() ^ cur_xor);
        ans = std::max(ans, pre_max[i - 1] + cur_max);
    }
    cout << ans;
}

void solve_bitrie() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];

    std::vector<uint32_t> pre_max(n);
    using Tree = OY::BiTrie::Tree<uint32_t, 30, OY::BiTrie::Ignore, OY::StaticBufferWrapWithCollect<6000000>::type>;
    Tree S;
    uint32_t cur_max = 0, cur_xor = 0, ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        S.insert(cur_xor);
        cur_max = std::max(cur_max, S.max_bitxor(cur_xor ^= arr[i]).second);
        pre_max[i] = cur_max;
    }
    cur_max = 0, cur_xor = 0;
    S.clear();
    for (uint32_t i = n - 1; i; i--) {
        S.insert(cur_xor);
        cur_max = std::max(cur_max, S.max_bitxor(cur_xor ^= arr[i]).second);
        ans = std::max(ans, pre_max[i - 1] + cur_max);
    }
    cout << ans;
}

int main() {
    solve_segcounter();
    // solve_bitrie();
}