#include "DS/LinkBucketHeap.h"
#include "IO/FastIO.h"

int main() {
    int val[8]{};
    auto map = [&](int i) { return val[i]; };
    OY::LinkBucketHeap<100, decltype(map)> q(8, map);
    val[3] = 60;
    q.push(3);
    val[5] = 20;
    q.push(5);
    val[6] = 70;
    q.push(6);
    cout << "top = " << q.top() << endl;
    q.pop();
    cout << "size = " << q.size() << endl;
    cout << "top = " << q.top() << endl;
    val[7] = 130;
    q.push(7);
    cout << "size = " << q.size() << endl;
    cout << "top = " << q.top() << endl;
}
/*
#输出如下
top = 5
size = 2
top = 3
size = 3
top = 3

*/