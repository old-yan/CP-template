#include "DS/AVL.h"
#include "DS/PairHeap.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[U361730 【模板】完全体·堆](https://www.luogu.com.cn/problem/U361730)
*/
/**
 * 本题为堆的模板题
 * 配对堆不能做删除操作也不能做 decrease 操作，但是可以使用惰性删除，类似 Dijkstra 中的做法
 */

static constexpr uint32_t N = 1000000, M = 1000000;
struct Pair {
    uint32_t m_index;
    int m_val;
    bool operator<(const Pair &rhs) const { return m_val > rhs.m_val; }
};
int val[N + 1];
using Heap = OY::PHeap::Heap<OY::PHeap::BaseNodeWrapper<Pair, std::less<Pair>>::type, OY::StaticBufferWrapWithCollect<N + M>::type>;
Heap H[N + 1];
void solve_heap() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 1; i <= n; i++) {
        int x;
        cin >> x;
        val[i] = x;
        H[i].push({i, x});
    }
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t x, y;
            cin >> x >> y;
            val[y] = 0x80000000;
        } else if (op == '1') {
            uint32_t x;
            cin >> x;
            while (true)
                if (auto p = H[x].root(); p->m_val.m_val > val[p->m_val.m_index])
                    H[x].pop();
                else
                    break;
            cout << H[x].top().m_val << endl;
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            H[x].join(H[y]);
        } else {
            uint32_t x, y;
            int z;
            cin >> x >> y >> z;
            val[y] = z;
            H[x].push({y, z});
        }
    }
}

template <typename Node>
struct AVLNodeWrap {
    using key_type = Pair;
    key_type m_key;
    static bool comp(const key_type &x, const key_type &y) {
        if (x.m_val != y.m_val)
            return x.m_val < y.m_val;
        else
            return x.m_index < y.m_index;
    }
    void set(const key_type &key) { m_key = key; }
    const key_type &get() const { return m_key; }
};
using AVL = OY::AVL::Tree<AVLNodeWrap, N + M>;
AVL F[N + 1];
void solve_avl() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 1; i <= n; i++) {
        int x;
        cin >> x;
        val[i] = x;
        F[i].insert_by_key({i, x});
    }
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t x, y;
            cin >> x >> y;
            val[y] = 0x80000000;
        } else if (op == '1') {
            uint32_t x;
            cin >> x;
            while (true)
                if (auto p = F[x].kth(0)->get(); p.m_val > val[p.m_index])
                    F[x].erase_by_rank(0);
                else {
                    cout << p.m_val << endl;
                    break;
                }
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            if (F[x].size() < F[y].size()) std::swap(F[x], F[y]);
            F[y].do_for_each([&](AVL::node *p) {
                p->m_lc = p->m_rc = 0;
                F[x].insert_node_by_key(p);
            });
        } else {
            uint32_t x, y;
            int z;
            cin >> x >> y >> z;
            val[y] = z;
            F[x].insert_by_key({y, z});
        }
    }
}

int main() {
    solve_heap();
    // solve_avl();
}