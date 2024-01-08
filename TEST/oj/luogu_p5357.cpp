#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P5357 【模板】AC 自动机（二次加强版）](https://www.luogu.com.cn/problem/P5357)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

using AC = OY::ACAM<26>;
int main() {
    uint32_t n;
    cin >> n;

    AC ac;
    ac.reserve(200000);
    std::vector<uint32_t> pos(n);
    std::vector<int> cnt(200001);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        pos[i] = ac.insert_lower(s);
    }
    ac.prepare();

    std::string str;
    cin >> str;
    uint32_t last_pos = 0;
    for (char c : str) {
        last_pos = ac.next(last_pos, c - 'a');
        cnt[last_pos]++;
    }
    ac.do_for_failing_nodes([&](uint32_t a) {
        cnt[ac.query_fail(a)] += cnt[a];
    });

    for (uint32_t i = 0; i < n; i++) cout << cnt[pos[i]] << endl;
}