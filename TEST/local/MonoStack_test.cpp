#include "DS/MonoStack.h"
#include "IO/FastIO.h"

void test_rmq() {
    // 查询每个区间的最值
    int arr[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];
    auto S_min = OY::make_MonoMaxStack(9, arr);
    for (int right = 0; right < 9; right++) {
        // 将右边界拓展到 right
        S_min.extend_right();
        for (int left = 0; left <= right; left++) {
            auto res = S_min.raw_query_on_left_side(left);
            cout << "max(arr[" << left << "~" << right << "]) = arr[" << res->m_index << "] " << res->m_value << '\n';
        }
    }
    cout << endl;
}

void test_find_right_larger() {
    // 查找每个元素右侧的最近的更大元素所在位置
    int arr[9] = {6, 1, 8, 7, 5, 3, 2, 9, 4};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];

    std::vector<int> right_larger(9);
    // 比较函数要用 std::less_equal
    auto getter = [&](int i) {
        return arr[i];
    };
    auto pop_call = [&](int popped, int by_who) {
        // popped 表示被弹出栈的元素的下标
        // by_who 表示是被谁淘汰出栈的
        right_larger[popped] = by_who;
    };
    OY::MONOSTACK::Table<int, std::less_equal<int>, decltype(getter), decltype(pop_call)> S(9, {}, getter, pop_call, 999, true);
    for (int right = 0; right < 9; right++) S.extend_right();

    // 检验一下对不对
    for (int i = 0; i < 9; i++) {
        int j = right_larger[i];
        if (j) {
            cout << "arr[" << j << "](" << arr[j] << ") > arr[" << i << "](" << arr[i] << ")\n";
        }
    }
    cout << endl;
}

int main() {
    test_rmq();
    test_find_right_larger();
}
/*
#输出如下
arr: 4 9 2 3 5 7 8 1 6
max(arr[0~0]) = arr[0] 4
max(arr[0~1]) = arr[1] 9
max(arr[1~1]) = arr[1] 9
max(arr[0~2]) = arr[1] 9
max(arr[1~2]) = arr[1] 9
max(arr[2~2]) = arr[2] 2
max(arr[0~3]) = arr[1] 9
max(arr[1~3]) = arr[1] 9
max(arr[2~3]) = arr[3] 3
max(arr[3~3]) = arr[3] 3
max(arr[0~4]) = arr[1] 9
max(arr[1~4]) = arr[1] 9
max(arr[2~4]) = arr[4] 5
max(arr[3~4]) = arr[4] 5
max(arr[4~4]) = arr[4] 5
max(arr[0~5]) = arr[1] 9
max(arr[1~5]) = arr[1] 9
max(arr[2~5]) = arr[5] 7
max(arr[3~5]) = arr[5] 7
max(arr[4~5]) = arr[5] 7
max(arr[5~5]) = arr[5] 7
max(arr[0~6]) = arr[1] 9
max(arr[1~6]) = arr[1] 9
max(arr[2~6]) = arr[6] 8
max(arr[3~6]) = arr[6] 8
max(arr[4~6]) = arr[6] 8
max(arr[5~6]) = arr[6] 8
max(arr[6~6]) = arr[6] 8
max(arr[0~7]) = arr[1] 9
max(arr[1~7]) = arr[1] 9
max(arr[2~7]) = arr[6] 8
max(arr[3~7]) = arr[6] 8
max(arr[4~7]) = arr[6] 8
max(arr[5~7]) = arr[6] 8
max(arr[6~7]) = arr[6] 8
max(arr[7~7]) = arr[7] 1
max(arr[0~8]) = arr[1] 9
max(arr[1~8]) = arr[1] 9
max(arr[2~8]) = arr[6] 8
max(arr[3~8]) = arr[6] 8
max(arr[4~8]) = arr[6] 8
max(arr[5~8]) = arr[6] 8
max(arr[6~8]) = arr[6] 8
max(arr[7~8]) = arr[8] 6
max(arr[8~8]) = arr[8] 6

arr: 6 1 8 7 5 3 2 9 4
arr[2](8) > arr[0](6)
arr[2](8) > arr[1](1)
arr[7](9) > arr[2](8)
arr[7](9) > arr[3](7)
arr[7](9) > arr[4](5)
arr[7](9) > arr[5](3)
arr[7](9) > arr[6](2)

*/