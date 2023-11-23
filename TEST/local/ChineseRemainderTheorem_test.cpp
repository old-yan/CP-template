#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem crt(3);
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(4, 7);
    cout << "result = " << crt.query() << endl;
}
/*
#输出如下
result = 53

*/