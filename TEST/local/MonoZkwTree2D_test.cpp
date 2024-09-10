#include "DS/MonoZkwTree2D.h"
#include "IO/FastIO.h"

int main() {
    // 二维线段树与一维线段树的用法基本类似，所以只展示一下特殊方法
    int matrix[4][5] = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
    };
    // 除了行数、列数，还需要传递一个寻址函数
    OY::MonoSumTree2D<int> T(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T << endl;
    // 输出[0,2]行，[1,4]列的和
    cout << "sum(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;

    // 来写一个二维树状数组做不到的：区域乘积
    struct Monoid {
        using value_type = int;
        static value_type identity() { return 1; }
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::MONOZKW2D::Tree<Monoid> T_mul(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T_mul << endl;
    // 输出[0,2]行，[1,4]列的最大值
    cout << "prod(matrix[0~1][2~4])=" << T_mul.query(0, 1, 2, 4) << endl;
}
/*
#输出如下
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
sum(matrix[0~2][1~4])=27
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
prod(matrix[0~1][2~4])=48

*/