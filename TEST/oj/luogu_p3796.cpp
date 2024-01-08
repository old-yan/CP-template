#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
*/
/**
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

using AC = OY::ACAM<26>;
int main() {
    while (true) {
        uint32_t n;
        cin >> n;
        if (!n) break;
        AC ac;
        ac.reserve(70 * n);
        std::vector<int> cnt(70 * n + 1), index(70 * n + 1);
        std::vector<std::string> words(n);
        for (uint32_t i = 0; i < n; i++) {
            cin >> words[i];
            int pos = ac.insert_lower(words[i]);
            index[pos] = i + 1;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        uint32_t last_pos = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c - 'a');
            cnt[last_pos]++;
        }

        // 在 fail 树上从叶到根倒推
        uint32_t ans = 0;
        std::vector<uint32_t> ans_pos;
        ac.do_for_failing_nodes([&](uint32_t a) {
            if (cnt[a]) {
                if (index[a])
                    if (cnt[a] > ans) {
                        ans = cnt[a];
                        ans_pos.clear();
                        ans_pos.push_back(index[a]);
                    } else if (cnt[a] == ans)
                        ans_pos.push_back(index[a]);
                cnt[ac.query_fail(a)] += cnt[a];
            }
        });

        std::sort(ans_pos.begin(), ans_pos.end());
        cout << ans << endl;
        for (auto a : ans_pos) cout << words[a - 1] << endl;
    }
}