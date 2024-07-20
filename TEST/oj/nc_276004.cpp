#include "DS/DynamicBitset.h"
#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"
#include "MISC/SubsetSumKnapsack.h"

/*
[图上计数(Hard)](https://ac.nowcoder.com/acm/problem/276004)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/86639/L
 * 本题显然需要找出所有的 ebcc 然后进行 01 背包
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::EBCC::Graph G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }
    auto sol = G.calc<false, true>();
    if (sol.m_ebcc_cnt == 1)
        cout << "0\n";
    else {
        std::vector<uint32_t> v;
        v.reserve(sol.m_ebcc_cnt);
        sol.do_for_each_ebcc([&](auto first, auto last) { v.push_back(last - first); });
        // 只背包前一半，减小常数
        OY::DynamicBitset S(n / 2 + 2);
        OY::SSK::solve(S, v);
        int64_t i = S.prev_one(n / 2 + 1);
        cout << i * (n - i) << endl;
    }
}