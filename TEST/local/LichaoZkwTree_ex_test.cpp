#include "DS/LichaoZkwTree_ex.h"
#include "IO/FastIO.h"

int main() {
    // 仅展示与普通版不同之处
    constexpr int inf = 1000000000;
    OY::LichaoSlopeChmaxZkwTree_ex<int, -inf, true> S(50);
    // 在 [10, 20] 插入 y = x + 3
    S.add_line(10, 20, {1, 3});

    // 在 [15, 25] 整体增加 1000
    S.add_value(15, 25, 1000);

    // 在 2<=x<=3 时，最大值为 -inf 
    cout << "max([2~3]) = " << S.query(2, 3) << endl;
    
    // 在 10<=x<=14 时，最大值为 17
    cout << "max([10~14]) = " << S.query(10, 14) << endl;
    
    // 在 15<=x<=25 时，最大值为 1023
    cout << "max([15~25]) = " << S.query(15, 25) << endl;
    
    // 在 21<=x<=22 时，最大值为 -inf+1000
    cout << "max([21~22]) = " << S.query(21, 22) << endl;
}
/*
#输出如下
max([2~3]) = -1000000000
max([10~14]) = 17
max([15~25]) = 1023
max([21~22]) = -999999000

*/