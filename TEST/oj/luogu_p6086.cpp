#include "IO/FastIO.h"
#include "TREE/Prufer.h"

/*
[P6086 【模板】Prufer 序列](https://www.luogu.com.cn/problem/P6086)
*/
/**
 * 本题为 Prufer 模板题，有编码和解码两个任务
 */
int main() {
    uint32_t n, type;
    cin >> n >> type;
    if (type == 1) {
        // 编码任务
        OY::PRUFER::Tree<5000000> G(n);
        for (uint32_t i = 0; i + 1 != n; i++) {
            uint32_t p;
            cin >> p;
            G.add_edge(i, p - 1);
        }
        auto code = G.encode();

        uint64_t ans = 0;
        for (uint64_t i = 0; i != n - 2; i++) ans ^= (i + 1) * (code[i] + 1);
        cout << ans;
    } else {
        // 解码任务
        std::vector<uint32_t> code(n - 2);
        for (uint64_t i = 0; i < code.size(); i++) cin >> code[i], code[i]--;
        auto fa = OY::PRUFER::Tree<5000000>::decode(code.begin(), code.end());

        uint64_t ans = 0;
        for (uint64_t i = 0; i != n - 1; i++) ans ^= (i + 1) * (fa[i] + 1);
        cout << ans;
    }
}