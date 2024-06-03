#include "DS/BiTrie.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)
*/
/**
 * 本题求指定异或值的数对数，可以借助 01 字典树
 */
struct node {
    int m_size;
};
class Solution {
public:
    int countPairs(vector<int> &nums, int low, int high) {
        OY::BiTrie32<15, node> S;
        using node = decltype(S)::node;
        S.reserve_by_length(nums.size());
        // 写个函数计算树中与自己异或值大于某值的数字数量
        auto get = [&](uint32_t self, uint32_t t) -> int64_t {
            static constexpr uint32_t tot = 1 << 15;
            node *cur = S.root();
            int cur_xor = 0, cur_mask = tot / 2;
            int64_t res = 0;
            for (auto c : OY::BiTrie::NumberInteration<uint32_t, 15>(self)) {
                if (!cur) break;
                node *c1 = S._child_of(cur, c), *c2 = S._child_of(cur, c ^ 1);
                if ((cur_xor + (cur_mask - 1)) <= t) {
                    cur_xor += cur_mask;
                    if (c1) res += c1->m_size;
                    cur = c2;
                } else
                    cur = c1;
                cur_mask >>= 1;
            }
            return res;
        };
        int ans = 0;
        for (int a : nums) {
            ans += get(a, high) - (low ? get(a, low - 1) : 0);
            S.root()->m_size++;
            S.insert(a, [](node *p) { p->m_size++; });
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countPairs);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif