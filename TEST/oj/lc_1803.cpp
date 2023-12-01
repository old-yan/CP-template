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
        OY::BiTrie32<15, node, 10000000> S;
        using iterator = decltype(S)::iterator;
        // 写个函数计算树中与自己异或值大于某值的数字数量
        auto get = [&](uint32_t self, uint32_t t) -> int64_t {
            static constexpr uint32_t tot = 1 << 15;
            iterator cur = S.m_root;
            int cur_xor = 0, cur_mask = tot / 2;
            int64_t res = 0;
            for (auto c : OY::BiTrie::NumberInteration<uint32_t, 15>(self)) {
                if (!cur) break;
                if ((cur_xor + (cur_mask - 1)) <= t) {
                    cur_xor += cur_mask;
                    res += cur.child(c)->m_size;
                    cur = cur.child(c ^ 1);
                } else
                    cur = cur.child(c);
                cur_mask >>= 1;
            }
            return res;
        };
        int ans = 0;
        for (int a : nums) {
            ans += get(a, high) - (low ? get(a, low - 1) : 0);
            S.m_root->m_size++;
            S.insert(a, [](iterator p) { p->m_size++; });
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