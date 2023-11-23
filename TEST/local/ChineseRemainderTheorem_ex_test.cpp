#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem_ex.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem_ex crt;
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(2, 7);
    cout << "result = " << crt.query() << endl;

    // 如果有数据合法性的问题，就检测 add 的返回值
    // “今有物不知其数，三三数之剩二，六六数之剩一。问物几何？”
    OY::ChineseRemainderTheorem_ex crt2;
    bool valid = true;
    if (!crt2.add(2, 3)) valid = false;
    if (!crt2.add(1, 6)) valid = false;
    if (valid) {
        cout << "result = " << crt2.query() << endl;
    } else {
        cout << "Impossible!\n";
    }
}
/*
#输出如下
result = 23
Impossible!

*/