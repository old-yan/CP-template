#include "DS/Discretizer.h"
#include "IO/FastIO.h"

int main() {
    OY::Discretizer<int> D;
    D << 11 << 22 << 33;
    std::vector<int> vec{15, 25, 35};
    std::vector<std::vector<int>> vec_2d{{100, 200}, {400, 300}, {15, 25}};
    std::pair<int, int> p{5, 300};
    std::array<int, 5> arr{4, 5, 6, 7, 8};
    D << vec << vec_2d << p << arr;
    // 准备后不会再接受数据
    D.prepare();
    for (int i = 0; i < D.size(); i++) {
        cout << "D[" << i << "]=\t" << D[i] << endl;
    }
    cout << "rank of " << 24 << "=" << D.rank(24) << endl;
    cout << "rank of " << 25 << "=" << D.rank(25) << endl;
    cout << "rank of " << 26 << "=" << D.rank(26) << endl;
}
/*
#输出如下
D[0]=	4
D[1]=	5
D[2]=	6
D[3]=	7
D[4]=	8
D[5]=	11
D[6]=	15
D[7]=	22
D[8]=	25
D[9]=	33
D[10]=	35
D[11]=	100
D[12]=	200
D[13]=	300
D[14]=	400
rank of 24=8
rank of 25=8
rank of 26=9

*/