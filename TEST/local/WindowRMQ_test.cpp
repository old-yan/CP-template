#include "DS/WindowRMQ.h"
#include "IO/FastIO.h"

void test_arr_rmq() {
    // 一般来说，对静态数组进行查询可以使用 make_ 系列函数
    int arr[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];

    // 最小值窗口
    auto S_min = OY::make_MinWindow(9, 3, arr);
    for (int i = 0; i < 9; i++) {
        auto res = S_min.next();
        cout << "min(arr[" << std::max(0, i - 2) << "~" << i << "]) = arr[" << res->m_index << "] " << res->m_value << '\n';
    }

    std::vector<std::string> arr2{"44", "99", "22", "33", "55", "77", "88", "11", "66"};
    cout << "\narr2: ";
    for (int i = 0; i < 9; i++) cout << arr2[i] << " \n"[i == 8];
    // 最大值窗口
    auto S_max = OY::make_MaxWindow(9, 5, arr2);
    // 如果不想查询长度太短的窗口，可以先把右边界拓展到长度差一位就够的地方
    S_max.extend_to(3);
    for (int i = 4; i < 9; i++) {
        auto res = S_max.next();
        cout << "max(arr2[" << std::max(0, i - 4) << "~" << i << "]) = arr2[" << res->m_index << "] " << res->m_value << '\n';
    }

    cout << endl
         << endl;
}

void test_custom_rmq() {
    // 一些自定义的序列上的应用
    int a[5][2] = {{3, 159},
                   {9, 325},
                   {7, 916},
                   {8, 287},
                   {6, 730}};
    // 对 [3, 9, 7, 8, 6] 建立最大值窗口
    cout << "col1: ";
    for (int i = 0; i < 5; i++) cout << a[i][0] << " \n"[i == 4];
    auto getter = [&](int i) {
        return a[i][0];
    };
    auto S_max = OY::MaxWindow<int, decltype(getter)>(5, 3, {}, getter);
    S_max.extend_to(1);
    for (int i = 2; i < 5; i++) {
        auto res = S_max.next();
        cout << "max(col1[" << std::max(0, i - 2) << "~" << i << "]) = col1[" << res->m_index << "] " << res->m_value << '\n';
    }

    // 对 [159, 325, 916, 287, 730] 建立窗口，求十位数最大的数字
    cout << "\ncol2: ";
    for (int i = 0; i < 5; i++) cout << a[i][1] << " \n"[i == 4];
    // 自定义比较函数
    auto comp = [](int x, int y) {
        return (x % 100 / 10) < (y % 100 / 10);
    };
    auto getter2 = [&](int i) {
        return a[i][1];
    };
    auto S_max2 = OY::WINDOW::Table<int, decltype(comp), decltype(getter2)>(5, 2, comp, getter2);
    S_max2.extend_to(0);
    for (int i = 1; i < 5; i++) {
        auto res = S_max2.next();
        cout << "max(col2[" << std::max(0, i - 1) << "~" << i << "]) = col2[" << res->m_index << "] " << res->m_value << '\n';
    }
}

int main() {
    test_arr_rmq();
    test_custom_rmq();
}
/*
#输出如下
arr: 4 9 2 3 5 7 8 1 6
min(arr[0~0]) = arr[0] 4
min(arr[0~1]) = arr[0] 4
min(arr[0~2]) = arr[2] 2
min(arr[1~3]) = arr[2] 2
min(arr[2~4]) = arr[2] 2
min(arr[3~5]) = arr[3] 3
min(arr[4~6]) = arr[4] 5
min(arr[5~7]) = arr[7] 1
min(arr[6~8]) = arr[7] 1

arr2: 44 99 22 33 55 77 88 11 66
max(arr2[0~4]) = arr2[1] 99
max(arr2[1~5]) = arr2[1] 99
max(arr2[2~6]) = arr2[6] 88
max(arr2[3~7]) = arr2[6] 88
max(arr2[4~8]) = arr2[6] 88


col1: 3 9 7 8 6
max(col1[0~2]) = col1[1] 9
max(col1[1~3]) = col1[1] 9
max(col1[2~4]) = col1[3] 8

col2: 159 325 916 287 730
max(col2[0~1]) = col2[0] 159
max(col2[1~2]) = col2[1] 325
max(col2[2~3]) = col2[3] 287
max(col2[3~4]) = col2[3] 287

*/