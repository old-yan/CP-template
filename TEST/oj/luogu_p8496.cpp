#include "DS/Deque.h"
#include "DS/FHQCounter.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[P8496 [NOI2022] 众数](https://www.luogu.com.cn/problem/P8496)
*/
/**
 * 本题需要求绝对众数
 * 此外还需要初始空间够小的 deque
 */

// 摩尔投票的合并类
struct item {
    uint32_t m_val, m_cnt;
    using ref = const uint32_t &;
    item() = default;
    item(uint32_t x) : m_val{x}, m_cnt{1} {}
    item(uint32_t x, uint32_t cnt) : m_val{x}, m_cnt{cnt} {}
    friend item operator+(const item &x, const item &y) {
        if (x.m_val == y.m_val) return {x.m_val, x.m_cnt + y.m_cnt};
        if (x.m_cnt >= y.m_cnt)
            return {x.m_val, x.m_cnt - y.m_cnt};
        else
            return {y.m_val, y.m_cnt - x.m_cnt};
    }
    friend item operator+(uint32_t x, const item &y) {
        if (y.m_cnt && y.m_val != x)
            return {y.m_val, y.m_cnt - 1};
        else
            return {x, 1 + y.m_cnt};
    }
    friend item operator+(const item &x, uint32_t y) {
        if (x.m_cnt && x.m_val != y)
            return {x.m_val, x.m_cnt - 1};
        else
            return {y, 1 + x.m_cnt};
    }
};
void solve_deque() {
    uint32_t n, q;
    cin >> n >> q;
    using Deque = OY::SumDeque<uint32_t, item>;
    using Counter = OY::SEGCNT::Table<uint32_t, uint32_t, false, false, false, OY::StaticBufferWrapWithCollect<2000000>::type>;
    // using Counter = OY::FHQCNT::Table<uint32_t, uint32_t, false, false, 1000000>;
    struct node {
        Deque m_q;
        Counter m_cnt;
        void push_back(uint32_t x) {
            m_q.push_back(x);
            m_cnt.add(x, 1);
        }
        void pop_back() {
            m_cnt.add(m_q.back(), -1);
            m_q.pop_back();
        }
        uint32_t count(uint32_t x) const {
            return m_cnt.query(x);
        }
        void merge(node &rhs) {
            if (m_q.size() >= rhs.m_q.size()) {
                for (uint32_t i = 0; i != rhs.m_q.size(); i++) m_q.push_back(rhs.m_q[i]);
            } else {
                for (uint32_t i = m_q.size() - 1; ~i; i--) rhs.m_q.push_front(m_q[i]);
                std::swap(m_q, rhs.m_q);
            }
            m_cnt.merge(rhs.m_cnt);
        }
    };
    std::vector<node> qs(n + q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t cnt;
        cin >> cnt;
        while (cnt--) {
            uint32_t x;
            cin >> x;
            qs[i].push_back(x);
        }
    }
    std::vector<uint32_t> buf;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            cin >> x >> y;
            qs[x - 1].push_back(y);
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            qs[x - 1].pop_back();
        } else if (op == '3') {
            uint32_t m;
            cin >> m;
            if (buf.size() < m) buf.resize(m);
            for (uint32_t j = 0; j != m; j++) {
                cin >> buf[j];
                if (qs[buf[j] - 1].m_q.empty()) j--, m--;
            }
            item a(qs[buf[0] - 1].m_q.query_all());
            for (uint32_t j = 1; j != m; j++) a = a + item(qs[buf[j] - 1].m_q.query_all());
            uint64_t tot{}, cnt{};
            for (uint32_t j = 0; j != m; j++) {
                tot += qs[buf[j] - 1].m_q.size();
                cnt += qs[buf[j] - 1].count(a.m_val);
            }
            if (cnt * 2 > tot)
                cout << a.m_val << endl;
            else
                cout << "-1\n";
        } else {
            uint32_t x1, x2, x3;
            cin >> x1 >> x2 >> x3;
            std::swap(qs[x1 - 1], qs[x3 - 1]);
            qs[x3 - 1].merge(qs[x2 - 1]);
        }
    }
}

int main() {
    solve_deque();
}