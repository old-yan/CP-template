#include "DS/RangeMex.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[7] = {1, 3, 0, 2, 0, 1, 5};
    OY::RangeMex<> rm(arr, arr + 7);

    cout << "mex of arr[0~1] = " << rm.query(0, 1) << endl;
    cout << "mex of arr[1~3] = " << rm.query(1, 3) << endl;
    cout << "mex of arr[4~6] = " << rm.query(4, 6) << endl;
    cout << "mex of arr[1~6] = " << rm.query(1, 6) << endl;
}

int main() {
    test();
}
/*
#输出如下
mex of arr[0~1] = 0
mex of arr[1~3] = 1
mex of arr[4~6] = 2
mex of arr[1~6] = 4

*/