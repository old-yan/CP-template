#include "DS/LinkBucket.h"
#include "DS/RollbackDSU.h"
#include "IO/FastIO.h"

/*
[Persistent Unionfind](https://judge.yosupo.jp/problem/persistent_unionfind)(https://github.com/yosupo06/library-checker-problems/issues/405)
*/
/**
 * 本题为可持久化并查集模板题
 * 由于没有在线，所以可以离线用回滚并查集
 */

static constexpr uint32_t N = 200000, M = 200000;
struct Node {
    uint32_t ver, is_query, a, b;
};
uint32_t id[M + 1];
void solve_rollbackdsu() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LBC::Container<Node> buckets(m + 1, m);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= m; i++) {
        char op;
        int k;
        uint32_t u, v;
        cin >> op >> k >> u >> v;
        k++;
        if (op == '0')
            buckets[id[k]].push_front(Node{id[i] = ++cur, false, u, v});
        else
            buckets[id[k]].push_front(Node{i - cur - 1, true, u, v});
    }
    OY::RollbackDSU::Table U(n);
    std::string res(m - cur, ' ');
    auto dfs = [&](auto self, uint32_t cur, uint32_t a, uint32_t b) -> void {
        uint32_t head_a = U.find(a), head_b = U.find(b);
        if (head_a != head_b) {
            if (U.size<true>(head_a) > U.size<true>(head_b)) std::swap(head_a, head_b);
            U.unite_to<false>(head_a, head_b);
        }
        for (auto &&[ver, is_q, a, b] : buckets[cur])
            if (is_q)
                res[ver] = '0' + U.in_same_group(a, b);
            else
                self(self, ver, a, b);
        if (head_a != head_b) U.cancle(head_a, head_b);
    };
    dfs(dfs, 0, 0, 0);
    for (char c : res) cout << c << endl;
}

int main() {
    solve_rollbackdsu();
}