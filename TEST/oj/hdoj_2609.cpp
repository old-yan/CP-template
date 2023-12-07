#include "IO/FastIO.h"
#include "STR/MinimalSequence.h"

#include <set>

/*
[How many](https://acm.hdu.edu.cn/showproblem.php?pid=2609)
*/
/**
 * 显然，本题为最小表示法模板题
 * 一个循环同构可以由其最小表示唯一确定
 */

int main() {
    uint32_t n;
    while (cin >> n) {
        std::set<std::string> S;
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            cin >> s;
            auto index = OY::get_minimal_rotation(s.begin(), s.end());
            S.insert(s.substr(index) + s.substr(0, index));
        }
        cout << S.size() << endl;
    }
}