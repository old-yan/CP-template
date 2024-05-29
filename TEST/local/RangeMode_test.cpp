#include "DS/RangeMode.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[9] = {400, 900, 200, 300, 400, 300, 300, 100, 900};
    OY::RangeMode<int> rm(arr, arr + 9);

    cout << "mode of arr[1~3] = " << rm.query(1, 3).m_val << " for " << rm.query(1, 3).m_cnt << " times" << endl;
    cout << "mode of arr[4~6] = " << rm.query(4, 6).m_val << " for " << rm.query(4, 6).m_cnt << " times" << endl;
    cout << "mode of arr[1~6] = " << rm.query(1, 6).m_val << " for " << rm.query(1, 6).m_cnt << " times" << endl;
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