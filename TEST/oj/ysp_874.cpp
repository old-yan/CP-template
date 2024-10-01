#include "DS/MinMaxHeap.h"
#include "IO/FastIO.h"

/*
[Double-Ended Priority Queue](https://judge.yosupo.jp/problem/double_ended_priority_queue)(https://github.com/yosupo06/library-checker-problems/issues/874)
*/
/**
 * 大小顶堆模板
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    OY::MMHEAP::Heap<int> H(n, read);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            int x;
            cin >> x;
            H.push(x);
        } else if (op == '1') {
            auto x = H.top_min();
            cout << x << endl;
            H.pop_min();
        } else {
            auto x = H.top_max();
            cout << x << endl;
            H.pop_max();
        }
    }
}