#include "IO/FastIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P5496 【模板】回文自动机（PAM）](https://www.luogu.com.cn/problem/P5496)
*/
/**
 * 本题为回文自动机模板题
 */

int main() {
    std::string s;
    cin >> s;

    using PAM = OY::StaticPAM_string<26>;
    // using PAM = OY::StaticRollbackPAM_string<26, 19>;
    PAM pam;
    pam.reserve(s.size());
    std::vector<uint32_t> value(s.size() + 2);
    uint32_t lastans = 0;
    for (uint32_t i = 0; i < s.size(); i++) {
        pam.push_back((s[i] - 'a' + lastans) % 26);
        auto id = pam.query_node_index(i);
        value[id] = value[pam.get_node(id)->m_fail] + 1;
        lastans = value[id];
        cout << lastans << ' ';
    }
}