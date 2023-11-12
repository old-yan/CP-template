#include "DS/PersistentBiTrie.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2935. 找出强数对的最大异或值 II](https://leetcode.cn/problems/maximum-strong-pair-xor-ii/)
*/
/**
 * 本题求指定区间内的异或值，可以通过可持久化 01 字典树来实现
 */

class Solution {
public:
    int maximumStrongPairXor(vector<int> &nums) {
        struct Info {
            int cnt;
        };
        using Trie = OY::PerBiTrie32<20, Info, false, 8000000>;
        using iterator = Trie::iterator;
        sort(nums.begin(), nums.end());
        std::vector<Trie> pool;
        Trie Base;
        Base.init();
        pool.push_back(Base);
        for (int a : nums) {
            Trie copy = pool.back().copy();
            copy.insert(a, [](iterator it) { it->cnt++; });
            pool.push_back(copy);
        }

        uint32_t ans = 0;
        for (int x : nums) {
            auto l = lower_bound(nums.begin(), nums.end(), (x + 1) / 2) - nums.begin();
            auto r = lower_bound(nums.begin(), nums.end(), x * 2 + 1) - nums.begin();
            ans = max(ans, Trie::reduce_max_bitxor(pool[l], pool[r], x, [](iterator it1, iterator it2) {
                          return it1->cnt != it2->cnt;
                      }));
        }
        return ans;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maximumStrongPairXor);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}