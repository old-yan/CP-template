#include "GRAPH/EulerPath_dg.h"
#include "IO/FastIO.h"

/*
[P1127 词链](https://www.luogu.com.cn/problem/P1127)
*/
/**
 * 将每种字母视为结点，将单词视为连接两个结点的有向边
 * 则本体可以视为要找出有向图的欧拉路径
 */

static constexpr uint32_t N = 26, M = 1000;
int main() {
    uint32_t n;
    cin >> n;
    // 先对边排序
    std::vector<std::string> words(n);
    for (uint32_t i = 0; i < n; i++) cin >> words[i];
    std::sort(words.begin(), words.end());

    OY::EulerPathDG::Graph<N, M> G(26, n);
    for (uint32_t i = 0; i < n; i++) G.add_edge(words[i][0] - 'a', words[i].back() - 'a');

    auto sol = G.calc();
    if (sol.is_Euler_graph() || sol.is_half_Euler_graph()) {
        bool started = false;
        sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
            if (started)
                cout << '.';
            else
                started = true;
            cout << words[index];
        });
    } else
        cout << "***";
}