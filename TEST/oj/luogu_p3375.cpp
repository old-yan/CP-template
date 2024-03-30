#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/HashLCP.h"
#include "STR/KMP.h"
#include "STR/RollbackKMP.h"
#include "STR/SuffixArray.h"
#include "STR/ZAlgorithm.h"

/*
[P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
*/
/**
 * 本题作为字符串模板题，有多种解法
 * 首先，肯定可以用 kmp算法 求出前缀函数
 * 其次，可以使用 Z 算法求出 Z 函数，间接求出前缀函数
 * 当然也可以用字符串哈希做
 */

auto lcp_to_pi = [](std::vector<uint32_t> &lcp) {
    // 利用单调队列优化 dp， LCP 转 Pi 数组
    uint32_t n = lcp.size();
    std::vector<uint32_t> pi(n);
    struct node {
        uint32_t m_left, m_end;
    };
    std::vector<node> stack;
    uint32_t cursor = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t z = i ? lcp[i] : n;
        if (i && (cursor == stack.size() || i + z - 1 > stack.back().m_end)) stack.push_back({i, i + z - 1});
        if (cursor < stack.size() && stack[cursor].m_end < i) cursor++;
        pi[i] = cursor < stack.size() ? i - stack[cursor].m_left + 1 : 0;
    }
    return pi;
};

auto lcp_to_pi2 = [](std::vector<uint32_t> &lcp) {
    // 也可以不用单调队列把 LCP 转 Pi 数组
    uint32_t n = lcp.size();
    std::vector<uint32_t> pi(n);
    for (uint32_t i = 1; i < n; i++)
        if (uint32_t z = i ? lcp[i] : n; z) pi[i + z - 1] = std::max(pi[i + z - 1], z);
    for (uint32_t i = n - 2; ~i; i--)
        if (pi[i + 1]) pi[i] = std::max(pi[i], pi[i + 1] - 1);
    return pi;
};

void solve_kmp() {
    std::string s1, s2;
    cin >> s1 >> s2;
    OY::KMP_string kmp(s2);
    // OY::RollbackKMP_string kmp(s2);
    uint32_t pi = 0;
    for (uint32_t i = 0; i < s1.size(); i++) {
        pi = kmp.jump(pi, s1[i]);
        if (s2[pi] == s1[i]) pi++;
        if (pi == kmp.size()) {
            cout << i - s2.size() + 2 << '\n';
            pi = kmp.query_Pi(pi - 1);
        }
    }
    for (uint32_t i = 0; i < s2.size(); i++) cout << kmp.query_Pi(i) << ' ';
}

void solve_hash() {
    using mint = OY::mintu32;
    using lcp_type = OY::HASHLCP::LCP<std::string, mint, 131>;
    using table_type = lcp_type::table_type;
    using hash_type = table_type::hash_type;
    std::string s1, s2;
    cin >> s1 >> s2;
    hash_type::s_info.prepare_unit(std::max(s1.size(), s2.size()));
    table_type S(s1);
    auto val = hash_type(s2).m_val;
    for (uint32_t l = 0, r = s2.size() - 1; r < s1.size(); l++, r++)
        if (S.query_value(l, r) == val) cout << l + 1 << '\n';

    auto LCP = OY::make_hash_LCP<mint, 131>(s2);
    std::vector<uint32_t> lcp(s2.size());
    for (uint32_t i = 0; i < s2.size(); i++) lcp[i] = i ? LCP.lcp(0, i) : s2.size();

    auto pi = lcp_to_pi(lcp);
    for (uint32_t i = 0; i < pi.size(); i++) cout << pi[i] << ' ';
}

void solve_z() {
    std::string s1, s2;
    cin >> s1 >> s2;
    OY::ZAlgorithm_string Z(s2);
    for (uint32_t i = 0, l = -1, r = -1, len = s1.size(); i < len; i++) {
        auto z = Z.adjust(l, r, i, len, s1);
        if (z == s2.size()) cout << i + 1 << '\n';
    }

    std::vector<uint32_t> lcp(s2.size());
    for (uint32_t i = 0; i < s2.size(); i++) lcp[i] = i ? Z.query_Z(i) : s2.size();

    auto pi = lcp_to_pi(lcp);
    for (uint32_t i = 0; i < pi.size(); i++) cout << pi[i] << ' ';
}

void solve_SA() {
    std::string s1, s2;
    cin >> s1 >> s2;
    OY::SA::SuffixArray<true, true, 2000000> SA(s1.size() + s2.size() + 1, [&](uint32_t i) {
        if (i < s1.size())
            return s1[i];
        else if (i == s1.size())
            return '^';
        else
            return s2[i - s1.size() - 1];
    });

    uint32_t rnk = SA.query_rank(s1.size() + 1);
    std::vector<bool> vis(s1.size() - s2.size() + 1);
    for (uint32_t i = rnk; i && SA.query_height(i) >= s2.size(); i--)
        if (auto pos = SA.query_sa(i - 1); pos < s1.size()) vis[pos] = true;
    for (uint32_t i = rnk + 1; SA.query_height(i) >= s2.size(); i++)
        if (auto pos = SA.query_sa(i); pos < s1.size()) vis[pos] = true;
    for (uint32_t i = 0; i < vis.size(); i++)
        if (vis[i]) cout << i + 1 << endl;

    std::vector<uint32_t> lcp(s2.size());
    for (uint32_t i = rnk, len = s2.size(); i; i--) {
        len = std::min(len, SA.query_height(i));
        if (!len) break;
        if (auto pos = SA.query_sa(i - 1); pos > s1.size()) lcp[pos - s1.size() - 1] = len;
    }
    for (uint32_t i = rnk + 1, len = s2.size(); i < s1.size() + s2.size() + 1; i++) {
        len = std::min(len, SA.query_height(i));
        if (!len) break;
        if (auto pos = SA.query_sa(i); pos > s1.size()) lcp[pos - s1.size() - 1] = len;
    }

    auto pi = lcp_to_pi(lcp);
    for (uint32_t i = 0; i < pi.size(); i++) cout << pi[i] << ' ';
}

int main() {
    solve_kmp();
    // solve_hash();
    // solve_z();
    // solve_SA();
}
