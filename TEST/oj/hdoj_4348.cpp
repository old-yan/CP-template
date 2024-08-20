#include "DS/PersistentBIT.h"
#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

/*
[To the moon](https://acm.hdu.edu.cn/showproblem.php?pid=4348)
*/
/**
 * 本题为可持久化的区间修改线段树模板题
 * 需要注意的是，可持久化线段树的占用空间很大，所以在 hdoj 的空间限制下无法通过
 * 恰好本题为“半可持久化”问题，也就是每个新建的版本一定是从当前的最新版本的基础上新建的
 * 所以可以使用可持久化树状数组通过，占用空间较小
 */

void solve_perbit() {
    uint32_t n, m;
    while (cin >> n >> m) {
        using Tree = OY::PerBIT::Tree<int64_t, uint32_t, OY::PerBIT::VectorTag, true, true>;
        Tree S(n, [](auto...) {
            int64_t x;
            cin >> x;
            return x;
        });
        S.reserve(m);
        for (uint32_t i = 1; i <= m; i++) {
            char op;
            cin >> op;
            if (op == 'C') {
                uint32_t l, r;
                int64_t d;
                cin >> l >> r >> d;
                S.copy_as(i);
                S.add(l - 1, r - 1, d);
            } else if (op == 'Q') {
                uint32_t l, r;
                cin >> l >> r;
                cout << S.query(l - 1, r - 1) << endl;
            } else if (op == 'H') {
                uint32_t l, r, t;
                cin >> l >> r >> t;
                cout << S.get_version_view(S.kth_version(t)).query(l - 1, r - 1) << endl;
            } else {
                uint32_t t;
                cin >> t;
                while (S.version_count() != t + 1) S.rollback();
            }
        }
        cout << endl;
    }
}

void solve_perseg() {
    auto solve = [] {
        uint32_t n, m;
        cin >> n >> m;
        using Tree = OY::VectorPerSegLazySumTree<int64_t, false, true, uint32_t>;
        std::vector<Tree> v;
        v.emplace_back(n, [](auto...) {
            int64_t x;
            cin >> x;
            return x;
        });
        Tree::unlock();
        for (uint32_t i = 0; i != m; i++) {
            char op;
            cin >> op;
            if (op == 'C') {
                uint32_t l, r;
                int64_t d;
                cin >> l >> r >> d;
                v.push_back(v.back().copy());
                v.back().add(l - 1, r - 1, d);
            } else if (op == 'Q') {
                uint32_t l, r;
                cin >> l >> r;
                cout << v.back().query(l - 1, r - 1) << endl;
            } else if (op == 'H') {
                uint32_t l, r, t;
                cin >> l >> r >> t;
                cout << v[t].query(l - 1, r - 1) << endl;
            } else {
                uint32_t t;
                cin >> t;
                v.resize(t + 1);
            }
        }
    };
    solve();
}

int main() {
    solve_perbit();
    // solve_perseg();
}
