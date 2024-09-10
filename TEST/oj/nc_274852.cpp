#include "DS/LinkBucket.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[parent 树上启发式合并](https://ac.nowcoder.com/acm/problem/274852)
*/
/**
 * 注意本题题面坑。询问字符串的和是 300000
 */

int main() {
    uint32_t n, q;
    std::string s;
    cin >> n >> q >> s;

    auto get_val = [](char c) {
        if (c <= '9')
            return c - '0';
        else if (c <= 'Z')
            return c - 'A' + 10;
        else
            return c - 'a' + 36;
    };
    OY::AC::Automaton<OY::AC::BaseNode, 62> ac;
    OY::LBC::LinkBucket<std::pair<uint32_t, uint32_t>> qs(300001, q);
    OY::SEGCNT::Table<uint32_t, uint32_t, true, false, false, OY::StaticBufferWrapWithCollect<200000>::type> tails[300001];
    for (uint32_t i = 0; i != q; i++) {
        std::string s;
        uint32_t k;
        cin >> s >> k;
        auto it = ac.insert(s.begin(), s.end(), [&](char c) { return get_val(c); });
        qs[it].push_front(std::make_pair(i, k - 1));
    }
    ac.prepare();

    uint32_t pos = 0;
    for (uint32_t i = 0; i < s.size(); i++) {
        pos = ac.next(pos, get_val(s[i]));
        tails[pos].add(i, 1);
    }
    std::vector<uint32_t> ans(q);
    ac.do_for_failing_nodes([&](uint32_t a) {
        // 先解决本地的询问
        for (auto &[idx, k] : qs[a]) ans[idx] = k < tails[a].query_all() ? tails[a].kth(k)->key() + 1 : -1;
        // 再把本地的尾下标们合并到 fail 上
        uint32_t p = ac.query_fail(a);
        tails[p].merge(tails[a]);
    });
    // 输出答案
    for (int a : ans) cout << a << endl;
}