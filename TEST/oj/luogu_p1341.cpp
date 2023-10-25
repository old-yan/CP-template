#include "GRAPH/EulerPath_udg.h"
#include "IO/FastIO.h"

/*
[P1341 无序字母对](https://www.luogu.com.cn/problem/P1341)
*/

static constexpr uint32_t N = 52, M = 52 * 52;
struct Edge {
    char from, to;
    bool operator<(const Edge &rhs) const {
        return from + to < rhs.from + rhs.to;
    }
} e[M];
auto mapping = [](char c) {
    return c <= 'Z' ? c - 'A' : c - 'a' + 26;
};
auto mapping2 = [](int c) -> char {
    return c < 26 ? c + 'A' : c - 26 + 'a';
};
int main() {
    uint32_t n;
    cin >> n;

    // 由于题目特殊要求，必须要把边排序
    for (uint32_t i = 0; i < n; i++) cin >> e[i].from >> e[i].to;
    std::sort(e, e + n);

    OY::EulerPathUDG::Graph<N, M> G(N, n);
    for (uint32_t i = 0; i < n; i++) G.add_edge(mapping(e[i].from), mapping(e[i].to));

    auto sol = G.calc();
    if (!sol.is_Euler_graph() && !sol.is_half_Euler_graph())
        cout << "No Solution";
    else {
        cout << mapping2(sol.get_source());
        sol.trace([](uint32_t index, uint32_t from, uint32_t to) {
            cout << mapping2(to);
        });
    }
}