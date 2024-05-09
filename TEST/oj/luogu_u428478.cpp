#include "DS/GlobalUnorderedDiscretizer.h"
#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/MenghaniMatani.h"

/*
[U428478 祖孙询问](https://www.luogu.com.cn/problem/U428478)
*/
/**
 * 本题为模板题，判断树上结点关系
 */

static constexpr uint32_t N = 40000;
OY::GlobalUnorderedDiscretizer<uint32_t, 1 << 17> D;
int main() {
    uint32_t n, rt;
    cin >> n;
    // 离散化
    struct edge {
        int a, b;
    };
    std::vector<edge> es(n);
    for (auto &[a, b] : es) {
        cin >> a >> b;
        D.insert(a);
        if (~b) D.insert(b);
    }

    OY::LinkTree::Tree<bool, N> S(D.size());
    for (auto &[a, b] : es)
        if (~b)
            S.add_edge(D.find(a), D.find(b));
        else
            rt = D.find(a);
    S.prepare(), S.set_root(rt);
    OY::MenghaniMatani::Table<decltype(S)> M(&S);

    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        a = D.find(a), b = D.find(b);
        if (M.get_depth(a) < M.get_depth(b)) {
            if (M.get_ancestor(b, M.get_depth(b) - M.get_depth(a)) == a)
                cout << "1\n";
            else
                cout << "0\n";
        } else if (M.get_depth(a) > M.get_depth(b)) {
            if (M.get_ancestor(a, M.get_depth(a) - M.get_depth(b)) == b)
                cout << "2\n";
            else
                cout << "0\n";
        } else
            cout << "0\n";
    }
}