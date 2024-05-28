#include "DS/InfoQueue.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorInfoQueue<int, std::plus<int>> q;
    q.push(100);
    q.push(300);
    q.push(200);
    cout << "sum = " << q.query_all() << endl;

    q.pop();
    cout << "front = " << q.front() << endl;
    cout << "sum = " << q.query_all() << endl;

    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
    cout << endl;
}

void test_max() {
    auto get_max = [](int x, int y) { return std::max(x, y); };
    OY::GlobalInfoQueue<int, decltype(get_max), 1000> q(get_max);
    q.push(100);
    q.push(30);
    q.push(20);
    cout << "max = " << q.query_all() << endl;

    q.pop();
    cout << "front = " << q.front() << endl;
    cout << "max = " << q.query_all() << endl;

    q.push(80);
    cout << "max = " << q.query_all() << endl;
    
    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
    cout << endl;
}

int main() {
    test();
    test_max();
}
/*
#输出如下
sum = 600
front = 300
sum = 500
300 200

max = 100
front = 30
max = 30
max = 80
30 20 80

*/