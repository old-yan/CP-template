#include "DS/IntegerSet.h"
#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "STR/Manacher.h"

/*
[Hotaru's problem](https://acm.hdu.edu.cn/showproblem.php?pid=5371)
*/
/**
 * 本题为回文串题目，可以有多种做法
 * 可以使用马拉车算法解决
 */

void solve_manacher() {
    uint32_t t;
    cin >> t;
    OY::INTSET::Table<17> right_centers;
    for (uint32_t i = 1; i <= t; i++) {
        cout << "Case #" << i << ": ";
        uint32_t n;
        cin >> n;
        std::vector<uint32_t> arr(n);
        for (auto &a : arr) cin >> a;
        OY::Manacher<true, false> M(arr);
        // buckets 用于根据根据左端找到所有的中心点
        OY::LBC::LinkBucket<int, 2000000, 2000000> buckets(n, n);
        for (int i = 1; i < n; i++) buckets[i - M.query_max_even(i - 1, i)].push_back(i);
        int ans = 0;
        for (int i = 0; i < n; i++) {
            for (auto j : buckets[i]) right_centers.set(j);
            right_centers.reset(i);
            int len = i ? M.query_max_even(i - 1, i) : 0;
            int it = right_centers.smaller_bound(i + len + 1);
            if (~it) ans = std::max(ans, it - i);
        }
        cout << ans * 3 << endl;
    }
}

int main() {
    solve_manacher();
}