#include "DS/LinkBucket.h"
#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[D. 最小生成树](https://qoj.ac/problem/9904)
*/
/**
 * 考虑克鲁斯卡尔算法，难点在于找两端未连通的边，需要找平方次数
 * 如果将找“不连通”转化为找“代表元不同”，即可用哈希解决
 */

void solve_hash() {
    using mint = OY::mint1000000007;
    using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
    using hash_type = table_type::hash_type;

    uint32_t n;
    cin >> n;
    hash_type::s_info.set_random_base(n);
    hash_type::s_info.prepare_unit(std::bit_ceil(n));
    hash_type::s_info.prepare_unit_inv(std::bit_ceil(n));
    std::vector<std::pair<uint32_t, uint32_t>> ps(n * 2 - 3);
    for (uint32_t i = 0; auto &[val, sum] : ps) {
        cin >> val;
        sum = ++i;
    }
    std::ranges::sort(ps);
    OY::LBC::Container<uint32_t, true> dsu(n, n);
    std::vector<uint32_t> belong(n);
    for (uint32_t i = 0; i != n; i++) belong[i] = i, dsu[i].push_front(i);

    struct monoid {
        using value_type = hash_type;
        static value_type identity() { return {}; }
        static value_type op(const value_type &x, const value_type &y) { return x.append_right(y); }
    };
    OY::MONOZKW::Tree<monoid> pre(n, [&](uint32_t i) { return hash_type::single(i); });
    OY::MONOZKW::Tree<monoid> suf(n, [&](uint32_t i) { return hash_type::single(n - 1 - i); });
    uint64_t ans = 0;
    for (auto [val, sum] : ps) {
        if (sum < n)
            while (true) {
                // pre[0~x] 和 suf[n-sum-1~n-sum+x-1]
                uint32_t low = 0, high = sum + 1;
                while (low < high) {
                    auto mid = (low + high) >> 1;
                    if (pre.query(0, mid) == suf.query(n - sum - 1, n - sum - 1 + mid))
                        low = mid + 1;
                    else
                        high = mid;
                }
                if (low > sum) break;
                ans += val;
                uint32_t head1 = belong[low], head2 = belong[sum - low];
                if (dsu[head1].size() < dsu[head2].size()) {
                    for (auto x : dsu[head1]) belong[x] = head2, pre.modify(x, hash_type::single(head2)), suf.modify(n - 1 - x, hash_type::single(head2));
                    dsu.merge(head2, head1);
                } else {
                    for (auto x : dsu[head2]) belong[x] = head1, pre.modify(x, hash_type::single(head1)), suf.modify(n - 1 - x, hash_type::single(head1));
                    dsu.merge(head1, head2);
                }
            }
        else
            while (true) {
                // pre[sum-n+1~sum-n+1+x] 和 suf[0~x]
                uint32_t low = 0, high = n * 2 - 1 - sum;
                while (low < high) {
                    auto mid = (low + high) >> 1;
                    if (pre.query(sum - n + 1, sum - n + 1 + mid) == suf.query(0, mid))
                        low = mid + 1;
                    else
                        high = mid;
                }
                if (low == n * 2 - 1 - sum) break;
                ans += val;
                uint32_t head1 = belong[sum - n + 1 + low], head2 = belong[n - 1 - low];
                if (dsu[head1].size() < dsu[head2].size()) {
                    for (auto x : dsu[head1]) belong[x] = head2, pre.modify(x, hash_type::single(head2)), suf.modify(n - 1 - x, hash_type::single(head2));
                    dsu.merge(head2, head1);
                } else {
                    for (auto x : dsu[head2]) belong[x] = head1, pre.modify(x, hash_type::single(head1)), suf.modify(n - 1 - x, hash_type::single(head1));
                    dsu.merge(head1, head2);
                }
            }
    }
    cout << ans;
}

int main() {
    solve_hash();
}