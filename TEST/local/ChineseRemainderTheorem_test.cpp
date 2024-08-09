#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem crt(3);
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(4, 7);

    // 结果是个 pair
    auto res = crt.query();
    // 最小答案
    auto min = res.m_rem;
    // 答案的周期
    auto cycle = res.m_div;

    // min, min+cycle, min+cycle*2 ... 均为可能的答案
    cout << "first result = " << min << endl;
    cout << "second result = " << min + cycle << endl;
    cout << "third result = " << min + cycle * 2 << endl;
}
/*
#输出如下
first result = 53
second result = 158
third result = 263

*/