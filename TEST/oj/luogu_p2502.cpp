#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

/*
[P2502 [HAOI2006] 旅行](https://www.luogu.com.cn/problem/P2502)
*/
/**
 * 本题可以在最小值单调递降的情况下追加边，贪心求最小的最大边
 */

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    struct Edge {
        uint32_t u, v, speed;
    };
    std::vector<Edge> es(m);
    for (auto &[u, v, speed] : es) cin >> u >> v >> speed, u--, v--;
    std::ranges::sort(es, [](auto &x, auto &y) {
        return x.speed > y.speed;
    });
    uint32_t s, t;
    cin >> s >> t, s--, t--;

    OY::StaticDynamicSPFA<OY::DSPFA::MaxGroup<uint32_t>, false, 500, 10000> G(n, m * 2);
    G.set_distance(s, 0);
    struct Ratio {
        uint32_t a, b;
        bool operator<(const Ratio &rhs) const {
            return a * rhs.b < rhs.a * b;
        }
    };
    Ratio ans{40000, 1};
    for (auto &[u, v, speed] : es) {
        G.update_by_edge(u, v, speed);
        G.update_by_edge(v, u, speed);
        if (G.query(t) != G.infinite())
            ans = std::min(ans, Ratio{G.query(t), speed});
    }
    if (ans.a == 40000)
        cout << "IMPOSSIBLE";
    else {
        auto g = std::gcd(ans.a, ans.b);
        ans.a /= g, ans.b /= g;
        if (ans.b == 1)
            cout << ans.a;
        else
            cout << ans.a << '/' << ans.b;
    }
}

int main() {
    solve_spfa();
}