#include "DS/IntegerSet.h"
#include "IO/FastIO.h"

int main() {
    OY::INTSET::Table<10> B;
    // 全部置为 1
    B.set();
    cout << "B.count() = " << B.count() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.at(i) << endl;
    // 全部置为 0
    B.reset();
    cout << "B.count() = " << B.count() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.at(i) << endl;

    B.set(16);
    B.set(980);
    B.set(357);
    B.set(2);
    for (int i = B.lower_bound(0); i >= 0; i = B.lower_bound(i + 1))
        cout << i << ' ';
    cout << endl;
    for (int i = B.smaller_bound(B.size()); i >= 0; i = B.smaller_bound(i))
        cout << i << ' ';
    cout << endl;
}
/*
#输出如下
B.count() = 4096
index 139:1
index 189:1
index 239:1
index 289:1
index 339:1
index 389:1
B.count() = 0
index 139:0
index 189:0
index 239:0
index 289:0
index 339:0
index 389:0
2 16 357 980 
980 357 16 2 

*/