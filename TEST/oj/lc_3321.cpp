#include "DS/ErasableMonoPairHeap.h"
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
 * 当然效率最高的是对顶堆
 */

class Solution {
    vector<long long> solve_heap(vector<int> &nums, int k, int x) {
        // 本平衡树维护两个属性的和
        // 属性一是频率
        // 属性二是键和频率的乘积
        struct Monoid {
            using value_type = pair<int, long long>;
            struct sum_type {
                long long v;
                sum_type() = default;
                sum_type(long long v) : v(v) {}
                sum_type(value_type v) : v(v.second) {}
                operator value_type() const { return {}; }
            };
            static sum_type op(const sum_type &x, const sum_type &y) { return {x.v + y.v}; }
            static sum_type identity() { return {}; }
            static sum_type inverse(const sum_type &x) { return {-x.v}; }
        };
        static bool init = false;
        if (!init) init = true, OY::EMONOPH::Heap<Monoid, less<>>::_reserve(200000), OY::EMONOPH::Heap<Monoid, greater<>>::_reserve(400000);
        OY::EMONOPH::Heap<Monoid, less<>> smaller;
        OY::EMONOPH::Heap<Monoid, greater<>> bigger;
        auto erase = [&](pair<int, long long> &&e) {
            if (!smaller.empty() && e <= smaller.top())
                smaller.erase(e);
            else {
                bigger.erase(e);
                if (!smaller.empty()) bigger.push(smaller.top()), smaller.pop();
            }
        };
        auto insert = [&](pair<int, long long> &&e) {
            bigger.push(e);
            if (bigger.size() > x) smaller.push(bigger.top()), bigger.pop();
        };
        // mp 维护频率
        unordered_map<int, int> mp;
        vector<long long> ans;
        // 给固定大小的滑动窗口传递回调函数
        // 对已就绪的窗口调用的回调
        auto call = [&](int l, int r) {
            ans.push_back(bigger.query_all().v);
        };
        // 左端点右移的回调
        auto left_call = [&](int i) {
            auto &cnt = mp[nums[i]];
            if (cnt) erase({cnt, 1ll * nums[i] * cnt});
            --cnt;
            if (cnt) insert({cnt, 1ll * nums[i] * cnt});
        };
        // 右端点右移的回调
        auto right_call = [&](int i) {
            auto &cnt = mp[nums[i]];
            if (cnt) erase({cnt, 1ll * nums[i] * cnt});
            ++cnt;
            if (cnt) insert({cnt, 1ll * nums[i] * cnt});
        };
        OY::WINDOW::solve(nums.size(), k, call, left_call, right_call);
        return ans;
    }
    vector<long long> solve_avl(vector<int> &nums, int k, int x) {
        // 本平衡树维护两个属性的和
        // 属性一是频率
        // 属性二是键和频率的乘积
        struct Monoid {
            using value_type = pair<int, long long>;
            struct sum_type {
                long long v;
                sum_type() = default;
                sum_type(long long v) : v(v) {}
                sum_type(value_type v) : v(v.second) {}
                operator value_type() const { return {}; }
            };
            static sum_type op(const sum_type &x, const sum_type &y) { return {x.v + y.v}; }
            static sum_type identity() { return {}; }
        };
        // using Tree = OY::MONOAVL::Tree<Monoid, false>;
        using Tree = OY::MONOSPLAY::Tree<Monoid, false>;
        Tree S;
        // mp 维护频率
        unordered_map<int, int> mp;
        vector<long long> ans;
        // 给固定大小的滑动窗口传递回调函数
        // 对已就绪的窗口调用的回调
        auto call = [&](int l, int r) {
            if (S.size() < x)
                ans.push_back(S.query_all().v);
            else
                ans.push_back(S.query(S.size() - x, S.size() - 1).v);
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

public:
    vector<long long> findXSum(vector<int> &nums, int k, int x) {
        return solve_heap(nums, k, x);
        // return solve_avl(nums, k, x);
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