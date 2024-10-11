/*
最后修改:
20241011
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RECTUNION__
#define __OY_RECTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RU {
        using size_type = uint32_t;
        template <typename Tp>
        class CoverTree {
        public:
            struct node {
                Tp m_sum, m_area;
                size_type m_tag;
                Tp get() const { return m_tag ? m_area : m_sum; }
            };
        private:
            size_type m_size, m_cap;
            std::vector<node> m_sub;
            static void _update(node *sub, size_type i) { sub[i].m_sum = sub[i * 2].get() + sub[i * 2 + 1].get(); }
            template <typename Callback>
            void _work(size_type left, size_type right, Callback &&call) {
                auto sub = m_sub.data();
                size_type cur = left;
                if (cur)
                    while (true) {
                        size_type j = std::countr_zero(cur), nxt = cur + (size_type(1) << j);
                        if (nxt > right) break;
                        call(sub + ((m_cap + cur) >> j));
                        for (size_type i = (m_cap + cur) >> j, end = nxt < right ? (m_cap + nxt) >> (std::countr_zero(nxt) + 1) : 0; (i >>= 1) != end;) _update(sub, i);
                        cur = nxt;
                    }
                while (cur < right) {
                    size_type j = std::bit_width(cur ^ right), nxt = cur + (size_type(1) << (j - 1));
                    call(sub + ((m_cap + cur) >> (j - 1)));
                    for (size_type i = (m_cap + cur) >> (j - 1), end = nxt < right ? (m_cap + cur) >> j : 0; (i >>= 1) != end;) _update(sub, i);
                    cur = nxt;
                }
            }
        public:
            CoverTree() = default;
            template <typename InitMapping>
            CoverTree(size_type length, InitMapping mapping) {
                m_cap = std::bit_ceil(m_size = length);
                m_sub.assign(m_cap * 2, {});
                for (size_type i = 0; i != m_size; i++) m_sub[m_cap + i].m_area = mapping(i);
                for (size_type len = m_cap / 2, cnt = (m_size + 1) / 2, w = 2; len; len >>= 1, cnt = (cnt + 1) / 2, w <<= 1)
                    for (size_type i = len; i != len + cnt; i++) m_sub[i].m_area = m_sub[i * 2].m_area + m_sub[i * 2 + 1].m_area;
            }
            void add(size_type left, size_type right) {
                _work(left, right, [](node *p) { ++p->m_tag; });
            }
            void remove(size_type left, size_type right) {
                _work(left, right, [](node *p) { --p->m_tag; });
            }
            Tp query_all() const { return m_sub[1].get(); }
        };
        template <typename SizeType>
        class Solver {
        public:
            struct event {
                SizeType m_time, m_low, m_high;
                bool operator<(const event &rhs) const { return m_time < rhs.m_time; }
            };
            struct pair {
                SizeType m_val;
                size_type m_index;
                bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
            };
        private:
            std::vector<event> m_es;
            std::vector<pair> m_ps;
        public:
            Solver(size_type rect_cnt = 0) { m_es.reserve(rect_cnt * 2), m_ps.reserve(rect_cnt * 2); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) {
                m_es.push_back({x_min});
                m_es.push_back({x_max + 1});
                m_ps.push_back({y_min, (size_type)m_ps.size()});
                m_ps.push_back({y_max + 1, (size_type)m_ps.size()});
            }
            template <typename SumType = SizeType>
            SumType solve() {
                std::sort(m_ps.begin(), m_ps.end());
                std::vector<SizeType> ys;
                ys.reserve(m_ps.size());
                for (size_type i = 0; i != m_ps.size(); i++) {
                    if (!i || m_ps[i].m_val != m_ps[i - 1].m_val) ys.push_back(m_ps[i].m_val);
                    m_es[m_ps[i].m_index].m_low = m_es[m_ps[i].m_index ^ 1].m_high = ys.size() - 1;
                }
                std::sort(m_es.begin(), m_es.end());
                CoverTree<SizeType> S(ys.size() - 1, [&](size_type i) { return ys[i + 1] - ys[i]; });
                SumType h{}, last{}, ans{};
                for (auto &e : m_es) {
                    ans += h * (e.m_time - last);
                    last = e.m_time;
                    if (e.m_low < e.m_high)
                        S.add(e.m_low, e.m_high);
                    else
                        S.remove(e.m_high, e.m_low);
                    h = S.query_all();
                }
                return ans;
            }
        };
    }
}

#endif