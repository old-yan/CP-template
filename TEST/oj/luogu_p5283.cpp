#include "DS/PersistentBiTrie.h"
#include "DS/PersistentSegCounter.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

#include <queue>

/*
[P5283 [十二省联考 2019] 异或粽子](https://www.luogu.com.cn/problem/P5283)
*/
/**
 * 注意到本题的 k 值不大，所以可以模拟
 */

void solve_persegcounter() {
    uint32_t n, k;
    cin >> n >> k;
    using Trie = OY::StaticPerSegCounter<uint64_t, uint32_t, true, false, false, 10000000>;
    std::vector<Trie> tries;
    struct item {
        uint32_t elem, id, rk, val;
    };
    auto comp = [](item &x, item &y) {
        return x.val < y.val;
    };
    std::priority_queue<item, std::vector<item>, decltype(comp)> Q;

    uint32_t pre_xor = 0;
    tries.reserve(n + 1);
    tries.push_back({});
    tries.back().add(pre_xor, 1);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        pre_xor ^= x;
        tries.push_back(tries.back().copy());
        tries.back().add(pre_xor, 1);
        Q.push(item{pre_xor, i - 1, i - 1, uint32_t(tries[i - 1].kth_bitxor(pre_xor, i - 1)->key() ^ pre_xor)});
    }
    uint64_t ans{};
    while (k--) {
        auto [elem, id, rk, val] = Q.top();
        Q.pop();
        ans += val;
        if (rk) {
            val = tries[id].kth_bitxor(elem, --rk)->key() ^ elem;
            Q.push({elem, id, rk, val});
        }
    }
    cout << ans;
}

void solve_perbitrie() {
    uint32_t n, k;
    cin >> n >> k;
    using Trie = OY::StaticCountPerBiTrie32<32, 20000000>;
    std::vector<Trie> tries;
    struct item {
        uint32_t elem, id, rk, val;
    };
    auto comp = [](item &x, item &y) {
        return x.val < y.val;
    };
    std::priority_queue<item, std::vector<item>, decltype(comp)> Q;

    uint32_t pre_xor = 0;
    tries.reserve(n + 1);
    tries.push_back({});
    tries.back().insert_one(pre_xor);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        pre_xor ^= x;
        tries.push_back(tries.back().copy());
        tries.back().insert_one(pre_xor);
        Q.push(item{pre_xor, i - 1, i - 1, tries[i - 1].kth_bitxor(pre_xor, i - 1).second});
    }
    uint64_t ans{};
    while (k--) {
        auto [elem, id, rk, val] = Q.top();
        Q.pop();
        ans += val;
        if (rk) {
            val = tries[id].kth_bitxor(elem, --rk).second;
            Q.push({elem, id, rk, val});
        }
    }
    cout << ans;
}

void solve_wavelet() {
    uint32_t n, k;
    cin >> n >> k;
    std::vector<uint32_t> pres;

    uint32_t pre_xor = 0;
    pres.reserve(n + 1);
    pres.push_back(0);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        pres.push_back(pre_xor ^= x);
    }
    OY::WaveLet::Table<uint64_t> W(pres.begin(), pres.end());
    auto check = [&](uint32_t x) {
        uint32_t cnt = 0;
        for (uint32_t i = 1; i <= n; i++) {
            cnt += i - W.rank_bitxor(0, i - 1, pres[i], x);
            if (cnt >= k) return true;
        }
        return false;
    };
    // 先求出分界点，low 为最小的使 [low, 0xffffffff] 之间的异或对数量小于 k 的值
    // 也就是最后要补上一部分 low-1 的异或值
    uint64_t low = 1, high = uint64_t(1) << 32;
    while (low != high) {
        auto mid = (low + high) / 2;
        if (check(mid))
            low = mid + 1;
        else
            high = mid;
    }
    uint32_t cnt = 0;
    uint64_t sum = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t c = i - W.rank_bitxor(0, i - 1, pres[i], low);
        cnt += c;
        for (uint32_t j = 0; j != c; j++) sum += W.quantile_bitxor(0, i - 1, pres[i], i - 1 - j);
    }
    cout << sum + (k - cnt) * (low - 1);
}

int main() {
    solve_persegcounter();
    // solve_perbitrie();
    // solve_wavelet();
}