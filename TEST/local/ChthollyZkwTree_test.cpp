#include "DS/ChthollyZkwTree.h"
#include "IO/FastIO.h"

int main() {
    // 不妨以珂朵莉树的起源， cf896c 为例
    OY::ChthollyZkwTree<int> S(10);
    for (int i = 0; i < 10; i++) S.modify(i, 100 * i);
    cout << S << endl;

    // 操作一，区间覆盖
    S.modify(1, 3, 200);
    S.modify(6, 8, 900);
    S.modify(3, 6, 10);
    cout << S << endl;

    // 操作二，区间加
    auto range_add = [&](int l, int r) {
        // 首先把区间找出来
        std::vector<std::pair<std::pair<int, int>, int>> rs;
        S.enumerate(l, r, [&](int L, int R, int val) { rs.emplace_back(std::make_pair(L, R), val); });
        // 最后统一进行区间加
        for (int i = 0; i < rs.size(); i++) {
            int L = rs[i].first.first;
            int R = rs[i].first.second;
            int val = rs[i].second;
            S.modify(L, R, val + 1);
        }
    };
    range_add(1, 8);
    cout << S << endl;

    // 操作三，区间排序后的第 k 个元素
    auto range_kth = [&](int l, int r, int k) {
        // 首先把区间找出来
        std::vector<std::pair<int, std::pair<int, int>>> rs;
        S.enumerate(l, r, [&](int L, int R, int val) { rs.emplace_back(val, std::make_pair(L, R)); });
        // 排序后找出第 k 个
        std::sort(rs.begin(), rs.end());
        int cnt = 0;
        for (int i = 0; i < rs.size(); i++) {
            int val = rs[i].first;
            int L = rs[i].second.first;
            int R = rs[i].second.second;
            if (cnt + R - L + 1 >= k) return val;
            cnt += R - L + 1;
        }
        return -1;
    };
    cout << "smallest number in [1,8] = " << range_kth(1, 8, 1) << endl;
    cout << "biggest number in [1,8] = " << range_kth(1, 8, 8) << endl;

    // 操作四和操作三同理，不过不需要存到 vector 里排序，只需要在 enumerate 的时候计算就行
}
/*
#输出如下
[0, 100, 200, 300, 400, 500, 600, 700, 800, 900]
[0, 200, 200, 10, 10, 10, 10, 900, 900, 900]
[0, 201, 201, 11, 11, 11, 11, 901, 901, 900]
smallest number in [1,8] = 11
biggest number in [1,8] = 901

*/