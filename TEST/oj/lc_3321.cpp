#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "IO/LeetcodeIO.h"
#include "MISC/SlideWindow.h"
using namespace std;

/*
[3321. 计算子数组的 x-sum II](https://leetcode.cn/problems/find-x-sum-of-all-k-long-subarrays-ii)
*/
/**
 * 滑动窗口模板，平衡树模板
 */

class Solution {
public:
    vector<long long> findXSum(vector<int> &nums, int k, int x) {
        // 本平衡树维护两个属性的和
        // 属性一是频率
        // 属性二是键和频率的乘积
        struct Monoid {
            using value_type = pair<int, long long>;
            static value_type op(value_type x, value_type y) { return {x.first + y.first, x.second + y.second}; }
            static value_type identity() { return {0, 0}; }
        };
        using Tree = OY::MONOAVL::Tree<Monoid, false>;
        // using Tree = OY::MONOSPLAY::Tree<Monoid, false>;
        Tree S;
        // mp 维护频率
        unordered_map<int, int> mp;
        vector<long long> ans;
        // 给固定大小的滑动窗口传递回调函数
        // 对已就绪的窗口调用的回调
        auto call = [&](int l, int r) {
            if (S.size() < x)
                ans.push_back(S.query_all().second);
            else
                ans.push_back(S.query(S.size() - x, S.size() - 1).second);
        };
        // 左端点右移的回调
        auto left_call = [&](int i) {
            auto &cnt = mp[nums[i]];
            if (cnt) S.erase_by_comparator({cnt, 1ll * nums[i] * cnt});
            --cnt;
            if (cnt) S.insert_by_comparator({cnt, 1ll * nums[i] * cnt});
        };
        // 右端点右移的回调
        auto right_call = [&](int i) {
            auto &cnt = mp[nums[i]];
            if (cnt) S.erase_by_comparator({cnt, 1ll * nums[i] * cnt});
            ++cnt;
            if (cnt) S.insert_by_comparator({cnt, 1ll * nums[i] * cnt});
        };
        OY::WINDOW::solve(nums.size(), k, call, left_call, right_call);
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(findXSum);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif