#include "DS/PersistentBiTrie.h"
#include "DS/WaveLet.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2935. 找出强数对的最大异或值 II](https://leetcode.cn/problems/maximum-strong-pair-xor-ii/)
*/
/**
 * 本题求指定区间内的异或值，可以通过可持久化 01 字典树来实现
 * 也可以通过小波树来解决
 */

class Solution {
    int solve_PerBiTrie(vector<int> &nums) {
        // 建立可持久化字典树池
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
        for (uint32_t l = 0, r = 0; l < nums.size(); l++) {
            while (r < nums.size() && nums[r] <= nums[l] * 2) r++;
            // 可持久化 01 字典树的查询
            ans = max(ans, Trie::reduce_max_bitxor(pool[l], pool[r], nums[l], [](iterator it1, iterator it2) { return it1->cnt != it2->cnt; }));
        }
        return ans;
    }
    int solve_WaveLet(vector<int> &nums) {
        // 建立小波树
        sort(nums.begin(), nums.end());
        OY::WaveLet::Table<uint32_t> S(nums.begin(), nums.end(), std::bit_width<uint32_t>(nums.back()));
        uint32_t ans = 0;
        for (uint32_t l = 0, r = 0; l < nums.size(); l++) {
            while (r < nums.size() && nums[r] <= nums[l] * 2) r++;
            // 小波树的查询
            ans = max(ans, S.max_bitxor(l, r - 1, nums[l]));
        }
        return ans;
    }

public:
    int maximumStrongPairXor(vector<int> &nums) {
        return solve_PerBiTrie(nums);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(maximumStrongPairXor);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif