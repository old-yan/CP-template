#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RaySeg.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> S(5);
    // 加边
    S.add_edge(2, 0);
    S.add_edge(1, 3);
    S.add_edge(4, 0);
    S.add_edge(0, 3);
    S.prepare(), S.set_root(3);

    // 预处理
    OY::RAYSEG::Table<decltype(S), int, 1000> sol(&S);
    // 屏蔽掉以 2 为根的子树，询问最长线段
    auto ray_seg = sol.max_ray_seg(0, sol.query_down_ray(2) + 1, sol.query_down_seg(2));
    cout << "longest ray from 0, length = " << ray_seg.m_ray << endl;
    cout << "longest seg, length = " << ray_seg.m_seg << endl;
}
/*
#输出如下
longest ray from 0, length = 2
longest seg, length = 3

*/