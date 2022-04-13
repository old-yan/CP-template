#include "DS/STtable.h"
#include "IO/FastIO.h"
#include "STR/DoublingSuffixArray.h"

int main() {
    std::string s = "abcabaaabca";
    OY::DoublingSuffixArray dsa(s.begin(), s.end());
    //按照字典序枚举后缀子串，且查询其与前一个串的最长公共前缀长度
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(dsa.m_sa[i]) << ' ' << dsa.m_height[i] << endl;
    //查询每个子串的名次
    for (int i = 0; i < s.size(); i++)
        cout << s.substr(i) << " 's rank = " << dsa.m_rank[i] << endl;
    //查询模式串 ab 的所有出现位置
    std::string p = "ab";
    int lower = dsa.lower_bound(p.begin(), p.end());
    int upper = dsa.upper_bound(p.begin(), p.end());
    for (int i = lower; i < upper; i++)
        cout << dsa.m_sa[i] << " is the index of \"ab\"\n";
    //查询两个后缀子串的大小关系
    std::string s1 = s.substr(3);
    std::string s2 = s.substr(7);
    if (dsa.m_rank[3] < dsa.m_rank[7])
        cout << s1 << " is smaller than " << s2 << endl;
    else
        cout << s1 << " is bigger than " << s2 << endl;

    //**********************************************************************
    //借助线段树、st 表等数据结构，还可以快速查询任意两个后缀子串的最长公共前缀
    //假如我们要处理的查询数很多，我们会选择 st 表。（初始化略慢，查询很快）
    OY::STTable st(dsa.m_height.begin(), dsa.m_height.end(), std::min);
    int lcp = dsa.m_rank[3] < dsa.m_rank[7] ? st.query(dsa.m_rank[3] + 1, dsa.m_rank[7]) : st.query(dsa.m_rank[7] + 1, dsa.m_rank[3]);
    cout << s.substr(3, lcp) << " is the longest common prefix of " << s1 << " and " << s2 << endl;
}