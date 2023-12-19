#include "IO/FastIO.h"
#include "STR/SALCP.h"

int main() {
    // 给出主串
    std::string s = "abcabbbabcb";
    // 构造助手
    auto S = OY::make_SA_LCP<1000>(s);
    // 查询 lcp
    cout << s.substr(0) << " and " << s.substr(7) << " 's lcp = " << S.lcp(0, 7) << endl;
    // 查询带上限的 lcp
    cout << s.substr(0, 2) << " and " << s.substr(7, 2) << " 's lcp = " << S.lcp(0, 7, 2) << endl;
    // 比较子串
    int res = S.compare(0, 3, 7, 10);
    if (res < 0)
        cout << s.substr(0, 4) << " is smaller than " << s.substr(7, 4) << endl;
    else if (res > 0)
        cout << s.substr(0, 4) << " is bigger than " << s.substr(7, 4) << endl;
    else
        cout << s.substr(0, 4) << " is equal to " << s.substr(7, 4) << endl;
}
/*
#输出如下
abcabbbabcb and abcb 's lcp = 3
ab and ab 's lcp = 2
abca is smaller than abcb

*/