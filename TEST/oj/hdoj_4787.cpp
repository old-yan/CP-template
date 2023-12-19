#include "IO/FastIO.h"
#include "STR/OnlineACAutomaton.h"

/*
[GRE Words Revenge](https://acm.hdu.edu.cn/showproblem.php?pid=4787)
*/
/**
 * 本题为 ac 自动机模板题
 * 可以按照二的幂，建立相应容量的 ac 自动机
 * 每当插入一个字符串时，将小自动机合并到大自动机上
 * 这样每个串最多只会被合并 log 次
 */

struct Node {
    uint32_t m_cnt;
};
int main() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        cout << "Case #" << i << ":\n";
        uint32_t n, lst = 0;
        cin >> n;
        OY::OnlineACAM<Node, 2> AC;
        for (uint32_t j = 0; j < n; j++) {
            char op;
            std::string s;
            cin >> op >> s;
            auto lst0 = lst % s.size();
            if (lst0) s = s.substr(lst0) + s.substr(0, lst0);
            if (op == '+') {
                if (AC.contains(s.begin(), s.end(), [](char c) { return c - '0'; })) continue;
                auto &&[ac, pos] = AC.insert(s.begin(), s.end(), [](char c) { return c - '0'; });
                ac->do_for_extending_nodes([&](uint32_t a) {
                    ac->get_node(a)->m_cnt = ac->get_fail_node(a)->m_cnt + (ac->get_node(a)->m_length > 0);
                });
            } else {
                lst = 0;
                auto query_cnt = [&](auto &&ac) {
                    uint32_t last = 0;
                    for (char c : s) {
                        last = ac.next(last, c - '0');
                        lst += ac.get_node(last)->m_cnt;
                    }
                };
                AC.do_for_each_ACAM(query_cnt);
                cout << lst << endl;
            }
        }
    }
}