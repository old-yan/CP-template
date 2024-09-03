#include "DS/SideView.h"
#include "IO/FastIO.h"

int main() {
    OY::SV::Table<int> S(4);
    S.modify(0, 100);
    S.modify(1, 120);
    S.modify(2, 80);
    S.modify(3, 140);

    // 在 [100, 120, 80, 140]，只能看到三个建筑
    cout << "building in view[100, 120, 80, 140]: " << S.presum(3) << endl;

    S.modify(2, 130);
    // 在 [100, 120, 130, 140]，可以看到四个建筑
    cout << "building in view[100, 120, 130, 140]: " << S.presum(3) << endl;

    S.modify(0, 125);
    // 在 [125, 120, 130]，可以看到两个建筑
    cout << "building in view[125, 120, 80]: " << S.presum(2) << endl;
    // 在 [120, 130, 140]，可以看到三个建筑
    cout << "building in view[120, 130, 140]: " << S.query(1, 3) << endl;
}
/*
#输出如下
building in view[100, 120, 80, 140]: 3
building in view[100, 120, 130, 140]: 4
building in view[125, 120, 80]: 2
building in view[120, 130, 140]: 3

*/