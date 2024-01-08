#include "IO/FastIO.h"
#include "STR/BiSuffixTree.h"

/*
[# 5362. I'm always close to you](https://qoj.ac/problem/5362)
*/
/**
 * 本题为双向后缀树模板题
 */

void solve_BiSTree() {
    using BiSTree = OY::StaticBiSufTree_string<uint8_t, 26, 200000>;
    BiSTree st;
    uint32_t n, type;
    cin >> n >> type;
    st.reserve(n);
    if (type) {
        uint64_t lst = 0;
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            char c;
            cin >> s >> c;
            if (s.size() == 9)
                st.push_back((c - 'a' + lst) % 26);
            else
                st.push_front((c - 'a' + lst) % 26);
            lst = st.unique_count();
            cout << lst << endl;
        }
    } else
        for (uint32_t i = 0; i < n; i++) {
            std::string s;
            char c;
            cin >> s >> c;
            if (s.size() == 9)
                st.push_back(c - 'a');
            else
                st.push_front(c - 'a');
            cout << st.unique_count() << endl;
        }
}

int main() {
    solve_BiSTree();
}