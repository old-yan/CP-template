#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

/*
[P1997 faebdc 的烦恼](https://www.luogu.com.cn/problem/P1997)
*/
/**
 * 本题表面上是求众数
 * 要对区间里的连续元素形成的段长进行最值统计
 * 段长是可以合并的，可以用线段树处理
 * 静态问题上，可以用 SqrtTree 解决
 */

struct item {
    int m_l, m_r;
    uint32_t m_lcnt, m_rcnt, m_len, m_max;
    item() = default;
    item(int x) : m_l{x}, m_r{x}, m_lcnt{1}, m_rcnt{1}, m_len{1}, m_max{1} {}
    item operator+(const item &rhs) const {
        item res;
        res.m_l = m_l;
        res.m_lcnt = m_lcnt == m_len and m_l == rhs.m_l ? m_lcnt + rhs.m_lcnt : m_lcnt;
        res.m_r = rhs.m_r;
        res.m_rcnt = rhs.m_rcnt == rhs.m_len and rhs.m_r == m_r ? m_rcnt + rhs.m_rcnt : rhs.m_rcnt;
        res.m_len = m_len + rhs.m_len;
        res.m_max = std::max({m_max, rhs.m_max});
        if (m_r == rhs.m_l) res.m_max = std::max(res.m_max, m_rcnt + rhs.m_lcnt);
        return res;
    }
};

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::Sqrt::Table<OY::Sqrt::CustomNode<item, std::plus<item>>, OY::Sqrt::RandomController<>, 10> S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1).m_max << endl;
    }
}