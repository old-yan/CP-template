#include "IO/FastIO.h"
#include "STR/FastSequenceAutomaton.h"
#include "STR/SequenceAutomaton.h"

/*
[P5826 【模板】子序列自动机](https://www.luogu.com.cn/problem/P5826)
*/
/**
 * 本题为序列自动机模板题
 * 值域小时，适用 FastSequenceAutomaton
 * 值域大时，适用 SequenceAutomaton
 */

int main() {
    uint32_t type, n, q, m;
    cin >> type >> n >> q >> m;
    if (type <= 2) {
        std::vector<uint32_t> arr(n);
        for (auto &a : arr) cin >> a, a--;
        OY::FastSequenceAutomaton<100> sa(arr.begin(), arr.end());
        for (uint32_t i = 0; i < q; i++) {
            uint32_t len;
            cin >> len;
            std::vector<uint32_t> arr2(len);
            for (auto &a : arr2) cin >> a, a--;
            cout << (sa.contains(arr2.begin(), arr2.end()) ? "Yes\n" : "No\n");
        }
    } else {
        std::vector<uint32_t> arr(n);
        for (auto &a : arr) cin >> a, a--;
        OY::SequenceAutomaton_vector sa(arr.begin(), arr.end());
        for (uint32_t i = 0; i < q; i++) {
            uint32_t len;
            cin >> len;
            std::vector<uint32_t> arr2(len);
            for (auto &a : arr2) cin >> a, a--;
            cout << (sa.contains(arr2.begin(), arr2.end()) ? "Yes\n" : "No\n");
        }
    }
}