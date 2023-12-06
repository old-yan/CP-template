#include "IO/FastIO.h"
#include "STR/SequenceAutomaton.h"

/*
[P3500 [POI2010] TES-Intelligence Test](https://www.luogu.com.cn/problem/P3500)
*/
/**
 * 本题为序列自动机模板题
 * 值域很大，适用 SequenceAutomaton
 */

int main() {
    uint32_t m;
    cin >> m;
    std::vector<uint32_t> arr(m);
    for (auto &a : arr) cin >> a, a--;
    OY::SequenceAutomaton_vector sa(arr.begin(), arr.end());

    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t len;
        cin >> len;
        std::vector<uint32_t> arr2(len);
        for (auto &a : arr2) cin >> a, a--;
        cout << (sa.contains(arr2.begin(), arr2.end()) ? "TAK\n" : "NIE\n");
    }
}