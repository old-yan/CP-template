#include "IO/FastIO.h"
#include "STR/KMP.h"

void test_find_longest_border() {
    cout << "test find border:\n";
    // 给出模式串并进行预处理
    std::string p = "aabaabaabaa";
    OY::KMP_string kmp(p);
    // 找出每个前缀子串的最长 border，或者说每个位置的前缀函数
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith " << p.substr(0, j) << endl;
        }
    }
    cout << endl;
}

void test_find_all_borders() {
    cout << "test find all borders:\n";
    // 给出模式串并进行预处理
    std::string p = "aabaabaabaa";
    OY::KMP_string kmp(p);
    // 找出每个前缀子串的所有 border
    for (int i = 0; i < p.size(); i++) {
        int j = kmp.query_Pi(i);
        if (j > 0) {
            cout << "index " << i << ": " << p.substr(0, i + 1) << " startwith and endwith ";
            //  << p.substr(0, j) << endl;
            kmp.do_for_each_border(j, [&](int pi) {
                if (pi != j) cout << "/";
                cout << p.substr(0, pi);
            });
            cout << endl;
        }
    }
    cout << endl;
}

void test_find_occurrence() {
    cout << "test find occurrence:\n";
    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::KMP_string kmp(p);
    // 给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = kmp.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl
         << endl;
}

void test_find_all_occurrences() {
    cout << "test find all occurrences:\n";
    // 给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::KMP_string kmp(p);

    // 给出文本串并进行查找所有出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";

    // 如果要查找所有出现位置
    int cur_pi = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        cur_pi = kmp.jump(cur_pi, c);
        if (p[cur_pi] == c) cur_pi++;
        if (cur_pi == kmp.size()) {
            int index = i - cur_pi + 1;
            cout << index << " " << s.substr(index, p.size()) << endl;
        }
    }
    cout << endl;
}

void test_find_cycle() {
    cout << "test find cycle of string:\n";
    // 借助前缀函数可以快速求出字符串周期
    std::string p = "abcabcabcabcabcabcabc";
    // p 长度为 21
    int n = p.size();
    OY::KMP_string kmp(p);
    auto val = kmp.query_Pi(p.size() - 1);
    if (n % (n - val) == 0) {
        cout << n - val << " is minimum cycle of string " << p << endl;
    }
    cout << endl;
}

int main() {
    test_find_longest_border();
    test_find_all_borders();
    test_find_occurrence();
    test_find_all_occurrences();
    test_find_cycle();
}
/*
#输出如下
test find border:
index 1: aa startwith and endwith a
index 3: aaba startwith and endwith a
index 4: aabaa startwith and endwith aa
index 5: aabaab startwith and endwith aab
index 6: aabaaba startwith and endwith aaba
index 7: aabaabaa startwith and endwith aabaa
index 8: aabaabaab startwith and endwith aabaab
index 9: aabaabaaba startwith and endwith aabaaba
index 10: aabaabaabaa startwith and endwith aabaabaa

test find all borders:
index 1: aa startwith and endwith a
index 3: aaba startwith and endwith a
index 4: aabaa startwith and endwith aa/a
index 5: aabaab startwith and endwith aab
index 6: aabaaba startwith and endwith aaba/a
index 7: aabaabaa startwith and endwith aabaa/aa/a
index 8: aabaabaab startwith and endwith aabaab/aab
index 9: aabaabaaba startwith and endwith aabaaba/aaba/a
index 10: aabaabaabaa startwith and endwith aabaabaa/aabaa/aa/a

test find occurrence:
11 abcdabceabcd

test find all occurrences:
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd

test find cycle of string:
3 is minimum cycle of string abcabcabcabcabcabcabc

*/