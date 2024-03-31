#include "DS/GlobalHashMap.h"
#include "DS/LinearDSU.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1488. 避免洪水泛滥](https://leetcode.cn/problems/avoid-flood-in-the-city/description/)
*/
/**
 * 本题需要有贪心思想
 * 从前往后扫描，每当发现一个池子要溢出时，从上一次下雨的时间往后找，找第一个空闲日
 * 可以使用并查集优化，将所有的非空闲日合并；加速找空闲日
 */

OY::GHASH::UnorderedMap<int, int, true, 20> GS;
class Solution {
public:
    vector<int> avoidFlood(vector<int> &rains) {
        int n = rains.size();
        OY::LDSU::Table u(n);
        GS.clear();
        vector<int> ans(n, -1);
        for (int i = 0; i < n; i++) {
            int x = rains[i];
            if (x) {
                if (i and rains[i - 1]) u.unite_after(i - 1);
                auto p = GS.insert(x);
                if (!p.m_flag) {
                    // 获取上次出现的位置
                    int j = p.m_ptr->m_mapped;
                    // 从上次出现的位置找下一个空闲日
                    int k = u.find_next(j);
                    if (k < i) {
                        ans[k] = rains[k] = x;
                        if (k + 1 < n and rains[k + 1]) u.unite_after(k);
                        if (k and rains[k - 1]) u.unite_after(k - 1);
                    } else
                        return {};
                }
                p.m_ptr->m_mapped = i;
            }
        }
        for (int i = 0; i < n; i++)
            if (rains[i] == 0) ans[i] = 1;
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(avoidFlood);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif