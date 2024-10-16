#include "DS/MonoPairHeap.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[P3378 【模板】堆](https://www.luogu.com.cn/problem/P3378)
*/
/**
 * 本题为堆的模板题
 */

uint32_t buf[1 << 20];
void solve_std() {
    uint64_t n, x{};
    cin >> n;
    for (uint64_t i = 0; i != n; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            cin >> buf[x++];
            std::push_heap(buf, buf + x, std::greater<uint32_t>());
        } else if (op == '2') {
            cout << buf[0] << endl;
        } else
            std::pop_heap(buf, buf + x--, std::greater<uint32_t>());
    }
}

void solve_monoph() {
    uint32_t n;
    cin >> n;
    OY::MonoPairHeap<uint32_t, std::greater<uint32_t>, OY::StaticBufferWrapWithCollect<1000001>::type> S;
    while (n--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.push(x);
        } else if (op == '2')
            cout << S.top() << endl;
        else
            S.pop();
    }
}

int main() {
    solve_std();
    // solve_monoph();
}