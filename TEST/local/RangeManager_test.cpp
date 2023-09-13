#include "DS/RangeManager.h"
#include "IO/FastIO.h"

int main() {
    OY::RangeManager<int> ranges;
    // 添加区间
    ranges.add_range({1, 10});
    ranges.add_range({21, 30});
    ranges.add_range({5, 15});
    cout << "after add_range:" << ranges << endl;

    // 移除区间
    // 结构化绑定需要 C++17
#if CPP_STANDARD >= 201702L
    auto [deleted, inserted] = ranges.remove_range({13, 25});
#else
    auto deleted_and_inserted = ranges.remove_range({13, 25});
    auto deleted = deleted_and_inserted.first;
    auto inserted = deleted_and_inserted.second;
#endif
    cout << "after remove_range:" << ranges << endl;

    cout << "during removing,\n";
    // 结构化绑定需要 C++17
#if CPP_STANDARD >= 201702L
    for (auto &[l, r] : deleted) {
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &[l, r] : inserted) {
        cout << '[' << l << ',' << r << "] is inserted\n";
    }
#else
    for (auto &l_and_r : deleted) {
        auto l = l_and_r.first, r = l_and_r.second;
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &l_and_r : inserted) {
        auto l = l_and_r.first, r = l_and_r.second;
        cout << '[' << l << ',' << r << "] is inserted\n";
    }
#endif

    // 维护区间总长度
    cout << "length of ranges = " << ranges.m_length << endl;
}
/*
#输出如下
after add_range:[[1, 15], [21, 30]]
after remove_range:[[1, 12], [26, 30]]
during removing,
[1,15] is deleted
[21,30] is deleted
[1,12] is inserted
[26,30] is inserted
length of ranges = 17

*/