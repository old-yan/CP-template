#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {100, 99, 100, 101, 102, 103, 102, 103, 102, 103};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个区间最大值、返回最大值下标的状压 RMQ 表
    OY::MaskRMQMaxIndexTable<int> S(A, A + 10);
    cout << "max(A[3~6]).index = " << S.query(3, 6) << endl;

    // 建立一个区间最小值、返回最小值的值的状压 RMQ 表
    OY::MaskRMQMinValueTable<int> S2(A, A + 10);
    cout << "min(A[3~6])       = " << S2.query(3, 6) << endl;
}
/*
#输出如下
100 99 100 101 102 103 102 103 102 103
max(A[3~6]).index = 5
min(A[3~6])       = 101

*/