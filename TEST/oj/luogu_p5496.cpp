#include "IO/FastIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"

/*
[P5496 【模板】回文自动机（PAM）](https://www.luogu.com.cn/problem/P5496)
*/
/**
 * 本题为回文自动机模板题
 */

template <typename Node>
struct NodeWrap {
    uint32_t m_value;
};
using PAM = OY::StaticPAM_string<NodeWrap, 26>;
// using PAM = OY::StaticRollbackPAM_string<NodeWrap, 26, 19>;
int main() {
    std::string s;
    cin >> s;

    PAM pam;
    pam.reserve(s.size());
    uint32_t lastans = 0;
    for (uint32_t i = 0; i < s.size(); i++) {
        pam.push_back((s[i] - 'a' + lastans) % 26);
        auto id = pam.query_node_index(i);
        pam.get_node(id)->m_value = pam.get_fail_node(id)->m_value + 1;
        lastans = pam.get_node(id)->m_value;
        cout << lastans << ' ';
    }
}