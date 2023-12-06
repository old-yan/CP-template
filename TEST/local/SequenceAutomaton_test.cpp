#include "IO/FastIO.h"
#include "STR/SequenceAutomaton.h"

int main() {
    std::string s = "abcfe";
    OY::SequenceAutomaton_vector sa(s.begin(), s.end());

    std::string p = "abcfe";
    if (sa.contains(p.begin(), p.end())) {
        cout << s << " has a subsequence :" << p << endl;
    }
    p = "bfe";
    if (sa.contains(p.begin(), p.end())) {
        cout << s << " has a subsequence :" << p << endl;
    }
    p = "bcd";
    if (!sa.contains(p.begin(), p.end())) {
        cout << s << " doesn't have a subsequence :" << p << endl;
    }
}
/*
#输出如下
abcfe has a subsequence :abcfe
abcfe has a subsequence :bfe
abcfe doesn't have a subsequence :bcd

*/