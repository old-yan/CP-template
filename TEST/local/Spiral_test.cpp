#include "IO/FastIO.h"
#include "MISC/Spiral.h"

void test_square() {
    OY::SpiralSquare S(4);
    cout << "side = " << S.side() << endl;
    cout << "size = " << S.size() << endl;
    // 给定遍历下标，可以快速找到位置
    cout << "traverse:\n";
    for (uint32_t idx = 0; idx != S.size(); idx++) {
        auto ij = S[idx];
        cout << ij.m_i << ", " << ij.m_j << endl;
    }
    // 给定位置，可以快速找到遍历下标
    cout << "order of position;\n";
    for (uint32_t i = 0; i < S.side(); i++) {
        for (uint32_t j = 0; j < S.side(); j++) {
            auto idx = S.index_of({i, j});
            cout << "(" << i << ", " << j << ").index = " << idx << endl;
        }
    }
    cout << endl;
}

void test_matrix() {
    OY::SpiralMatrix S(5, 7);
    cout << "row = " << S.row() << endl;
    cout << "column = " << S.column() << endl;
    cout << "size = " << S.size() << endl;
    // 给定遍历下标，可以快速找到位置
    cout << "traverse:\n";
    for (uint32_t idx = 0; idx != S.size(); idx++) {
        auto ij = S[idx];
        cout << ij.m_i << ", " << ij.m_j << endl;
    }
    // 给定位置，可以快速找到遍历下标
    cout << "order of position;\n";
    for (uint32_t i = 0; i < S.row(); i++) {
        for (uint32_t j = 0; j < S.column(); j++) {
            auto idx = S.index_of({i, j});
            cout << "(" << i << ", " << j << ").index = " << idx << endl;
        }
    }
}

int main() {
    test_square();
    test_matrix();
}
/*
#输出如下
side = 4
size = 16
traverse:
0, 0
0, 1
0, 2
0, 3
1, 3
2, 3
3, 3
3, 2
3, 1
3, 0
2, 0
1, 0
1, 1
1, 2
2, 2
2, 1
order of position;
(0, 0).index = 0
(0, 1).index = 1
(0, 2).index = 2
(0, 3).index = 3
(1, 0).index = 11
(1, 1).index = 12
(1, 2).index = 13
(1, 3).index = 4
(2, 0).index = 10
(2, 1).index = 15
(2, 2).index = 14
(2, 3).index = 5
(3, 0).index = 9
(3, 1).index = 8
(3, 2).index = 7
(3, 3).index = 6

row = 5
column = 7
size = 35
traverse:
0, 0
0, 1
0, 2
0, 3
0, 4
0, 5
0, 6
1, 6
2, 6
3, 6
4, 6
4, 5
4, 4
4, 3
4, 2
4, 1
4, 0
3, 0
2, 0
1, 0
1, 1
1, 2
1, 3
1, 4
1, 5
2, 5
3, 5
3, 4
3, 3
3, 2
3, 1
2, 1
2, 2
2, 3
2, 4
order of position;
(0, 0).index = 0
(0, 1).index = 1
(0, 2).index = 2
(0, 3).index = 3
(0, 4).index = 4
(0, 5).index = 5
(0, 6).index = 6
(1, 0).index = 19
(1, 1).index = 20
(1, 2).index = 21
(1, 3).index = 22
(1, 4).index = 23
(1, 5).index = 24
(1, 6).index = 7
(2, 0).index = 18
(2, 1).index = 31
(2, 2).index = 32
(2, 3).index = 33
(2, 4).index = 34
(2, 5).index = 25
(2, 6).index = 8
(3, 0).index = 17
(3, 1).index = 30
(3, 2).index = 29
(3, 3).index = 28
(3, 4).index = 27
(3, 5).index = 26
(3, 6).index = 9
(4, 0).index = 16
(4, 1).index = 15
(4, 2).index = 14
(4, 3).index = 13
(4, 4).index = 12
(4, 5).index = 11
(4, 6).index = 10

 */