/*
最后修改:
20241122
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SLIDEWINDOW__
#define __OY_SLIDEWINDOW__

#include <algorithm>
#include <cstdint>
#include <memory>
#include <numeric>
#include <vector>

namespace OY {
    namespace WINDOW {
        using size_type = uint32_t;
        template <typename Tp>
        struct node {
            size_type m_index;
            Tp m_value;
        };
        template <typename Tp, size_type MAXN>
        struct StaticBuffer {
            static node<Tp> s_buf[MAXN];
        };
        template <typename Tp, size_type MAXN>
        node<Tp> StaticBuffer<Tp, MAXN>::s_buf[MAXN];
        template <typename Callback, typename ShortenLeft, typename LengthenRight>
        void solve(size_type length, size_type window_len, Callback &&call, ShortenLeft &&shorten_call, LengthenRight &&lengthen_call) {
            size_type l = 0, r = 0;
            while (r != window_len) lengthen_call(r++);
            call(l, r - 1);
            while (r != length) {
                shorten_call(l++);
                lengthen_call(r++);
                call(l, r - 1);
            }
        }
        template <size_type MAXN, typename Mapping, typename Callback, typename Compare>
        void solve_rmq(size_type length, size_type window_len, Mapping &&mapping, Callback &&call, Compare &&comp) {
            using Tp = typename std::decay<decltype(mapping(0))>::type;
            auto &&buf = StaticBuffer<Tp, MAXN>::s_buf;
            size_type ql = 0, qr = 0;
            auto right_call = [&](size_type r) {
                Tp val = mapping(r);
                while (ql != qr && !comp(val, buf[qr - 1].m_value)) qr--;
                buf[qr++] = {r, val};
            };
            auto left_call = [&](size_type l) {
                if (ql != qr && buf[ql].m_index == l) ql++;
            };
            auto call2 = [&](size_type l, size_type r) { call(l, r, buf[ql].m_value); };
            solve(length, window_len, call2, left_call, right_call);
        }
        template <size_type MAXN, typename Mapping, typename Callback>
        void solve_max(size_type length, size_type window_len, Mapping &&mapping, Callback &&call) {
            using Tp = typename std::decay<decltype(mapping(0))>::type;
            solve_rmq<MAXN>(length, window_len, mapping, call, std::less<Tp>());
        }
        template <size_type MAXN, typename Mapping, typename Callback>
        void solve_min(size_type length, size_type window_len, Mapping &&mapping, Callback &&call) {
            using Tp = typename std::decay<decltype(mapping(0))>::type;
            solve_rmq<MAXN>(length, window_len, mapping, call, std::greater<Tp>());
        }
        template <typename Callback, typename ShortenLeft, typename LengthenRight>
        struct Iter {
            size_type m_window_len, m_l, m_r;
            Callback m_call;
            ShortenLeft m_shorten;
            LengthenRight m_lengthen;
            Iter(size_type window_len, Callback call, ShortenLeft shorten, LengthenRight lengthen) : m_window_len(window_len), m_l(-1), m_r(0), m_call(call), m_shorten(shorten), m_lengthen(lengthen) {
                while (m_r != m_window_len - 1) lengthen(m_r++);
            }
            auto next() -> decltype(m_call(0, 0)) {
                if (~m_l) m_shorten(m_l);
                m_l++;
                m_lengthen(m_r);
                return m_call(m_l, m_r++);
            }
        };
        template <typename Tp, typename Mapping, typename Compare>
        struct RMQIter {
            struct Deque {
                std::vector<node<Tp>> m_buf;
                size_type m_l, m_r;
                Deque(size_type cap) : m_buf(cap), m_l(0), m_r(0) {}
                bool empty() const { return m_l == m_r; }
                const node<Tp> &front() const { return m_buf[m_l]; }
                const node<Tp> &back() const { return m_buf[m_r - 1]; }
                void pop_front() { m_l++; }
                void pop_back() { m_r--; }
                void push_back(size_type i, Tp x) { m_buf[m_r++] = {i, x}; }
            };
            struct Callback {
                const Deque &m_q;
                Tp operator()(size_type, size_type) const { return m_q.front().m_value; }
            };
            struct Shorten {
                Deque &m_q;
                Mapping &m_mapping;
                void operator()(size_type l) {
                    if (!m_q.empty() && m_q.front().m_index == l) m_q.pop_front();
                }
            };
            struct Lengthen {
                Deque &m_q;
                Mapping &m_mapping;
                void operator()(size_type r) {
                    Tp val = m_mapping(r);
                    while (!m_q.empty() && !Compare()(val, m_q.back().m_value)) m_q.pop_back();
                    m_q.push_back(r, val);
                }
            };
            Deque m_q;
            Mapping m_mapping;
            Iter<Callback, Shorten, Lengthen> m_iter;
            RMQIter(size_type length, Mapping mapping, size_type window_len) : m_q(length), m_mapping(mapping), m_iter(window_len, {m_q}, {m_q, m_mapping}, {m_q, m_mapping}) {}
            Tp next() { return m_iter.next(); }
        };
        template <typename Tp, typename Mapping>
        using MaxIter = RMQIter<Tp, Mapping, std::less<Tp>>;
        template <typename Tp, typename Mapping>
        using MinIter = RMQIter<Tp, Mapping, std::greater<Tp>>;
    }
}

#endif
