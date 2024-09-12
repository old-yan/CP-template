#include "DS/Queue.h"
#include "IO/FastIO.h"

void test() {
    OY::SumQueue<int> q;
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
    OY::MaxQueue<int> q;
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

void test_semigroup() {
    struct SemiGroup {
        using value_type = std::string;
        static value_type op(std::string x, std::string y) { return x + y; }
    };
    OY::QUE::Queue<SemiGroup> S;
    S.push("h");
    S.push("e");
    S.push("l");
    cout << S << endl;
    S.push(S.back());
    S.push("o");
    cout << S << endl;
    cout << S.query_all() << endl;
}

int main() {
    test();
    test_max();
    test_semigroup();
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

[h, e, l]
[h, e, l, l, o]
hello

*/