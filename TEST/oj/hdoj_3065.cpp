#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[病毒侵袭持续中](https://acm.hdu.edu.cn/showproblem.php?pid=3065)
*/

int main() {
    uint32_t n;
    while (cin >> n) {
        using AC = OY::ACAM<27>;
        AC ac;

        std::vector<std::string> words(n);
        for (auto &a : words) cin >> a;

        ac.reserve(n * 50);
        std::vector<int> index(n * 50 + 1), cnt(n * 50 + 1);
        for (uint32_t i = 0; i < n; i++) {
            int pos = ac.insert(words[i].begin(), words[i].end(), [](char c) { return c - 'A'; });
            index[pos] = i + 1;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        int last_pos = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c >= 'A' && c <= 'Z' ? c - 'A' : 26);
            cnt[last_pos]++;
        }

        // 在 fail 树上从叶到根倒推
        std::vector<uint32_t> ans_cnt(n);
        ac.do_for_failing_nodes([&](uint32_t a) {
            if (cnt[a]) {
                if (index[a])
                    ans_cnt[index[a] - 1] = cnt[a];
                cnt[ac.query_fail(a)] += cnt[a];
            }
        });

        for (uint32_t i = 0; i < n; i++)
            if (ans_cnt[i])
                cout << words[i] << ": " << ans_cnt[i] << endl;
    }
}