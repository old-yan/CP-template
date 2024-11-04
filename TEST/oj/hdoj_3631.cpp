#include "GRAPH/DynamicFloyd.h"
#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

/*
[Shortest Path](https://acm.hdu.edu.cn/showproblem.php?pid=3631)
*/
/**
 * 本题需要一个支持动态加点的 floyd
 * 在一开始就可以把所有的边插入进去，然后当解锁某个点的时候，调用 update_by_vertex
 * 
 * 也可以使用支持动态加边的 spfa
 * 这就不能在一开始就把所有的边插入进去
 * 而是，当解锁某个点的时候，把这个点的邻边加进去
 */

static constexpr uint32_t N = 300, M = 100000;
void solve_floyd() {
    for (uint32_t t = 1;; t++) {
        uint32_t n, m, q;
        cin >> n >> m >> q;
        if (!n) break;
        OY::StaticDynamicFloyd<OY::DFLOYD::AddGroup<uint32_t>, false, N> G(n);
        // OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<uint32_t>, false> G(n);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, c;
            cin >> a >> b >> c;
            G.add_edge(a, b, c);
        }

        std::vector<bool> marked(n);
        cout << "Case " << t << ":\n";
        for (uint32_t i = 0; i != q; i++) {
            char op;
            cin >> op;
            if (op == '0') {
                uint32_t x;
                cin >> x;
                if (marked[x])
                    cout << "ERROR! At point " << x << endl;
                else {
                    marked[x] = true;
                    G.update_by_vertex(x);
                }
            } else {
                uint32_t x, y;
                cin >> x >> y;
                if (!marked[x] || !marked[y])
                    cout << "ERROR! At path " << x << " to " << y << endl;
                else if (G.query(x, y) == G.infinite())
                    cout << "No such path\n";
                else
                    cout << G.query(x, y) << endl;
            }
        }
    }
}

uint32_t edges[N][N];
void solve_spfa() {
    for (uint32_t t = 1;; t++) {
        uint32_t n, m, q;
        cin >> n >> m >> q;
        if (!n) break;
        OY::StaticDynamicSPFAs<OY::DSPFA::AddGroup<uint32_t>, false, N, M> G(n, m);
        // OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<uint32_t>, false> G(n);
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++) edges[i][j] = G.infinite();
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, c;
            cin >> a >> b >> c;
            edges[a][b] = std::min(edges[a][b], c);
        }

        std::vector<bool> marked(n);
        cout << "Case " << t << ":\n";
        for (uint32_t i = 0; i != q; i++) {
            char op;
            cin >> op;
            if (op == '0') {
                uint32_t x;
                cin >> x;
                if (marked[x])
                    cout << "ERROR! At point " << x << endl;
                else {
                    marked[x] = true;
                    for (uint32_t r = 0; r != n; r++)
                        if (marked[r]) G.update_by_edge(r, x, edges[r][x]);
                    for (uint32_t c = 0; c != n; c++)
                        if (marked[c]) G.update_by_edge(x, c, edges[x][c]);
                }
            } else {
                uint32_t x, y;
                cin >> x >> y;
                if (!marked[x] || !marked[y])
                    cout << "ERROR! At path " << x << " to " << y << endl;
                else if (G.query(x, y) == G.infinite())
                    cout << "No such path\n";
                else
                    cout << G.query(x, y) << endl;
            }
        }
    }
}

int main() {
    solve_floyd();
    // solve_spfa();
}
