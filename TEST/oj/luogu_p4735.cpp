#include "DS/BiTrie.h"
#include "IO/FastIO.h"

/*
[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)
*/
/**
 * 本题可以通过离线，转化为普通的 01 字典树问题
*/
struct node {
    uint32_t m_pos;
};
static constexpr uint32_t N = 300000, M = 300000, L = 24;
using Tree = OY::BiTrie32<L, node, (N + M) * L + 1>;
uint32_t A[N + M + 1], ans[M];

struct Query {
    uint32_t x, i, l, r;
    bool operator<(const Query &rhs) const {
        return r < rhs.r;
    }
} Qs[M];
uint32_t id;
// 离线处理所有查询
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 1; i <= n; i++) cin >> A[i], A[i] ^= A[i - 1];
    uint32_t len = n;
    uint32_t j = 0;
    for (uint32_t i = 0; i < m; i++) {
        char c;
        cin >> c;
        if (c == 'A') {
            uint32_t x;
            cin >> x;
            A[len + 1] = A[len] ^ x;
            len++;
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            x ^= A[len];
            Qs[id].x = x, Qs[id].i = j++, Qs[id].l = l, Qs[id].r = r;
            id++;
        }
    }
    std::sort(Qs, Qs + id);
    Tree S;
    uint32_t cur = 0;
    for (uint32_t r = 1; r <= len; r++) {
        auto p = S.insert(A[r - 1], [r](auto it) { it->m_pos = r; });
        while (cur != id and Qs[cur].r == r) {
            auto &&[x, i, l, r] = Qs[cur++];
            auto res = S.query_max_bitxor(x, [l](auto it) { return it.m_index && it->m_pos >= l; });
            ans[i] = res.second;
        }
    }
    for (uint32_t i = 0; i < j; i++) cout << ans[i] << '\n';
}