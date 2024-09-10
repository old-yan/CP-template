#include "DS/HistoryBIT.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[U216697 线段树区间历史版本和](https://www.luogu.com.cn/problem/U216697)
*/
/**
 * 区间历史和问题：
 * 设 t 为当前经历的版本数
 * 设 s 为 a 数组的区间历史和
 * 设 s2[i]=s[i]-a[i]*t
 *
 * 当历史版本增加一版时，版本数从 t 增长到 t+1
 * 若：a[i]+=v， 则 s2[i]-=v*t
 * 若：a[i]无修，则 s2[i]无变化
 * 所以只要每次操作把 s2 也修改一下即可
 *
 * bit 和 zkw 都可以这么写
 * zkw 还有一种下传标记的写法
 */

void solve_bit() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::StaticHistoryBIT<uint64_t, true, 1 << 21> S(n, read);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, v;
            cin >> l >> r >> v;
            // 先拷贝出一个新版本，在新版本上做修改
            S.copy_version();
            S.add(l - 1, r - 1, v);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            // 先回答完查询，再新建版本。否则会把新版本也算进去
            cout << S.history_query(l - 1, r - 1) << endl;
            S.copy_version();
        }
    }
}

struct Node {
    struct Info {
        uint64_t m_val, m_his_sum;
        Info operator+(const Info &rhs) const {
            return {m_val + rhs.m_val, m_his_sum + rhs.m_his_sum};
        }
    };
    struct tag {
        uint32_t m_inc, m_ver_inc;
        uint64_t m_his_inc;
        tag operator+(const tag &rhs) const {
            return {m_inc + rhs.m_inc, m_ver_inc + rhs.m_ver_inc, m_his_inc + rhs.m_his_inc + uint64_t(m_inc) * rhs.m_ver_inc};
        }
    };
    using value_type = Info;
    using modify_type = tag;
    static value_type op(value_type x, value_type y) { return x + y; }
    static void map(const modify_type &modify, Node *p, uint32_t len) {
        p->m_val.m_his_sum += modify.m_his_inc * len + p->m_val.m_val * modify.m_ver_inc;
        p->m_val.m_val += uint64_t(modify.m_inc) * len;
    }
    static void com(const modify_type &modify, Node *p) { p->m_tag = p->m_tag + modify; }
    value_type m_val;
    modify_type m_tag;
    void set(uint64_t val) { m_val.m_val = m_val.m_his_sum = val; }
    void set(value_type val) { m_val = val; }
    value_type get() const { return m_val; }
    bool has_lazy() const { return m_tag.m_inc || m_tag.m_ver_inc; }
    const modify_type &get_lazy() const { return m_tag; }
    void clear_lazy() { m_tag = {}; }
};
void solve_zkw() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::ZKW::Tree<Node> S(n, read);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, v;
            cin >> l >> r >> v;
            S.add(0, n - 1, {0, 1, 0});
            S.add(l - 1, r - 1, {v, 0, v});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l - 1, r - 1).m_his_sum << endl;
            S.add(0, n - 1, {0, 1, 0});
        }
    }
}

int main() {
    solve_bit();
    // solve_zkw();
}