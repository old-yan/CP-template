#include "DS/ChthollyZkwTree.h"
#include "DS/ChthollySegTree.h"
#include "IO/FastIO.h"

/*
[P2787 语文1（chin1）- 理理思维](https://www.luogu.com.cn/problem/P2787)
*/
/**
 * 操作一需要区间信息查询
 * 操作二需要一个区间推平
 * 操作三可以看成操作一和操作二的结合
 */

void solve_czkw() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;
    struct Segment {
        using value_type = char;
        uint16_t m_cnt[26]{};
        Segment() = default;
        Segment(char color) { m_cnt[color - 'A']++; }
        Segment operator+(const Segment &rhs) const {
            Segment res;
            for (uint32_t i = 0; i != 26; i++) res.m_cnt[i] = m_cnt[i] + rhs.m_cnt[i];
            return res;
        }
        Segment operator*(uint32_t n) const {
            Segment res;
            for (uint32_t i = 0; i != 26; i++) res.m_cnt[i] = m_cnt[i] * n;
            return res;
        }
        bool has_change() const {
            return std::count_if(m_cnt, m_cnt + 26, [](auto v) { return v; }) > 1;
        }
        char get() const {
            return 'A' + (std::find_if(m_cnt, m_cnt + 26, [](auto v) { return v; }) - m_cnt);
        }
    };
    OY::ChthollyFastPowZkwTree<Segment> S(n, 'A'); // 1.17s
    // OY::VectorChthollyFastPowSegTree<Segment> S(n, 'A'); // 1.48s
    // S._reserve(150000);
    for (uint32_t i = 0; i != n; i++) {
        if (s[i] >= 'a') s[i] ^= 32;
        if (s[i] != 'A') S.modify(i, s[i]);
    }

    for (uint32_t i = 0; i != m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            char c;
            cin >> c;
            if (c >= 'a') c ^= 32;
            cout << S.query(l - 1, r - 1).m_cnt[c - 'A'] << endl;
        } else if (op == '2') {
            char c;
            cin >> c;
            if (c >= 'a') c ^= 32;
            S.modify(l - 1, r - 1, c);
        } else {
            auto res = S.query(l - 1, r - 1);
            uint32_t cur = l - 1;
            for (uint32_t c = 0; c != 26; c++)
                if (res.m_cnt[c]) {
                    S.modify(cur, cur + res.m_cnt[c] - 1, 'A' + c);
                    cur += res.m_cnt[c];
                }
        }
    }
}

int main() {
    solve_czkw();
}