#include "IO/LeetcodeIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"
using namespace std;

/*
[3241. 标记所有节点需要的时间](https://leetcode.cn/problems/time-taken-to-mark-all-nodes/)
*/
/**
 * 本题显然为换根模板题
 * 求每个点作为根时，最深的结点的距离
 */

class Solution {
public:
    vector<int> timeTaken(vector<vector<int>> &edges) {
        int n = edges.size() + 1;
        OY::FlatTree::Tree<bool, 800000> S(n);
        for (auto &e : edges) S.add_edge(e[0], e[1]);
        S.prepare(), S.set_root(0);
        struct Top2 {
            int m_val[2];
        };
        auto mapping = [](uint32_t i) -> Top2 { return {}; };
        auto merge = [](Top2 &dp_a, Top2 dp_son, uint32_t a, uint32_t son, auto...) {
            int offer = dp_son.m_val[0] + (son % 2 ? 1 : 2);
            if (offer > dp_a.m_val[0])
                dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = offer;
            else if (offer > dp_a.m_val[1])
                dp_a.m_val[1] = offer;
        };
        auto exclude = [](Top2 &dp_a, Top2 dp_fa, uint32_t a, uint32_t fa, auto...) {
            if (~fa) {
                int offer = dp_a.m_val[0] + (a % 2 ? 1 : 2);
                int rest = offer == dp_fa.m_val[0] ? dp_fa.m_val[1] : dp_fa.m_val[0];
                rest += (fa % 2 ? 1 : 2);
                if (rest > dp_a.m_val[0])
                    dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = rest;
                else if (rest > dp_a.m_val[1])
                    dp_a.m_val[1] = rest;
            }
        };
        auto dp = OY::TreeTransfer::solve<Top2>(S, mapping, merge, exclude);
        vector<int> ans(n);
        for (int i = 0; i < n; i++) ans[i] = dp[i].m_val[0];
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(timeTaken);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif