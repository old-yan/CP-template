#include "DS/DynamicBitset.h"
#include "DS/LazyBitset.h"
#include "DS/StaticBitset.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[3161. 物块放置查询](https://leetcode.cn/problems/block-placement-queries/)
*/
/**
 * 本题只需要使用位集的查询最长数量 1 ，即可解决
 */

class Solution {
public:
    vector<bool> getResults(vector<vector<int>> &queries) {
        OY::DynamicBitset S(min<int>(queries.size() * 3 + 1, 50001));
        // OY::StaticBitset<50001> S{};
        // OY::StaticLazyBitset<uint32_t, true, 1 << 20> S(min<int>(queries.size() * 3 + 1, 50001));
        S.set(0);
        vector<bool> ans;
        ans.reserve(queries.size());
        for (auto &q : queries)
            if (q[0] == 1)
                S.set(q[1]);
            else
                ans.push_back(q[2] == 1 or (q[1] > 1 and S.longest_zeros(1, q[1] - 1) >= q[2] - 1));
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(getResults);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif