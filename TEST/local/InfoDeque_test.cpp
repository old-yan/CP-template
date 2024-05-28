#include "DS/InfoDeque.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorInfoDeque<int, std::plus<int>> q;
    q.push_back(100);
    q.push_back(300);
    q.push_back(200);
    q.push_front(400);
    cout << "sum = " << q.query_all() << endl;

    q.pop_front();
    q.pop_back();
    q.push_front(500);
    q.push_back(600);
    cout << "sum = " << q.query_all() << endl;
    
    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
}

int main() {
    test();
}
/*
#输出如下
sum = 1000
sum = 1500
500 100 300 600

*/