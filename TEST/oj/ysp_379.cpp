#include "DS/LinkBucket.h"
#include "IO/FastIO.h"

/*
[Persistent Queue](https://judge.yosupo.jp/problem/persistent_queue)(https://github.com/yosupo06/library-checker-problems/issues/379)
*/
/**
 * 本题为可持久化队列模板题
 * 由于没有在线，所以可以离线用回滚队列
 */

static constexpr uint32_t Q = 500000;
struct Node {
    uint32_t ver, at_back, x;
};
OY::LBC::LinkBucket<Node> buckets;
uint32_t buf[Q];
void solve_rollback() {
    uint32_t q;
    cin >> q;
    buckets.resize(q + 1, q);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= q; i++) {
        char op;
        int t;
        cin >> op >> t;
        t++;
        if (op == '0') {
            uint32_t x;
            cin >> x;
            buckets[t].push_front(Node{i, true, x});
        } else
            buckets[t].push_front(Node{i, false, cur++});
    }
    uint32_t l = 0, r = 0;
    std::vector<uint32_t> res(cur);
    auto dfs = [&](auto self, uint32_t cur, uint32_t at_back, uint32_t x) -> void {
        if (at_back)
            buf[r++] = x;
        else if (~x)
            res[x] = buf[l++];
        for (auto &&[ver, at_back, x] : buckets[cur]) self(self, ver, at_back, x);
        if (at_back)
            r--;
        else if (~x)
            buf[--l] = res[x];
    };
    dfs(dfs, 0, 0, -1);
    for (auto c : res) cout << c << endl;
}

int main() {
    solve_rollback();
}