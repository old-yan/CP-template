#include "DS/Leftist.h"
#include "IO/FastIO.h"

/*
[P3378 【模板】堆](https://www.luogu.com.cn/problem/P3378)
*/
/**
 * 本题为堆的模板题
*/
OY::LeftistHeap<uint32_t, std::greater<uint32_t>, 1000001> S;
int main() {
    uint32_t n;
    cin >> n;
    while (n--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.push(x);
        } else if (op == '2') {
            cout << S.top() << endl;
        } else
            S.pop();
    }
}