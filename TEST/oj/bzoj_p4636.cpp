#include "DS/Discretizer.h"
#include "DS/TagSegTree.h"
#include "DS/TagZkwTree.h"
#include "IO/FastIO.h"

/*
[#P4636. 蒟蒻的数列](https://new.bzoj.org:88/p/P4636)
*/
/**
 * 区间打标记，最后遍历
 */

void solve_tagzkw() {
    uint32_t m;
    cin >> m;

    struct Query {
        uint32_t l, r, v;
    };
    std::vector<Query> qs(m);
    OY::Discretizer<uint32_t> D;
    for (auto &[l, r, v] : qs) cin >> l >> r >> v, D << l << r;
    D.prepare();

    OY::TagMaxZkw<uint32_t> S(D.size() - 1);
    for (auto &[l, r, v] : qs) S.add(D.rank(l), D.rank(r) - 1, v);

    uint64_t sum{};
    uint32_t i = 0;
    S.enumerate([&](uint64_t val) {
        sum += val * (D[i + 1] - D[i]);
        i++;
    });
    cout << sum;
}

void solve_tagseg() {
    uint32_t m;
    cin >> m;

    OY::VectorTagMaxSeg<uint32_t, 0, uint32_t> S(1000000001);
    S._reserve(500000);
    std::vector<uint32_t> rs;
    rs.reserve(m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t l, r, v;
        cin >> l >> r >> v;
        rs.push_back(l), rs.push_back(r);
        S.add(l, r - 1, v);
    }
    std::sort(rs.begin(), rs.end());
    rs.erase(std::unique(rs.begin(), rs.end()), rs.end());

    uint64_t sum{};
    for (uint32_t i = 0; i + 1 != rs.size(); i++)
        sum += uint64_t(S.query(rs[i])) * (rs[i + 1] - rs[i]);
    cout << sum;
}

int main() {
    solve_tagzkw();
    // solve_tagseg();
}