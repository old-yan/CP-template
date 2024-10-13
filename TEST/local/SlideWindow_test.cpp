#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

#include <map>
void test() {
    // 窗口数颜色
    cout << "count color count:\n";
    int a[] = {4, 7, 5, 1, 8, 1, 4, 4, 1};

    int n = 9;
    int window_len = 4;
    std::map<int, int> mp;
    auto call = [&](int l, int r) {
        cout << "color count of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << mp.size() << endl;
    };
    auto left_call = [&](int i) {
        auto &cnt = mp[a[i]];
        if (!--cnt) mp.erase(a[i]);
    };
    auto right_call = [&](int i) {
        auto &cnt = mp[a[i]];
        ++cnt;
    };
    OY::WINDOW::solve(n, window_len, call, left_call, right_call);
}

int main() {
    test();
}
/*
#输出如下
count color count:
color count of [4 7 5 1] = 4
color count of [7 5 1 8] = 4
color count of [5 1 8 1] = 3
color count of [1 8 1 4] = 3
color count of [8 1 4 4] = 3
color count of [1 4 4 1] = 2

*/