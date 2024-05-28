#include "DS/SqrtTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P7261 [COCI2009-2010#3] PATULJCI](https://www.luogu.com.cn/problem/P7261)
*/
/**
 * 本题要求绝对众数
 * 静态问题上，可以用 SqrtTree 解决
 * 也可以用小波树求区间中位数
 */

// 把加法实现为摩尔投票
struct item {
    uint32_t m_val, m_cnt;
    item() = default;
    item(uint32_t x) : m_val{x}, m_cnt{1} {}
    item(uint32_t x, uint32_t cnt) : m_val{x}, m_cnt{cnt} {}
    item operator+(const item &rhs) const {
        if (m_val == rhs.m_val) return {m_val, m_cnt + rhs.m_cnt};
        if (m_cnt >= rhs.m_cnt)
            return {m_val, m_cnt - rhs.m_cnt};
        else
            return {rhs.m_val, rhs.m_cnt - m_cnt};
    }
};
void solve_sqrt() {
    uint32_t n, c;
    cin >> n >> c;
    std::vector<std::vector<uint32_t>> pos(c + 1);
    OY::Sqrt::Table<OY::Sqrt::CustomNode<item, std::plus<item>>, OY::Sqrt::RandomController<>, 10> S(n, [&](uint32_t i) {
        uint32_t x;
        cin >> x;
        pos[x].push_back(i);
        return x;
    });
    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        auto res = S.query(l - 1, r - 1).m_val;
        auto it1 = std::lower_bound(pos[res].begin(), pos[res].end(), l - 1);
        auto it2 = std::lower_bound(pos[res].begin(), pos[res].end(), r);
        if (it2 - it1 > (r - l + 1) / 2)
            cout << "yes " << res << endl;
        else
            cout << "no\n";
    }
}

void solve_wavelet() {
    uint32_t n, c;
    cin >> n >> c;
    OY::WaveLet::Table<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        uint32_t len = r - l + 1;
        auto res = S.quantile(l - 1, r - 1, len / 2);
        if (S.count(l - 1, r - 1, res) > len / 2)
            cout << "yes " << res << endl;
        else
            cout << "no\n";
    }
}

int main() {
    solve_sqrt();
    // solve_wavelet();
}