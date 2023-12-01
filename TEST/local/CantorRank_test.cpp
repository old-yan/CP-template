#include "IO/FastIO.h"
#include "MATH/CantorRank.h"
#include "MATH/StaticModInt32.h"

using mint = OY::mint1000000007;
int main() {
    int A[] = {4, 20, 9, 1, 6, 11, 2, 8, 0, 5, 7, 15, 19};
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;
    std::swap(A[0], A[1]);
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;
    std::sort(A, A + 13);
    cout << "A's rank mod 1000000007 is " << OY::CantorRank<mint>::raw_query(A, A + 13) << endl;

    // 非数字类型需要离散化
    std::string s = "acb";
    cout << s << "'s rank is " << OY::CantorRank<mint>::query(s.begin(), s.end()) << endl;
}
/*
#输出如下
A's rank mod 1000000007 is 902001313
A's rank mod 1000000007 is 893681285
A's rank mod 1000000007 is 0
acb's rank is 1

*/