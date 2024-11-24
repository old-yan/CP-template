#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

#include <map>
void test_count_color() {
    // 窗口数颜色
    cout << "count color:\n";
    int a[] = {4, 4, 4, 7, 5, 1, 5, 5, 4, 1, 6};

    int n = 11;
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
    cout << endl;
}

void test_max() {
    // 窗口最大值
    cout << "query window max:\n";
    int a[] = {4, 4, 4, 7, 5, 1, 5, 5, 4, 1, 6};

    int n = 11;
    int window_len = 4;
    auto mapping = [&](int i) { return a[i]; };
    auto call = [&](int l, int r, int val) {
        cout << "max of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << val << endl;
    };
    OY::WINDOW::solve_max<100>(n, window_len, mapping, call);
    cout << endl;
}

void test_min() {
    // 窗口最小值
    cout << "query window min:\n";
    int a[] = {4, 4, 4, 7, 5, 1, 5, 5, 4, 1, 6};

    int n = 11;
    int window_len = 4;
    auto mapping = [&](int i) { return a[i]; };
    auto call = [&](int l, int r, int val) {
        cout << "min of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << val << endl;
    };
    OY::WINDOW::solve_min<100>(n, window_len, mapping, call);
    cout << endl;
}

void test_count_color_iter() {
    // 窗口颜色数迭代器
    cout << "iter of window color count:\n";
    int a[] = {4, 4, 4, 7, 5, 1, 5, 5, 4, 1, 6};

    int n = 11;
    int window_len = 4;
    std::map<int, int> mp;
    auto call = [&](int l, int r) -> std::map<int, int> & {
        return mp;
    };
    auto left_call = [&](int i) {
        auto &cnt = mp[a[i]];
        if (!--cnt) mp.erase(a[i]);
    };
    auto right_call = [&](int i) {
        auto &cnt = mp[a[i]];
        ++cnt;
    };
    auto iter = OY::WINDOW::Iter<decltype(call), decltype(left_call), decltype(right_call)>(window_len, call, left_call, right_call);

    // 不断调用 iter 获取下一个颜色数
    for (int l = 0, r = window_len; r < n; l++, r++) {
        cout << "color count of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << iter.next().size() << endl;
    }
    cout << endl;
}

void test_max_iter() {
    // 窗口最大值迭代器
    cout << "iter of window max:\n";
    int a[] = {4, 4, 4, 7, 5, 1, 5, 5, 4, 1, 6};

    int n = 11;
    int window_len = 4;
    auto mapping = [&](int i) { return a[i]; };
    auto iter = OY::WINDOW::MaxIter<int, decltype(mapping)>(n, mapping, window_len);

    // 不断调用 iter 获取下一个最大值
    for (int l = 0, r = window_len; r < n; l++, r++) {
        cout << "max of [";
        for (int i = l; i <= r; i++) cout << a[i] << " ]"[i == r];
        cout << " = " << iter.next() << endl;
    }
    cout << endl;
}

int main() {
    test_count_color();
    test_max();
    test_min();
    test_count_color_iter();
    test_max_iter();
}
/*
#输出如下
count color:
color count of [4 4 4 7] = 2
color count of [4 4 7 5] = 3
color count of [4 7 5 1] = 4
color count of [7 5 1 5] = 3
color count of [5 1 5 5] = 2
color count of [1 5 5 4] = 3
color count of [5 5 4 1] = 3
color count of [5 4 1 6] = 4

query window max:
max of [4 4 4 7] = 7
max of [4 4 7 5] = 7
max of [4 7 5 1] = 7
max of [7 5 1 5] = 7
max of [5 1 5 5] = 5
max of [1 5 5 4] = 5
max of [5 5 4 1] = 5
max of [5 4 1 6] = 6

query window min:
min of [4 4 4 7] = 4
min of [4 4 7 5] = 4
min of [4 7 5 1] = 1
min of [7 5 1 5] = 1
min of [5 1 5 5] = 1
min of [1 5 5 4] = 1
min of [5 5 4 1] = 1
min of [5 4 1 6] = 1

iter of window color count:
color count of [4 4 4 7 5] = 2
color count of [4 4 7 5 1] = 3
color count of [4 7 5 1 5] = 4
color count of [7 5 1 5 5] = 3
color count of [5 1 5 5 4] = 2
color count of [1 5 5 4 1] = 3
color count of [5 5 4 1 6] = 3

iter of window max:
max of [4 4 4 7 5] = 7
max of [4 4 7 5 1] = 7
max of [4 7 5 1 5] = 7
max of [7 5 1 5 5] = 7
max of [5 1 5 5 4] = 5
max of [1 5 5 4 1] = 5
max of [5 5 4 1 6] = 5

*/