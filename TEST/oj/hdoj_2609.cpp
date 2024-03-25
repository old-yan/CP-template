#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "STR/MinimalSequence.h"

/*
[How many](https://acm.hdu.edu.cn/showproblem.php?pid=2609)
*/
/**
 * 显然，本题为最小表示法模板题
 * 一个循环同构可以由其最小表示唯一确定
 */

OY::GHASH::UnorderedSet<std::string, true, 16> GS;
int main() {
    uint32_t n;
    GS.reserve(10000);
    while (cin >> n) {
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            cin >> s;
            auto index = OY::get_minimal_rotation(s.begin(), s.end());
            std::rotate(s.begin(), s.begin() + index, s.end());
            GS.insert(s);
        }
        cout << GS.size() << endl;
        GS.clear();
    }
}