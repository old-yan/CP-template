#include "DS/OfflineSideView.h"
#include "IO/FastIO.h"

int main() {
    OY::OFFLINESV::Solver<int> S(4);
    S.add_modify(0, 100);
    S.add_modify(1, 120);
    S.add_modify(2, 80);
    S.add_modify(3, 140);

    // 在 [100, 120, 80, 140]，只能看到三个建筑
    S.add_query(3);

    S.add_modify(2, 130);

    // 在 [100, 120, 130, 140]，可以看到四个建筑
    S.add_query(3);

    S.add_modify(0, 125);
    // 在 [125, 120, 130]，可以看到两个建筑
    S.add_query(2);

    auto res = S.solve();
    cout << "building in view[100, 120, 80, 140]: " << res[0] << endl;
    cout << "building in view[100, 120, 130, 140]: " << res[1] << endl;
    cout << "building in view[125, 120, 80]: " << res[2] << endl;
}
/*
#输出如下
building in view[100, 120, 80, 140]: 3
building in view[100, 120, 130, 140]: 4
building in view[125, 120, 80]: 2

*/