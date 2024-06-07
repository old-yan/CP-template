#include "DS/InfoDeque.h"
#include "IO/FastIO.h"

/*
[B3656 【模板】双端队列 1](https://www.luogu.com.cn/problem/B3656)
*/
/**
 * 本题为双端队列模板题
 */

using Deque = OY::VectorInfoDeque<uint32_t>;
int main() {
    uint32_t q;
    cin >> q;
    std::vector<Deque> Q;
    while (q--) {
        std::string op;
        uint32_t a;
        cin >> op >> a;
        while (Q.size() < a) Q.resize(a);
        if (op[3] == 'e') {
            cout << Q[a - 1].size() << endl;
        } else if (op[3] == 'k') {
            if (Q[a - 1].empty()) continue;
            cout << Q[a - 1].back() << endl;
        } else if (op[3] == 'n') {
            if (Q[a - 1].empty()) continue;
            cout << Q[a - 1].front() << endl;
        } else if (op[5] == 'b') {
            uint32_t x;
            cin >> x;
            Q[a - 1].push_back(x);
        } else if (op[5] == 'f') {
            uint32_t x;
            cin >> x;
            Q[a - 1].push_front(x);
        } else if (op[4] == 'b') {
            if (Q[a - 1].empty()) continue;
            Q[a - 1].pop_back();
        } else {
            if (Q[a - 1].empty()) continue;
            Q[a - 1].pop_front();
        }
    }
}