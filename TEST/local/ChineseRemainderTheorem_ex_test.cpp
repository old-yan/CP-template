#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem_ex.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem_ex crt;
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(2, 7);

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

    // 如果有数据合法性的问题，就检测 add 的返回值
    // “今有物不知其数，三三数之剩二，六六数之剩一。问物几何？”
    OY::ChineseRemainderTheorem_ex crt2;
    bool valid = true;
    if (!crt2.add(2, 3)) valid = false;
    if (!crt2.add(1, 6)) valid = false;
    if (valid) {
        cout << "min result = " << crt2.query().m_rem << endl;
    } else {
        cout << "Impossible!\n";
    }
}
/*
#输出如下
first result = 23
second result = 128
third result = 233
Impossible!

*/