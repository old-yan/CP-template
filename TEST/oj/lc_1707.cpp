#include "DS/BiTrie.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
*/
/**
 * 本题求最大异或，需要 01 字典树
 * SegCounter 也有这个功能
 */

class Solution {
    vector<int> solve_segcounter(vector<int> &nums, vector<vector<int>> &queries) {
        vector<tuple<int, int, int>> qs;
        vector<int> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) qs.emplace_back(queries[i][1], queries[i][0], i);
        sort(qs.begin(), qs.end());
        sort(nums.begin(), nums.end());
        OY::SEGCNT::Table<uint32_t, uint32_t, false, false, false, OY::StaticBufferWrapWithCollect<200000>::type> S;
        int cur = 0;
        for (auto &[m, x, i] : qs) {
            while (cur < nums.size() && nums[cur] <= m) S.add(nums[cur++], 1);
            ans[i] = cur ? (S.max_bitxor(x)->key() ^ x) : -1;
        }
        return ans;
    }
    vector<int> solve_bitrie(vector<int> &nums, vector<vector<int>> &queries) {
        vector<tuple<int, int, int>> qs;
        vector<int> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) qs.emplace_back(queries[i][1], queries[i][0], i);
        sort(qs.begin(), qs.end());
        sort(nums.begin(), nums.end());
        OY::BiTrie::Tree<uint32_t, 30, OY::BiTrie::Ignore, OY::StaticBufferWrapWithCollect<1550000>::type> S;
        int cur = 0;
        for (auto &[m, x, i] : qs) {
            while (cur < nums.size() && nums[cur] <= m) S.insert(nums[cur++]);
            ans[i] = cur ? S.max_bitxor(x).second : -1;
        }
        return ans;
    }

public:
    vector<int> maximizeXor(vector<int> &nums, vector<vector<int>> &queries) {
        return solve_segcounter(nums, queries);
        // return solve_bitrie(nums, queries);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maximizeXor);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif