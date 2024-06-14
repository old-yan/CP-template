#include "DS/PersistentLeftist.h"
#include "IO/FastIO.h"

int main() {
    OY::VectorPerLeftistHeap<int, std::less<int>> S;

    // 初始堆含有五个元素
    int A[5] = {400, 200, 500, 800, 600};
    for (int a : A) S.push(a);

    // 第二个堆多了一个元素
    auto S2 = S.copy();
    S2.push(250);

    // 第三个堆少了一个元素
    auto S3 = S.copy();
    S3.pop();

    // 第四个堆是第三个堆和自己的合并，也就是元素数量翻倍
    auto S4 = S3.copy();
    S4.join(S3);

    // 现在我们来观测一下每个堆里的东西
    auto print_and_clear = [&](OY::VectorPerLeftistHeap<int, std::less<int>> &x) {
        while (!x.empty()) {
            cout << x.top() << ' ';
            x.pop();
        }
        cout << '\n';
    };

    cout << "S :" << endl;
    print_and_clear(S);

    cout << "S2 :" << endl;
    print_and_clear(S2);

    cout << "S3 :" << endl;
    print_and_clear(S3);

    cout << "S4 :" << endl;
    print_and_clear(S4);
}
/*
#输出如下
S :
800 600 500 400 200 
S2 :
800 600 500 400 250 200 
S3 :
600 500 400 200 
S4 :
600 600 500 500 400 400 200 200 

*/