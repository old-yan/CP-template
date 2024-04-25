#include "DS/KindCounter.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[9] = {400, 900, 200, 300, 400, 300, 300, 100, 900};
    OY::ArrayKindCounter<900> kc(arr, arr + 9);

    cout << "kind count of arr[1~3] = " << kc.query(1, 3) << endl;
    cout << "kind count of arr[4~6] = " << kc.query(4, 6) << endl;
    cout << "kind count of arr[1~6] = " << kc.query(1, 6) << endl;
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