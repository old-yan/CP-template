/*
最后修改:
20241008
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BITEX__
#define __OY_BITEX__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BITEX {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                using value_type = Node *;
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static void malloc(value_type &x, size_type length) { x = s_buf + s_use_cnt, s_use_cnt += length; }
                static void reset_buffer() { std::fill_n(s_buf, s_use_cnt, Node{}), s_use_cnt = 0; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt;
        template <typename Node>
        struct VectorBuffer {
            using value_type = std::vector<Node>;
            static void malloc(value_type &x, size_type length) { x.assign(length, {}); }
        };
        template <typename Tp, template <typename> typename BufferType = VectorBuffer>
        class Tree {
        public:
            struct node {
                Tp m_val[2];
                Tp calc(size_type i) const { return m_val[0] * i - m_val[1]; }
                node &operator+=(const node &rhs) {
                    m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
                    return *this;
                }
            };
            using buffer_type = BufferType<node>;
        private:
            size_type m_size, m_length;
            typename buffer_type::value_type m_sum;
            void _add(size_type i, node inc) {
                while (i < m_length) m_sum[i] += inc, i += lowbit(i + 1);
            }
        public:
            Tree() = default;
            Tree(size_type length) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (length) buffer_type::malloc(m_sum, m_length = std::bit_ceil(m_size = length));
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!length) return;
                buffer_type::malloc(m_sum, m_length = std::bit_ceil(m_size = length));
                Tp temp{};
                for (size_type i = 0; i != length; i++) {
                    Tp cur = mapping(i);
                    m_sum[i].m_val[0] = cur - temp, m_sum[i].m_val[1] = m_sum[i].m_val[0] * i, temp = cur;
                }
                if (length < m_length) m_sum[length].m_val[0] = -temp, m_sum[length].m_val[1] = m_sum[length].m_val[0] * length;
                for (size_type i = 0; i != m_length; i++) {
                    size_type j = i + lowbit(i + 1);
                    if (j < m_length) m_sum[j] += m_sum[i];
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            void add(size_type i, Tp inc) { _add(i, node{inc, Tp(inc * i)}), _add(i + 1, node{-inc, Tp(-inc * (i + 1))}); }
            void add(size_type left, size_type right, Tp inc) { _add(right + 1, node{-inc, Tp(-inc * (right + 1))}), _add(left, node{inc, Tp(inc * left)}); }
            Tp presum(size_type i) const {
                node res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += m_sum[j];
                return res.calc(i + 1);
            }
            Tp query(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += m_sum[j].m_val[0];
                return res;
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
            Tp query_all() const { return m_sum[m_length - 1].calc(m_length); }
            size_type kth(Tp k) const {
                size_type cursor = -1;
                node cur{};
                for (size_type d = m_length >> 1; d; d >>= 1)
                    if ((cur.m_val[0] + m_sum[cursor + d].m_val[0]) * (cursor + d + 1) - (cur.m_val[1] + m_sum[cursor + d].m_val[1]) <= k) cur += m_sum[cursor += d];
                return cursor + 1;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                Tp val{};
                for (size_type i = 0; i != m_size; i++) {
                    val += m_sum[i].m_val[0];
                    for (size_type ctz = std::countr_zero(~i); ctz--;) val -= m_sum[i - (size_type(1) << ctz)].m_val[0];
                    call(val);
                }
            }
        };
        template <typename Ostream, typename Tp, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Tp, BufferType> &x) {
            out << '[';
            x.enumerate([&out, &x, i = 0](Tp val) mutable { out << (i++ ? ", " : "") << val; });
            return out << "]";
        }
    };
    template <typename Tp, BITEX::size_type BUFFER = 1 << 22>
    using StaticBIT_ex = BITEX::Tree<Tp, BITEX::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp>
    using VectorBIT_ex = BITEX::Tree<Tp, BITEX::VectorBuffer>;
}

#endif