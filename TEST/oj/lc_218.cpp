#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/Discretizer.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[218. 天际线问题](https://leetcode.cn/problems/the-skyline-problem/)
*/
/**
 * 本题为珂朵莉模板题
 * 珂朵莉既可以用平衡树实现，也可以用线段树实现
 */

class Solution {
    vector<vector<int>> solve_cseg(vector<vector<int>> &buildings) {
        // 先把建筑按高度排序，再做区间覆盖
        ranges::sort(buildings, [](auto &x, auto &y) { return x[2] < y[2]; });
        int maxr = 0;
        for (auto &building : buildings) maxr = max(maxr, building[1]);

        OY::VectorChthollySegTree<int, uint32_t> S(maxr + 1u);
        for (auto &building : buildings) {
            int l = building[0], r = building[1];
            S.modify(l, r - 1, building[2]);
        }
        vector<vector<int>> ans;
        S.enumerate_all([&](int l, int r, int val) {
            if (l || val) ans.push_back({l, val});
        });
        return ans;
    }
    vector<vector<int>> solve_czkw(vector<vector<int>> &buildings) {
        // 先把建筑按高度排序，再做区间覆盖
        OY::Discretizer<int> D;
        for (auto &building : buildings) D << building[0] << building[1];
        D.prepare();
        for (auto &building : buildings) {
            building[0] = D.rank(building[0]);
            building[1] = D.rank(building[1]);
        }

        ranges::sort(buildings, [](auto &x, auto &y) { return x[2] < y[2]; });

        OY::ChthollyZkwTree<int> S(D.size());
        for (auto &building : buildings) {
            int l = building[0], r = building[1];
            S.modify(l, r - 1, building[2]);
        }
        vector<vector<int>> ans;
        S.enumerate_all([&](int l, int r, int val) {
            ans.push_back({D[l], val});
        });
        return ans;
    }

public:
    vector<vector<int>> getSkyline(vector<vector<int>> &buildings) {
        return solve_cseg(buildings);
        // return solve_czkw(buildings);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(getSkyline);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif