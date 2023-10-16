#include "DS/Leftist.h"
#include "DS/LinkBucket.h"
#include "IO/FastIO.h"

/*
[P3261 [JLOI2015] 城池攻占](https://www.luogu.com.cn/problem/P3261)
*/
/**
 * 本题为可整堆修改的可并堆的模板题
*/
static constexpr uint32_t N = 300000, M = 300000;
struct item {
    int64_t m_key;
    uint32_t m_id;
};
template <typename node>
struct Node {
    using value_type = item;
    int64_t m_modify_type, m_modify_val;
    item m_item;
    uint32_t m_size;
    static bool comp(const item &x, const item &y) { return x.m_key > y.m_key; }
    void set(const value_type &key) { m_item = key; }
    const value_type &get() const { return m_item; }
    void modify(int64_t modify_type, int64_t modify_val) { (m_item.m_key *= modify_type) += modify_val, m_modify_type *= modify_type, (m_modify_val *= modify_type) += modify_val; }
    void pushdown(node *lchild, node *rchild) {
        if (m_modify_type != 1 or m_modify_val) {
            if (!lchild->is_null()) lchild->modify(m_modify_type, m_modify_val);
            if (!rchild->is_null()) rchild->modify(m_modify_type, m_modify_val);
            m_modify_type = 1, m_modify_val = 0;
        }
    }
    void pushup(node *lchild, node *rchild) { m_size = 1 + (lchild->is_null() ? 0 : lchild->m_size) + (rchild->is_null() ? 0 : rchild->m_size); }
};
using Tree = OY::Leftist::Heap<Node, M + 1>;
using node = Tree::node;
OY::LBC::LinkBucket<uint32_t, N, N> Sons;
OY::LBC::LinkBucket<item, N, M> Visitors;
struct {
    int64_t modify_type, modify_val, defence, ans;
} Castle[N];
uint32_t soldier[M];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> Castle[i].defence;
    Sons.resize(n, n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t fa;
        cin >> fa >> Castle[i].modify_type >> Castle[i].modify_val;
        Sons[fa - 1].push_back(i);
    }
    Visitors.resize(n, m);
    for (uint32_t id = 0; id < m; id++) {
        int64_t w;
        uint32_t f;
        cin >> w >> f;
        Visitors[f - 1].push_back(item{w, id});
    }
    auto dfs = [&](auto self, uint32_t cur, uint32_t d) -> Tree {
        Tree res;
        for (auto &&[key, id] : Visitors[cur]) res.push({key, id}, [&](node *p) { p->m_modify_type = 1; }), soldier[id] = d;
        for (auto son : Sons[cur]) res.join(self(self, son, d + 1));
        Castle[cur].ans = res.root()->m_size;
        while (!res.empty() && res.top().m_key < Castle[cur].defence) soldier[res.top().m_id] -= d, res.pop();
        Castle[cur].ans -= res.root()->m_size;
        if (!res.empty())
            if (Castle[cur].modify_type == 0)
                res.root()->modify(1, Castle[cur].modify_val);
            else
                res.root()->modify(Castle[cur].modify_val, 0);
        return res;
    };
    dfs(dfs, 0, 1);
    for (uint32_t i = 0; i < n; i++) cout << Castle[i].ans << '\n';
    for (uint32_t i = 0; i < m; i++) cout << soldier[i] << '\n';
}