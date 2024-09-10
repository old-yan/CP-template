#include "DS/LazyBitset.h"
#include "DS/OfflineRangeMex.h"
#include "DS/RangeMex.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/LeetcodeIO.h"
#include "TREE/DfnController.h"
#include "TREE/FlatTree.h"
using namespace std;

/*
[2003. 每棵子树内缺失的最小基因值](https://leetcode.cn/problems/smallest-missing-genetic-value-in-each-subtree/)
*/
/**
 * 可以离线在树上进行 bitset 的合并
 */

class Solution {
    vector<int> solve_bitset(vector<int> &parents, vector<int> &nums) {
        int n = parents.size();
        OY::FlatTree::Tree<bool, 500000> S(n);
        for (int i = 1; i < n; i++) S.add_edge(i, parents[i]);
        S.prepare(), S.set_root(0);
        using Bitset = OY::LazyBitset::Tree<uint32_t, false, OY::StaticBufferWrapWithoutCollect<4000000>::type>;
        vector<Bitset> bs(n);
        for (int i = 0; i < n; i++) bs[i].resize(n + 1);
        vector<int> ans(n);
        auto report = [&](int a, int to) {
            bs[a] |= bs[to];
        };
        auto after = [&](int a) {
            if (nums[a] - 1 <= n) bs[a].set(nums[a] - 1);
            ans[a] = bs[a].first_zero() + 1;
        };
        S.tree_dp_vertex(0, {}, report, after);
        return ans;
    }
    vector<int> solve_offlinemex(vector<int> &parents, vector<int> &nums) {
        int n = parents.size();
        OY::FlatTree::Tree<bool, 500000> S(n);
        for (int i = 1; i < n; i++) S.add_edge(i, parents[i]);
        S.prepare(), S.set_root(0);
        auto controller = OY::DFN::Table<decltype(S)>(&S);
        OY::OfflineRangeMexSolver sol(n, n);
        for (int i = 0; i < n; i++)
            controller.do_for_subtree(i, [&](auto l, auto r) {
                sol.add_query(l, r);
            });
        vector<int> seq(n);
        for (int i = 0; i < n; i++) controller.do_for_vertex(i, [&](auto pos) { seq[pos] = nums[i] - 1; });
        auto ans = sol.solve(seq.begin(), seq.end());
        vector<int> res(n);
        for (int i = 0; i < n; i++) res[i] = ans[i] + 1;
        return res;
    }
    vector<int> solve_onlinemex(vector<int> &parents, vector<int> &nums) {
        int n = parents.size();
        OY::FlatTree::Tree<bool, 500000> S(n);
        for (int i = 1; i < n; i++) S.add_edge(i, parents[i]);
        S.prepare(), S.set_root(0);
        auto controller = OY::DFN::Table<decltype(S)>(&S);

        vector<int> seq(n);
        for (int i = 0; i < n; i++) controller.do_for_vertex(i, [&](auto pos) { seq[pos] = nums[i] - 1; });
        OY::RangeMex<> sol(seq.begin(), seq.end());
        vector<int> res(n);
        for (int i = 0; i < n; i++)
            controller.do_for_subtree(i, [&](auto l, auto r) {
                res[i] = sol.query(l, r) + 1;
            });
        return res;
    }

public:
    vector<int> smallestMissingValueSubtree(vector<int> &parents, vector<int> &nums) {
        return solve_bitset(parents, nums);
        // return solve_offlinemex(parents, nums);
        // return solve_onlinemex(parents, nums);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(smallestMissingValueSubtree);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif