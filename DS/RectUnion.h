/*
最后修改:
20240526
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RECTUNION__
#define __OY_RECTUNION__

#include "ZkwTree.h"

namespace OY {
    namespace RECTUNION {
        using size_type = uint32_t;
        template <typename Tp>
        struct ZkwNode {
            using value_type = Tp;
            using modify_type = bool;
            using node_type = ZkwNode<Tp>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x, uint32_t len) {
                if (modify)
                    x->m_modify++, x->m_val = x->m_area;
                else if (!--x->m_modify)
                    x->m_val = x->m_sum;
            }
            static void com(const modify_type &modify, node_type *x) {}
            Tp m_val, m_sum, m_area;
            uint32_t m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_area = val; }
            void init_pushup(node_type *lchild, node_type *rchild) {
                m_area = lchild->m_area + rchild->m_area;
            }
            void pushup(node_type *lchild, node_type *rchild) {
                m_sum = lchild->m_val + rchild->m_val;
                m_val = m_modify ? m_area : m_sum;
            }
        };
        template <typename SizeType>
        struct Solver {
            struct event {
                SizeType m_time, m_low, m_high;
                bool operator<(const event &rhs) const { return m_time < rhs.m_time; }
            };
            struct pair {
                SizeType m_val;
                size_type m_index;
                bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
            };
            std::vector<event> m_es;
            std::vector<pair> m_ps;
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
                ZKW::Tree<ZkwNode<SizeType>> S(ys.size() - 1, [&](size_type i) { return ys[i + 1] - ys[i]; });
                SumType h{}, last{}, ans{};
                for (auto &e : m_es) {
                    ans += h * (e.m_time - last);
                    last = e.m_time;
                    if (e.m_low < e.m_high)
                        S.add(e.m_low, e.m_high - 1, 1);
                    else
                        S.add(e.m_high, e.m_low - 1, 0);
                    h = S.query_all();
                }
                return ans;
            }
        };
    }
}

#endif