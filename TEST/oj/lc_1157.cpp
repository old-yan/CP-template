#include "DS/SqrtTree.h"
#include "DS/WaveLet.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[1157. 子数组中占绝大多数的元素](https://leetcode.cn/problems/online-majority-element-in-subarray/)
*/
/**
 * 本题要对静态区间求绝对众数，可以用摩尔投票线段树解决
 * 也可以用小波树取出区间中位数
 */

// 把加法实现为摩尔投票
struct item {
    int m_val, m_cnt;
    item() = default;
    item(int x) : m_val{x}, m_cnt{1} {}
    item(int x, int cnt) : m_val{x}, m_cnt{cnt} {}
    item operator+(const item &rhs) const {
        if (m_val == rhs.m_val) return {m_val, m_cnt + rhs.m_cnt};
        if (m_cnt >= rhs.m_cnt)
            return {m_val, m_cnt - rhs.m_cnt};
        else
            return {rhs.m_val, rhs.m_cnt - m_cnt};
    }
};

class MajorityChecker_solve1 {
    OY::SqrtSumTable<item, OY::SQRT::RandomController<>, 12> m_table;
    vector<vector<int>> m_pos;

public:
    MajorityChecker_solve1(vector<int> &arr) : m_table(arr.begin(), arr.end()) {
        int mx = *max_element(arr.begin(), arr.end());
        m_pos.resize(mx + 1);
        for (int i = 0; i < arr.size(); i++) m_pos[arr[i]].push_back(i);
    }
    int query(int left, int right, int threshold) {
        auto res = m_table.query(left, right).m_val;
        auto it1 = lower_bound(m_pos[res].begin(), m_pos[res].end(), left);
        auto it2 = lower_bound(m_pos[res].begin(), m_pos[res].end(), right + 1);
        return it2 - it1 >= threshold ? res : -1;
    }
};

class MajorityChecker_solve2 {
    OY::WaveLet::Table<uint32_t> m_table;

public:
    MajorityChecker_solve2(vector<int> &arr) : m_table(arr.begin(), arr.end()) {}
    int query(int left, int right, int threshold) {
        auto res = m_table.quantile(left, right, (right - left + 1) / 2);
        return m_table.count(left, right, res) >= threshold ? res : -1;
    }
};

using MajorityChecker = MajorityChecker_solve1;
// using MajorityChecker = MajorityChecker_solve2;

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_CLASS(MajorityChecker, vector<int> &);
    REGISTER_MEMBERFUNCTION_CLASS(MajorityChecker, query);
    while (true) {
        executor.constructClass();
        while (executor) {
            executor.executeClass();
        }
    }
}
#endif