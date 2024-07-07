#include "IO/LeetcodeIO.h"
#include "STR/ACAutomaton.h"
using namespace std;

/*
[3213. 最小代价构造字符串](https://leetcode.cn/problems/construct-string-with-minimum-cost/)
*/
/**
 * 本题通过 ac 自动机搜索所有适配位置
 */

class Solution {
public:
    int minimumCost(string target, vector<string> &words, vector<int> &costs) {
        constexpr uint32_t inf = 0x3f3f3f3f;
        struct info {
            uint32_t real_fail;
            uint16_t len, cost = 0x3f3f;
        };
        static OY::AC::Automaton<info, 26> S;
        S.reserve(50000);
        for (int i = 0; i < words.size(); i++) {
            auto p = S.get_node(S.insert_lower(words[i]));
            p->len = words[i].size();
            p->cost = min<int>(p->cost, costs[i]);
        }
        S.prepare();
        // 为了跳过没有匹配的结点，所以预处理一下 real_fail
        S.do_for_extending_nodes([](uint32_t cur) {
            auto p = S.get_node(cur);
            auto fa = S.get_fail_node(cur);
            p->real_fail = fa->len ? p->m_fail : fa->real_fail;
        });
        vector<uint32_t> dp(target.size() + 1, inf);
        dp[0] = 0;
        uint32_t pos = 0;
        for (int r = 0; r < target.size(); r++) {
            pos = S.next(pos, target[r] - 'a');
            // 遍历所有和 target[~r] 后缀相同的模式串
            for (auto x = pos; x; x = S.get_node(x)->real_fail) {
                auto p = S.get_node(x);
                dp[r + 1] = min(dp[r + 1], dp[r - p->len + 1] + p->cost);
            }
        }
        return dp.back() == inf ? -1 : dp.back();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minimumCost);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif
