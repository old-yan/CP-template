#include "DS/FHQTreap.h"
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

/*
[P4036 [JSOI2008] 火星人](https://www.luogu.com.cn/problem/P4036)
*/
/**
 * 本题须结合字符串哈希与平衡树
 * lcp 的查询可以参考 HashLCP 模板的实现
 */

static constexpr uint32_t N = 100000;
using mint = OY::mintu32;
using hash_type = OY::STRHASH::StrHash<mint, 131>;
template <typename Node>
struct NodeWrap {
    using key_type = typename hash_type::value_type;
    key_type m_key, m_sum;
    void set(const key_type &key) { m_key = key; }
    const key_type &get() const { return m_key; }
    void pushup(Node *lchild, Node *rchild) {
        if (lchild->is_null())
            if (rchild->is_null())
                m_sum = m_key;
            else
                m_sum = hash_type::combine(m_key, 1, rchild->m_sum, rchild->m_size);
        else if (rchild->is_null())
            m_sum = hash_type::combine(lchild->m_sum, lchild->m_size, m_key, 1);
        else
            m_sum = hash_type::combine(hash_type::combine(lchild->m_sum, lchild->m_size, m_key, 1), lchild->m_size + 1, rchild->m_sum, rchild->m_size);
    }
};
using Tree = OY::FHQ::Multiset<NodeWrap, N + 1>;
using node = Tree::node;
int main() {
    std::string s;
    uint32_t m;
    cin >> s >> m;

    uint32_t maxlen = std::min<uint32_t>(N, s.size() + m);
    hash_type::s_info.prepare_unit(maxlen), hash_type::s_info.prepare_unit_inv(maxlen);
    auto S = Tree::from_sorted(s.begin(), s.end());
    auto query = [&](uint32_t l, uint32_t r) {
        auto S3 = S.split_by_rank(r + 1);
        auto S2 = S.split_by_rank(l);
        auto res = S2.root()->m_sum;
        S.join(S2), S.join(S3);
        return res;
    };

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t x, y;
            cin >> x >> y;
            uint32_t len1 = S.size() - x + 1, len2 = S.size() - y + 1;
            // 此处二分与 HashLCP 模板中的实现相同
            uint32_t low = 0, high = std::min(len1, len2);
            while (low < high)
                if (uint32_t mid = (low + high + 1) / 2; query(x - 1, x + mid - 2) == query(y - 1, y + mid - 2))
                    low = mid;
                else
                    high = mid - 1;
            cout << low << endl;
        } else if (op == 'R') {
            uint32_t x;
            char d;
            cin >> x >> d;
            S.modify_by_rank(x - 1, [&](node *p) { p->set(d); });
        } else {
            uint32_t x;
            char d;
            cin >> x >> d;
            S.insert_by_rank(d, x);
        }
    }
}