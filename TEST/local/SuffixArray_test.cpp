#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

int main() {
    std::string s = "abcabaaabca";
    OY::SA::SuffixArray<true, true, 1000> SA(s);

    // 按照字典序枚举后缀子串，且查询其与前一个串的最长公共前缀长度
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(SA.query_sa(i)) << ' ' << SA.query_height(i) << endl;
    // 查询每个子串的名次
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(i) << " 's rank = " << SA.query_rank(i) << endl;

    // 查询两个后缀子串的大小关系
    std::string s1 = s.substr(3);
    std::string s2 = s.substr(7);
    if (SA.m_rank[3] < SA.m_rank[7])
        cout << s1 << " is smaller than " << s2 << endl;
    else
        cout << s1 << " is bigger than " << s2 << endl;
}
/*
#输出如下
a 0
aaabca 1
aabca 2
abaaabca 1
abca 2
abcabaaabca 4
baaabca 0
bca 1
bcabaaabca 3
ca 0
cabaaabca 2
abcabaaabca 's rank = 5
bcabaaabca 's rank = 8
cabaaabca 's rank = 10
abaaabca 's rank = 3
baaabca 's rank = 6
aaabca 's rank = 1
aabca 's rank = 2
abca 's rank = 4
bca 's rank = 7
ca 's rank = 9
a 's rank = 0
abaaabca is smaller than abca

*/