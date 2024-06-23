#include "DS/LinkBucket.h"
#include "IO/FastIO.h"

/*
[Cycle Detection (Directed)](https://judge.yosupo.jp/problem/cycle_detection)(https://github.com/yosupo06/library-checker-problems/issues/534)
*/
/**
 * 检测环，只需要基础的 dfs
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LBC::LinkBucket<std::pair<uint32_t, uint32_t>> L(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        L[a].push_front(std::pair<uint32_t, uint32_t>{b, i});
    }
    std::vector<uint32_t> state(n);
    std::vector<std::pair<uint32_t, uint32_t>> next(n);
    for (uint32_t i = 0; i != n; i++)
        if (!state[i]) {
            auto dfs = [&](auto &self, uint32_t cur, uint32_t idx) -> void {
                state[cur] = 1;
                for (auto &&[to, idx] : L[cur])
                    if (!state[to])
                        next[cur] = {idx, to}, self(self, to, idx);
                    else if (state[to] == 1) {
                        uint32_t u = to, cnt{};
                        next[cur] = {idx, to};
                        std::vector<uint32_t> buf;
                        do {
                            buf.push_back(next[u].first);
                            u = next[u].second;
                        } while (u != to);
                        cout << buf.size() << endl;
                        for (auto e : buf) cout << e << endl;
                        exit(0);
                    }
                state[cur] = 2;
            };
            dfs(dfs, i, -1);
        }
    cout << "-1\n";
}