#include "DS/AdjDiff.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1310. 子数组异或查询](https://leetcode.cn/problems/xor-queries-of-a-subarray/)
*/
/**
 * 通过差分查询
 */

class Solution {
public:
    vector<int> xorQueries(vector<int> &arr, vector<vector<int>> &queries) {
        OY::AdjBitxorTable<int, false> S(arr.begin(), arr.end());
        S.switch_to_presum();
        vector<int> ans;
        ans.reserve(queries.size());
        for (auto &q : queries) ans.push_back(S.query(q[0], q[1]));
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(xorQueries);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif