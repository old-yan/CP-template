#include "IO/FastIO.h"
#include "GRAPH/HLPP.h"

int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;
    OY::FlowNetwork<uint64_t> G(n, m);
    for (int i = 0; i < m; i++) {
        uint32_t a, b;
        uint64_t c;
        cin >> a >> b >> c;
        G.addEdge(a - 1, b - 1, c);
    }
    G.prepareSorted([](const auto &x, const auto &y) { return x.value > y.value; });
    OY::HLPP sol(G, s - 1, t - 1);
    cout << sol.calc();
}