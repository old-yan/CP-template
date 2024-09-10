#include "DS/RangeManager.h"
#include "IO/FastIO.h"

int main() {
    OY::RangeManager<int> ranges;

    // 一般来说，回调可以写成匿名函数，而且 add_range 和 remove_range 的回调通用
    auto add_call = [&](uint32_t l, uint32_t r) {
        cout << "range [" << l << ", " << r << "] is added\n";
    };
    auto remove_call = [&](uint32_t l, uint32_t r) {
        cout << "range [" << l << ", " << r << "] is removed\n";
    };

    // 添加区间
    ranges.add_range({1, 10}, add_call, remove_call);
    ranges.add_range({21, 30}, add_call, remove_call);
    ranges.add_range({5, 15}, add_call, remove_call);
    cout << "after add_range:" << ranges << endl;

    // 移除区间
    ranges.remove_range({13, 25}, add_call, remove_call);
    cout << "after remove_range:" << ranges << endl;

    // 维护区间总长度
    cout << "length of ranges = " << ranges.length() << endl;
}
/*
#输出如下
range [1, 10] is added
range [21, 30] is added
range [1, 10] is removed
range [1, 15] is added
after add_range:[[1, 15], [21, 30]]
range [1, 15] is removed
range [1, 12] is added
range [21, 30] is removed
range [26, 30] is added
after remove_range:[[1, 12], [26, 30]]
length of ranges = 17

*/