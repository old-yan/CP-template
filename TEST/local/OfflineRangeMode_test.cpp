#include "DS/OfflineRangeMode.h"
#include "IO/FastIO.h"

void test() {
    std::vector<int> arr{400, 900, 200, 300, 400, 300, 300, 100, 900};
    std::vector<std::pair<int, int>> queries{{1, 3}, {4, 6}, {1, 6}};

    OY::OFFLINEMODE::Solver<int> sol(arr.begin(), arr.end());
    for (int i = 0; i < 3; i++) sol.add_query(queries[i].first, queries[i].second);
    auto res = sol.solve();

    for (int i = 0; i < 3; i++)
        cout << "mode of arr[" << queries[i].first << "~" << queries[i].second << "] = " << res[i].m_elem << " for " << res[i].m_freq << " times" << endl;
}

int main() {
    test();
}
/*
#输出如下
mode of arr[1~3] = 900 for 1 times
mode of arr[4~6] = 300 for 2 times
mode of arr[1~6] = 300 for 3 times

*/