#include "DS/OfflineKindCounter.h"
#include "IO/FastIO.h"

void test() {
    std::vector<int> arr{400, 900, 200, 300, 400, 300, 300, 100, 900};
    std::vector<std::pair<int, int>> queries{{1, 3}, {4, 6}, {1, 6}};
    auto res = OY::OFFLINEKC::solve(arr, queries);

    for (int i = 0; i < queries.size(); i++)
        cout << "kind count of arr[" << queries[i].first << "~" << queries[i].second << "] = " << res[i] << endl;
}

int main() {
    test();
}
/*
#输出如下
kind count of arr[1~3] = 3
kind count of arr[4~6] = 2
kind count of arr[1~6] = 4

*/