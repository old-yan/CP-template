#include "DS/GlobalUnorderedDiscretizer.h"
#include "IO/FastIO.h"

OY::GlobalUnorderedDiscretizer<int, 1 << 10> D;
int main() {
    cout << "22 -> " << D.insert(22) << endl;
    cout << "33 -> " << D.insert(33) << endl;
    cout << "11 -> " << D.insert(11) << endl;

    // 输出一下存在的元素
    cout << "elems: ";
    D.do_for_each([](int x) {
        cout << x << ' ';
    });
    cout << endl;
}
/*
#输出如下
22 -> 0
33 -> 1
11 -> 2
elems: 22 33 11 

*/