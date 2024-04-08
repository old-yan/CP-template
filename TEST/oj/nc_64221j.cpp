#include "DS/SortSegTree.h"
#include "DS/SortFHQTreap.h"
#include "IO/FastIO.h"

/*
[草原追击](https://ac.nowcoder.com/acm/contest/64221/J)
*/
/**
 * 本题可以通过区间排序线段树解决
 * 稳定排序意味着在所有同高度的草丛里的排名不变
 * 将同高度草丛权值设为 1，不同高度草丛权值设为 0
 * 则每次模拟 sort 之后，通过树上二分可以找到藏匿的草丛
 */

void solve_seg() {
    uint32_t n, q, pos;
    cin >> n >> q >> pos;

    // 由于本模板只支持无重的键值，所以需要事先给所有的键值离散化去重
    std::vector<uint32_t> keys(n);
    for (uint32_t i = 0; i < n; i++) cin >> keys[i];
    uint32_t key_pos = keys[pos - 1];
    uint32_t cnt = std::count(keys.begin(), keys.begin() + pos, key_pos);

    std::vector<uint32_t> indexes(n);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::sort(indexes.begin(), indexes.end(), [&](uint32_t x, uint32_t y) { return keys[x] < keys[y]; });
    std::vector<uint32_t> ids(n);
    for (uint32_t i = 0; i < n; i++) ids[indexes[i]] = i;

    auto key_mapping = [&](uint32_t i) { return ids[i]; };
    auto mapping = [&](uint32_t i) { return keys[i] == key_pos; };
    OY::SortSeg::Tree<uint32_t, uint32_t, OY::SortSeg::MAINTAIN_RANGE, 1 << 21> S(n, key_mapping, mapping, n - 1, {});

    while (q--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1')
            S.sort<false>(l - 1, r - 1);
        else
            S.sort<true>(l - 1, r - 1);
        cout << S.max_right(0, [&](uint32_t x) { return x < cnt; }) + 2 << endl;
    }
}

void solve_fhq() {
    uint32_t n, q, pos;
    cin >> n >> q >> pos;

    // 本模板支持重复键值
    std::vector<uint32_t> keys(n);
    for (uint32_t i = 0; i < n; i++) cin >> keys[i];
    uint32_t key_pos = keys[pos - 1];
    uint32_t cnt = std::count(keys.begin(), keys.begin() + pos, key_pos);

    auto key_mapping = [&](uint32_t i) { return keys[i]; };
    auto mapping = [&](uint32_t i) { return keys[i] == key_pos; };
    OY::SortFHQ::Tree<uint32_t, uint32_t, std::less<uint32_t>, OY::SortFHQ::MAINTAIN_RANGE, 1 << 18> S(n, key_mapping, mapping, {});

    while (q--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1')
            S.sort<false>(l - 1, r - 1);
        else
            S.sort<true>(l - 1, r - 1);
        cout << S.max_right(0, [&](uint32_t x) { return x < cnt; }) + 2 << endl;
    }
}

int main() {
    solve_seg();
    // solve_fhq();
}