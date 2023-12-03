#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[P4151 [WC2011] 最大XOR和路径](https://www.luogu.com.cn/problem/P4151)
*/
/**
 * 本题可以使用线性基解决
 * dfs 遇到非树边时，可以与树边组成环，将环长加入线性基
 */

static constexpr uint32_t N = 50000, M = 100000;
struct Node {
    uint32_t m_to;
    uint64_t m_dis;
};
using graph = OY::LBC::LinkBucket<Node, N, M * 2>;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    graph G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        uint64_t dis;
        cin >> a >> b >> dis;
        G[a - 1].push_back(Node{b - 1, dis});
        G[b - 1].push_back(Node{a - 1, dis});
    }

    OY::StaticHamelXorBase64<60> hxb{};
    std::vector<uint64_t> dis(n, -1);
    auto dfs = [&](auto self, uint32_t cur, uint64_t cur_dis) -> void {
        dis[cur] = cur_dis;
        for (auto &&[to, d] : G[cur])
            if (~dis[to]) {
                uint64_t loop = dis[to] ^ (cur_dis ^ d);
                hxb.insert(loop);
            } else
                self(self, to, cur_dis ^ d);
    };
    dfs(dfs, 0, 0);

    cout << hxb.query_max_bitxor(dis[n - 1]);
}