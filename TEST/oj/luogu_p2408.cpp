#include "IO/FastIO.h"
#include "STR/SAM.h"
#include "STR/SuffixArray.h"

/*
[P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
*/
/**
 * 本题为经典子串问题，求本质不同的子串数量，可以有多种做法
 * 可以套一个后缀自动机轻易解决
 * 也可以使用后缀数组解决
 */

void solve_sa() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    OY::SA::SuffixArray<true, true, 100000> SA(s);

    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t can = n - SA.query_sa(i);
        uint32_t h = SA.query_height(i);
        ans += can - h;
    }
    cout << ans << endl;
}

struct Node {
    uint64_t m_cnt;
};
void solve_SAM() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    using SAM = OY::StaticSAM_string<Node, 26>;
    SAM sam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    sam.prepare();

    uint64_t ans = 0;
    sam.do_for_failing_nodes([&](uint32_t a) {
        auto p = sam.get_node(a);
        for (uint32_t i = 0; i < 26; i++)
            if (p->has_child(i)) p->m_cnt += sam.get_node(p->get_child(i))->m_cnt + 1;
    });

    cout << sam.get_node(0)->m_cnt << endl;
}

int main() {
    solve_sa();
    // solve_SAM();
}