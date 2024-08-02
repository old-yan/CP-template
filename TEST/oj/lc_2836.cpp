#include "GRAPH/Psuedotree_ig.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2836. 在传球游戏中最大化函数值](https://leetcode.cn/problems/maximize-value-of-function-in-a-ball-passing-game/)
*/
/**
 * 作为倍增板子题很好做
 * 但是我们也可以尝试一下建基环森林
 * 将 MaintainAncestor 参数设为 true
 */

class Solution {
public:
    long long getMaxFunctionValue(vector<int> &receiver, long long k) {
        int n = receiver.size();
        OY::PsuedoIG::Graph<uint32_t, uint64_t, true> G(n);
        for (int i = 0; i < n; i++) G.set_edge(i, receiver[i], receiver[i]);
        G.prepare();
        uint64_t ans = 0;
        for (int i = 0; i < n; i++) {
            uint64_t sum = i;
            // info 是结点 i 的信息
            auto info = G.get_vertex_info(i);
            // cycle 是结点 i 所依附的环的信息
            auto cycle = G.get_cycle_info(info.cycle_id());
            if (k < info.step_to_cycle()) {
                // 如果走 k 步还不足以进入环，那么在环外直接做差即可
                int dest = G.next(i, k);
                sum += info.dis_to_cycle() - G.get_vertex_info(dest).dis_to_cycle();
            } else {
                // 如果走 k 步足以进入环，那么总路程分成三部分：环外的，环上的绕整圈部分，环上的不够一圈的部分
                uint64_t kk = k;
                // 环外部分
                sum += info.dis_to_cycle(), kk -= info.step_to_cycle();
                // 整圈部分
                sum += kk / cycle.size() * cycle.length(), kk %= cycle.size();
                // 不够一圈的部分，i1 i2 分别是在环上的下标
                int i1 = info.cycle_join(), i2 = (i1 + kk % cycle.size()) % cycle.size();
                if (i1 <= i2)
                    sum += cycle.position(i2) - cycle.position(i1);
                else
                    sum += cycle.length() + cycle.position(i2) - cycle.position(i1);
            }
            ans = max(ans, sum);
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(getMaxFunctionValue);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif