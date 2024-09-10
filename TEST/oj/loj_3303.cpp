#include "DS/ReversedSegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"

/*
[#3303. 「联合省选 2020 A」树](https://loj.ac/p/3303)
*/
/**
 * 本题可以使用 ReverseSegCounter 模拟解决
 */

void solve_revsegcounter() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> val(n);
    for (uint32_t i = 0; i != n; i++) cin >> val[i];
    OY::FlatTree::Tree<bool, 525010> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t p;
        cin >> p;
        S.add_edge(p - 1, i);
    }
    S.prepare(), S.set_root(0);

    using Trie = OY::REVSEGCNT::Table<uint32_t, uint32_t, false, true, true, OY::StaticBufferWrapWithCollect<1060000>::type>;
    std::vector<Trie> tries(n);
    uint64_t ans = 0;
    auto pre = [&](uint32_t a, uint32_t p) {
        tries[a].add(val[a], 1);
    };
    auto report = [&](uint32_t a, uint32_t to) {
        tries[to].globally_plus_one();
        tries[a].merge(tries[to]);
    };
    auto after = [&](uint32_t a) {
        ans += tries[a].key_xorsum();
    };
    S.tree_dp_vertex(0, pre, report, after);
    cout << ans;
}

int main() {
    solve_revsegcounter();
}