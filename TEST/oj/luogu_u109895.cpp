#include "DS/BiTrie.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

/*
[U109895 [HDU4825]Xor Sum](https://www.luogu.com.cn/problem/U109895)
*/
/**
 * 本题为全局最大异或典题
 * 注意 SegCounter<uint32_t> 只能处理 2^31 以内的最大异或，本体需要 SegCounter<uint64_t>
 * 
 * 当然，不使用数据结构的 dfs 做法是最快的
 */

uint32_t arr[500000], mid[500000][32];
void solve_dfs() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    std::sort(arr, arr + n);
    for (uint32_t i = 0; i != m; i++) {
        uint64_t x;
        cin >> x;
        auto dfs = [&](auto self, uint32_t l, uint32_t r, uint32_t j) {
            if (!~j) return void(cout << arr[l] << endl);
            if ((arr[l] ^ arr[r - 1]) >> j & 1) {
                uint32_t m;
                if (mid[l][j])
                    m = mid[l][j];
                else
                    m = mid[l][j] = std::partition_point(arr + l, arr + r, [&](auto x) { return !(x >> j & 1); }) - arr;
                if (x >> j & 1)
                    self(self, l, m, j - 1);
                else
                    self(self, m, r, j - 1);
            } else
                self(self, l, r, j - 1);
        };
        dfs(dfs, 0, n, 31);
    }
}

void solve_segcounter() {
    uint32_t n, m;
    cin >> n >> m;
    using Tree = OY::StaticSegCounter<uint64_t, uint32_t, false, false, false, 1000000>;
    Tree S;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        S.add(x, 1);
    }
    for (uint32_t i = 0; i != m; i++) {
        uint64_t x;
        cin >> x;
        cout << S.max_bitxor(x)->key() << endl;
    }
}

void solve_bitrie() {
    uint32_t n, m;
    cin >> n >> m;
    using Tree = OY::StaticBiTrie32<32, OY::BiTrie::Ignore, 10000000>;
    Tree S;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        S.insert(x);
    }
    for (uint32_t i = 0; i != m; i++) {
        uint64_t x;
        cin >> x;
        cout << (S.max_bitxor(x).second ^ x) << endl;
    }
}

int main() {
    solve_dfs();
    // solve_segcounter();
    // solve_bitrie();
}