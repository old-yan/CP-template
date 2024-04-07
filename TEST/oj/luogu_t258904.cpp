#include "DS/SortSegTree.h"
#include "IO/FastIO.h"

/*
[T258904 sort（区间排序）](https://www.luogu.com.cn/problem/T258904)
*/

void solve_seg() {
    uint32_t n, m, k;
    cin >> n >> m >> k;

    // 由于本模板只支持无重的键值，所以需要事先给所有的键值离散化去重
    std::vector<uint32_t> keys(n);
    for (uint32_t i = 0; i < n; i++) cin >> keys[i];
    std::vector<uint32_t> indexes(n);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::sort(indexes.begin(), indexes.end(), [&](uint32_t x, uint32_t y) { return keys[x] < keys[y]; });
    std::vector<uint32_t> ids(n);
    for (uint32_t i = 0; i < n; i++) ids[indexes[i]] = i;

    using Tree = OY::SortSeg::Tree<uint32_t, uint64_t, OY::SortSeg::MAINTAIN_RANGE, 1 << 21>;
    auto key_mapping = [&](uint32_t i) { return ids[i]; };
    auto mapping = [&](uint32_t i) { return keys[i]; };
    Tree S(n, key_mapping, mapping, 0);
    while (m--) {
        char op;
        uint32_t i;
        cin >> op >> i;
        if (op == '1')
            S.sort<false>((i - 1) * k, i * k - 1);
        else
            cout << S.query(0, i - 1) << endl;
    }
}

int main() {
    solve_seg();
}