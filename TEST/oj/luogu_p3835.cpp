#include "DS/PersistentFHQTreap.h"
#include "IO/FastIO.h"

/*
[P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
*/
/**
 * 本题为 P3369 可持久化版本
 * 为可持久化平衡树模板题
 */
using Tree = OY::PerFHQTreap<int, std::less<int>, true, 11000000>;
Tree pool[500001];
int main() {
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            pool->unlock();
            pool[i] = pool[v].copy();
            pool[i].insert_by_key(x);
            pool->lock();
        } else if (opt == '2') {
            pool->unlock();
            pool[i] = pool[v].copy();
            pool[i].erase_by_key(x);
            pool->lock();
        } else {
            pool[i] = pool[v];
            int res = 0;
            if (opt == '3')
                res = pool[i].rank(x) + 1;
            else if (opt == '4')
                res = pool[i].kth(x - 1)->get();
            else if (opt == '5') {
                auto p = pool[i].smaller_bound(x);
                res = p->is_null() ? -2147483647 : p->get();
            } else {
                auto p = pool[i].upper_bound(x);
                res = p->is_null() ? 2147483647 : p->get();
            }
            cout << res << endl;
        }
    }
}