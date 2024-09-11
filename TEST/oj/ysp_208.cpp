#include "DS/Queue.h"
#include "IO/FastIO.h"

/*
[Queue Operate All Composite](https://judge.yosupo.jp/problem/queue_operate_all_composite)(https://github.com/yosupo06/library-checker-problems/issues/208)
*/
/**
 * 本题为带有信息聚合的队列模板
 * 用两个栈相互倒，即可实现
 */

static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};

int main() {
    uint32_t q;
    cin >> q;
    // 两个方向互相倒的栈，向左不会加，向右一直加可以加 50万 个元素。所以需要总共 50万 的空间
    // 懒得重写 Monoid，所以直接把 VectorSumQueue 的 monoid 借来
    using monoid = OY::SumQueue<node>::monoid;
    OY::GlobalQueue<monoid, 500000> Q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b;
            cin >> a >> b;
            Q.push({a, b});
        } else if (op == '1')
            Q.pop();
        else {
            uint32_t x;
            cin >> x;
            cout << (Q.empty() ? x : Q.query_all().calc(x)) << endl;
        }
    }
}
