#include "IO/FastIO.h"
#include "STR/OnlineACAutomaton.h"

struct Node {
    std::string m_str;
    const std::string &get() const { return m_str; }
    void set(const std::string &str) { m_str = str; }
};
int main() {
    using OnlineACAM = OY::ONLINEAC::Automaton<Node, 26>;
    using ACAM = OnlineACAM::ACAM_type;

    OnlineACAM AC;

    // 本函数可以插入一个串，并在结点上做标记
    auto insert_lower = [&](const std::string &s) {
        auto res = AC.insert_lower(s);
        auto ptr_ac = res.first;
        int pos = res.second;
        ptr_ac->get_node(pos)->set(s);
    };

    // 搜寻、展示所有的 border
    auto show_border = [&](const std::string &s) {
        auto call = [&](const ACAM &ac) {
            int last_pos = 0;
            for (char c : s) last_pos = ac.next(last_pos, c - 'a');
            while (!ac.get_node(last_pos)->m_str.empty()) {
                cout << s << "'s border: " << ac.get_node(last_pos)->m_str << endl;
                last_pos = ac.query_fail(last_pos);
            }
        };
        cout << "try to find " << s << ":\n";
        for (int i = 0; i < AC.size(); i++) {
            cout << "In " << i << "-th ACAM:\n";
            call(*AC.get_ACAM(i));
        }
        cout << endl;
    };

    // 搜寻、展示所有的 border，第二种实现，但是不会区分 ACAM 的索引
    auto show_border_2 = [&](const std::string &s) {
        auto call = [&](const ACAM &ac) {
            int last_pos = 0;
            for (char c : s) last_pos = ac.next(last_pos, c - 'a');
            while (!ac.get_node(last_pos)->m_str.empty()) {
                cout << s << "'s border: " << ac.get_node(last_pos)->m_str << endl;
                last_pos = ac.query_fail(last_pos);
            }
        };
        cout << "try to find " << s << ":\n";
        AC.do_for_each_ACAM(call);
        cout << endl;
    };

    insert_lower("a");
    insert_lower("c");
    insert_lower("abc");
    show_border("xababc");

    insert_lower("ab");
    insert_lower("babc");
    insert_lower("abd");
    insert_lower("bc");
    show_border_2("xababc");
}
/*
#输出如下
try to find xababc:
In 0-th ACAM:
xababc's border: abc
In 1-th ACAM:
xababc's border: c

try to find xababc:
xababc's border: bc
xababc's border: babc
xababc's border: abc
xababc's border: c

*/