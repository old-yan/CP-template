#include "DS/SegTree2D.h"
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
    OY::VectorSegSumTree2D<int, false, uint32_t> T(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T << endl;
    // 输出[0,2]行，[1,4]列的和
    cout << "sum(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;

    // 来完成一个二维树状数组做不到的：区域最值
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间最大值累加器
    auto getmax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto T_max = OY::make_SegTree2D<int, true, OY::Seg2D::Ignore>(4, 5, getmax, [&](int i, int j) { return matrix[i][j]; });
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } getmax;
    auto T_max = OY::make_SegTree2D<int, true, OY::Seg2D::Ignore>(4, 5, getmax, [&](int i, int j) { return matrix[i][j]; });
#endif
    cout << T_max << endl;
    // 输出[0,2]行，[1,4]列的最大值
    cout << "max(matrix[0~2][1~4])=" << T_max.query(0, 2, 1, 4) << endl;
}
/*
#输出如下
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
sum(matrix[0~2][1~4])=27
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
max(matrix[0~2][1~4])=6
*/