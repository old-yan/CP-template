#include "IO/FastIO.h"
#include "STR/RollbackKMP.h"

int main() {
    // RollbackKMP 拥有和普通 kmp 一样的功能，仅仅是保证了 push_back 和 pop_back 不会因最坏情况而超时

    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::RollbackKMP_string kmp(p);
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith " << p.substr(0, j) << endl;
        }
    }

    // 给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = kmp.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl;

    // 如果要查找所有出现位置
    int cur_pi = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        cur_pi = kmp.get_fail(cur_pi, c);
        if (p[cur_pi] == c) cur_pi++;
        if (cur_pi == kmp.size()) {
            int index = i - cur_pi + 1;
            cout << index << " " << s.substr(index, p.size()) << endl;
            cur_pi = kmp.query_Pi(cur_pi - 1);
        }
    }

    // 借助前缀函数可以快速求出字符串周期
    std::string q = "abcabcabcabcabcabcabc";
    // q 长度为 21
    int n = q.size();
    OY::RollbackKMP_string kmp2(q);
    auto val = kmp2.query_Pi(q.size() - 1);
    if (n % (n - val) == 0) {
        cout << n - val << " is minimum cycle of string " << q << endl;
    }
}
/*
#输出如下
index 4: abcda startwith and endwith a
index 5: abcdab startwith and endwith ab
index 6: abcdabc startwith and endwith abc
index 8: abcdabcea startwith and endwith a
index 9: abcdabceab startwith and endwith ab
index 10: abcdabceabc startwith and endwith abc
index 11: abcdabceabcd startwith and endwith abcd
11 abcdabceabcd
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd
3 is minimum cycle of string abcabcabcabcabcabcabc

*/