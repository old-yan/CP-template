#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"
#include "TREE/VirtualTree.h"

/*
[P4103 [HEOI2014] 大工程](https://www.luogu.com.cn/problem/P4103)
*/
/**
 * 本题为虚树模板题
 * 由于本题在建好虚树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
 */
static constexpr uint32_t N = 1000000;
struct {
    uint32_t cnt;
    uint32_t far;
    uint32_t near;
    uint32_t dep;
    bool is_key;
} info[N];
uint32_t keys[N];
int main() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> T(n);
    // OY::FlatTree::Tree<bool, N> T(n);
    // OY::VectorTree::Tree<bool> T(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        T.add_edge(a - 1, b - 1);
    }
    T.prepare(), T.set_root(0);
    T.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { if(~p)info[a].dep = info[p].dep + 1; }, {}, {});

    OY::HLD::Table<decltype(T), N> R(&T);
    // OY::RMQLCA::Table<decltype(T), OY::SqrtMinTable<uint32_t, N>, N> R(&T);

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t m;
        cin >> m;
        uint32_t m0 = m;
        for (uint32_t i = 0; i < m; i++) {
            cin >> keys[i];
            info[--keys[i]].is_key = true;
        }
        // 最好还是让原树的根也作为虚树的根
        if (!info[0].is_key) keys[m++] = 0;

        uint64_t sum = 0;
        uint32_t far_pair = 0;
        uint32_t near_pair = 0x3f3f3f3f;
        auto update_far = [&](uint32_t x) {
            if (x > far_pair) far_pair = x;
        };
        auto update_near = [&](uint32_t x) {
            if (x < near_pair) near_pair = x;
        };
        auto call = [&](uint32_t a, uint32_t p) {
            if (info[a].is_key) {
                info[a].cnt++;
                if (info[a].far) update_far(info[a].far);
                if (info[a].near) update_near(info[a].near);
                info[a].near = 0;
            }
            uint32_t d = info[a].dep - info[p].dep;
            sum += uint64_t(d) * info[a].cnt * (m0 - info[a].cnt);
            if (info[p].far) update_far(info[p].far + d + info[a].far);
            info[p].far = std::max(info[p].far, d + info[a].far);
            if (info[p].near)
                update_near(info[p].near + d + info[a].near), info[p].near = std::min(info[p].near, d + info[a].near);
            else
                info[p].near = d + info[a].near;
            info[p].cnt += info[a].cnt;

            info[a].cnt = info[a].far = info[a].near = info[a].is_key = 0;
        };
        OY::VTREE::VirtualTree<N>::solve_hld(keys, keys + m, R, call);
        // OY::VTREE::VirtualTree<N>::solve_rmqlca(keys, keys + m, R, call);

        if (info[0].is_key) {
            update_far(info[0].far);
            update_near(info[0].near);
        }
        info[0].cnt = info[0].far = info[0].near = info[0].is_key = 0;

        cout << sum << ' ' << near_pair << ' ' << far_pair << endl;
    }
}