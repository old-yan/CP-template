#include "DS/BiTrie.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1707. 与数组中元素的最大异或值](https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/)
*/
class Solution {
public:
    vector<int> maximizeXor(vector<int> &nums, vector<vector<int>> &queries) {
        vector<tuple<int, int, int>> qs;
        vector<int> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) qs.emplace_back(queries[i][1], queries[i][0], i);
        sort(qs.begin(), qs.end());
        sort(nums.begin(), nums.end());
        OY::BiTrie<uint32_t, 30, OY::BinaryTrie::BaseInfo, 1 << 22> S;
        int cur = 0;
        for (auto &[m, x, i] : qs) {
            while (cur < nums.size() && nums[cur] <= m) S.insert(nums[cur++]);
            ans[i] = cur ? S.query_max_bitxor(x).second : -1;
        }
        return ans;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maximizeXor);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}