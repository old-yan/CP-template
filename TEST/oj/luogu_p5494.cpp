#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
*/
/**
 * 本题为线段树分裂/合并模板题
 * 涉及到分裂合并，只能使用动态开点版本，不能使用 Zkw
*/
using Tree = OY::Seg::Tree<OY::Seg::BaseNode<uint64_t>, OY::Seg::Ignore, false, uint32_t, 1 << 22>;
Tree pool[200002];
uint32_t cnt = 2;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    pool[1].resize(n, [&](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') { // 加新
            uint32_t p, x, y;
            cin >> p >> x >> y;
            auto S3 = pool[p].split_by_key(y);
            pool[cnt++] = pool[p].split_by_key(x - 1);
            pool[p].merge(S3);
        } else if (op == '1') { // 合并
            uint32_t p, t;
            cin >> p >> t;
            pool[p].merge(pool[t]);
        } else if (op == '2') { // 添加
            uint32_t p, q;
            uint64_t x;
            cin >> p >> x >> q;
            pool[p].add(q - 1, x);
        } else if (op == '3') { // 查询
            uint32_t p, x, y;
            cin >> p >> x >> y;
            cout << pool[p].query(x - 1, y - 1) << endl;
        } else { // 查询
            uint32_t p;
            uint64_t k;
            cin >> p >> k;
            if (pool[p].query_all() < k)
                cout << "-1\n";
            else
                cout << pool[p].kth(k - 1) + 1 << '\n';
        }
    }
}