#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"

/*
[P4305 [JLOI2011] 不重复数字](https://www.luogu.com.cn/problem/P4305)
*/
/**
 * 本题为哈希表模板题
 */

OY::GHASH::UnorderedSet<int, true, 18> GS;
int main() {
    uint32_t t;
    cin >> t;
    GS.reserve(50000);
    while (t--) {
        uint32_t n;
        cin >> n;
        while (n--) {
            int x;
            cin >> x;
            if (GS.insert(x).m_flag) cout << x << ' ';
        }
        cout << endl;
        GS.clear();
    }
}