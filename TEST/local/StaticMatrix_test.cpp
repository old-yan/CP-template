#include "IO/FastIO.h"
#include "MATH/StaticMatrix.h"
#include "MATH/StaticModInt32.h"

template <typename Matrix>
void print(Matrix &mat) {
    for (int i = 0; i < mat.row(); i++) {
        for (int j = 0; j < mat.column(); j++) {
            cout << mat[i][j] << (j == mat.column() - 1 ? '\n' : ' ');
        }
    }
    cout << "---------------------\n";
}
int main() {
    using elem = OY::mint1000000007;
    OY::StaticMatrix<elem, 2, 3> A{{{1, 2, 3}, {4, 5, 6}}};
    A += 20;
    print(A);
    A -= 10;
    print(A);
    A *= 3;
    print(A);

    OY::StaticMatrix<elem, 3, 3> B = OY::StaticMatrix<elem, 3, 3>::raw(10);
    print(B);

    OY::StaticMatrix<elem, 3, 3> C = OY::StaticMatrix<elem, 3, 3>::unit();
    print(C);

    // 利用矩阵加速递推的例子：斐波那契数列
    // 假设 a[i] = x, a[i+1] = y, 则 a[i+2] = a[i] + a[i+1] = x + y
    // 由线性代数可知
    //
    //                     0   1
    //  {a[i], a[i+1]} × {       } = {a[i+1], a[i+2]}
    //                     1   1
    //
    // 不妨把右侧的矩阵叫做 P，则有
    //
    //                   0   1  ^n                   ^n
    //  {a[0], a[1]} × {       }   = {a[0], a[1]} × P   = {a[n], a[n+1]}
    //                   1   1
    //
    // 矩阵乘算虽然不满足交换律，但是符合结合律。我们可以用快速幂加速计算 P 的幂

    cout << "fibonacci[0] = 1\n";
    cout << "fibonacci[1] = 1\n";
    OY::StaticMatrix<elem, 1, 2> init{{{1, 1}}};
    OY::StaticMatrix<elem, 2, 2> P{{{0, 1}, {1, 1}}};
    // 计算 a[10]%1000000007
    cout << "fibonacci[10] = " << (init * P.pow(10))[0][0] << endl;
    // 计算 a[100000000000000]%1000000007
    cout << "fibonacci[100000000000000] = " << (init * P.pow(100000000000000))[0][0] << endl;
}
/*
#输出如下
21 22 23
24 25 26
---------------------
11 12 13
14 15 16
---------------------
33 36 39
42 45 48
---------------------
10 10 10
10 10 10
10 10 10
---------------------
1 0 0
0 1 0
0 0 1
---------------------
fibonacci[0] = 1
fibonacci[1] = 1
fibonacci[10] = 89
fibonacci[100000000000000] = 481075011

*/